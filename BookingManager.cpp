#include "BookingManager.h"
#include <QDate>
#include <QSqlQuery>

bool BookingManager::bookCar(int carId,
                             int renterId,
                             const QString &startDate,
                             const QString &endDate,
                             int &bookingId,
                             double &totalPrice,
                             QString &message)
{
    // Find the car and its owner before creating a booking.
    QSqlQuery carQuery;
    carQuery.prepare(
        "SELECT ownerId, pricePerDay, availableFrom, availableTo "
        "FROM Cars WHERE id = :carId"
        );
    carQuery.bindValue(":carId", carId);

    if (!carQuery.exec() || !carQuery.next()) {
        message = "Car not found";
        return false;
    }

    int ownerId = carQuery.value(0).toInt();
    double pricePerDay = carQuery.value(1).toDouble();
    QString availableFrom = carQuery.value(2).toString();
    QString availableTo = carQuery.value(3).toString();

    if (ownerId == renterId) {
        message = "You cannot book your own car";
        return false;
    }

    if (startDate < availableFrom || endDate > availableTo) {
        message = "Car is not available for those dates";
        return false;
    }

    // Dates must be valid and in order.
    QDate start = QDate::fromString(startDate, "yyyy-MM-dd");
    QDate end = QDate::fromString(endDate, "yyyy-MM-dd");
    int days = start.daysTo(end) + 1;

    if (!start.isValid() || !end.isValid() || days <= 0) {
        message = "Dates must use YYYY-MM-DD and end date must be after start date";
        return false;
    }

    // Prevent double booking by checking for overlapping date ranges.
    QSqlQuery overlapQuery;
    overlapQuery.prepare(
        "SELECT id FROM Bookings "
        "WHERE carId = :carId "
        "AND status = 'Booked' "
        "AND startDate <= :endDate "
        "AND endDate >= :startDate"
        );
    overlapQuery.bindValue(":carId", carId);
    overlapQuery.bindValue(":startDate", startDate);
    overlapQuery.bindValue(":endDate", endDate);

    if (!overlapQuery.exec()) {
        message = "Booking check failed";
        return false;
    }

    if (overlapQuery.next()) {
        message = "This car is already booked for those dates";
        return false;
    }

    totalPrice = days * pricePerDay;

    // Create the booking with Booked status until payment is completed.
    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO Bookings("
        "carId, renterId, ownerId, startDate, endDate, status, totalPrice) "
        "VALUES ("
        ":carId, :renterId, :ownerId, :startDate, :endDate, 'Booked', :totalPrice)"
        );
    insertQuery.bindValue(":carId", carId);
    insertQuery.bindValue(":renterId", renterId);
    insertQuery.bindValue(":ownerId", ownerId);
    insertQuery.bindValue(":startDate", startDate);
    insertQuery.bindValue(":endDate", endDate);
    insertQuery.bindValue(":totalPrice", totalPrice);

    if (!insertQuery.exec()) {
        message = "Booking failed";
        return false;
    }

    bookingId = insertQuery.lastInsertId().toInt();

    // Notify both users after a booking is made.
    QSqlQuery renterNotification;
    renterNotification.prepare(
        "INSERT INTO Notifications(userId, message) "
        "VALUES (:userId, :message)"
        );
    renterNotification.bindValue(":userId", renterId);
    renterNotification.bindValue(":message", "Booking created. Booking ID: " + QString::number(bookingId));
    renterNotification.exec();

    QSqlQuery ownerNotification;
    ownerNotification.prepare(
        "INSERT INTO Notifications(userId, message) "
        "VALUES (:userId, :message)"
        );
    ownerNotification.bindValue(":userId", ownerId);
    ownerNotification.bindValue(":message", "Your car was booked. Booking ID: " + QString::number(bookingId));
    ownerNotification.exec();

    message = "Booking successful";
    return true;
}

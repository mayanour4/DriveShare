#include "PaymentProxy.h"
#include <QSqlDatabase>
#include <QSqlQuery>

bool PaymentProxy::processPayment(int bookingId, int renterId, double &amountPaid, QString &errorMessage)
{
    // Proxy checks that this renter owns the booking before payment.
    QSqlQuery bookingQuery;
    bookingQuery.prepare(
        "SELECT ownerId, totalPrice, status "
        "FROM Bookings "
        "WHERE id = :bookingId AND renterId = :renterId"
        );
    bookingQuery.bindValue(":bookingId", bookingId);
    bookingQuery.bindValue(":renterId", renterId);

    if (!bookingQuery.exec() || !bookingQuery.next()) {
        errorMessage = "Booking not found for this user";
        return false;
    }

    int ownerId = bookingQuery.value(0).toInt();
    amountPaid = bookingQuery.value(1).toDouble();
    QString status = bookingQuery.value(2).toString();

    if (status == "Paid") {
        errorMessage = "This booking is already paid";
        return false;
    }

    // Check renter balance before changing any money.
    QSqlQuery balanceQuery;
    balanceQuery.prepare("SELECT balance FROM Users WHERE id = :renterId");
    balanceQuery.bindValue(":renterId", renterId);

    if (!balanceQuery.exec() || !balanceQuery.next()) {
        errorMessage = "Could not find renter balance";
        return false;
    }

    double renterBalance = balanceQuery.value(0).toDouble();

    if (renterBalance < amountPaid) {
        errorMessage = "Not enough balance to pay for this booking";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    // Payment transfer: renter loses money and owner receives money.
    QSqlQuery updateRenter;
    updateRenter.prepare(
        "UPDATE Users SET balance = balance - :amount "
        "WHERE id = :renterId"
        );
    updateRenter.bindValue(":amount", amountPaid);
    updateRenter.bindValue(":renterId", renterId);

    QSqlQuery updateOwner;
    updateOwner.prepare(
        "UPDATE Users SET balance = balance + :amount "
        "WHERE id = :ownerId"
        );
    updateOwner.bindValue(":amount", amountPaid);
    updateOwner.bindValue(":ownerId", ownerId);

    QSqlQuery updateBooking;
    updateBooking.prepare(
        "UPDATE Bookings SET status = 'Paid' "
        "WHERE id = :bookingId"
        );
    updateBooking.bindValue(":bookingId", bookingId);

    if (!updateRenter.exec() || !updateOwner.exec() || !updateBooking.exec()) {
        db.rollback();
        errorMessage = "Payment update failed";
        return false;
    }

    // Notify both users about the payment.
    QSqlQuery renterNotification;
    renterNotification.prepare(
        "INSERT INTO Notifications(userId, message) "
        "VALUES (:userId, :message)"
        );
    renterNotification.bindValue(":userId", renterId);
    renterNotification.bindValue(":message", "Payment sent for booking " + QString::number(bookingId));

    QSqlQuery ownerNotification;
    ownerNotification.prepare(
        "INSERT INTO Notifications(userId, message) "
        "VALUES (:userId, :message)"
        );
    ownerNotification.bindValue(":userId", ownerId);
    ownerNotification.bindValue(":message", "Payment received for booking " + QString::number(bookingId));

    if (!renterNotification.exec() || !ownerNotification.exec()) {
        db.rollback();
        errorMessage = "Notification update failed";
        return false;
    }

    db.commit();
    return true;
}

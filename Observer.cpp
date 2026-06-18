#include "Observer.h"
#include <QSqlQuery>
#include <QString>

Observer::~Observer()
{
}

void WatchObserver::notifyCarChanged(int carId)
{
    // Load the car that changed or was added.
    QSqlQuery carQuery;
    carQuery.prepare(
        "SELECT model, availableFrom, availableTo, pricePerDay "
        "FROM Cars WHERE id = :carId"
        );
    carQuery.bindValue(":carId", carId);

    if (!carQuery.exec() || !carQuery.next()) {
        return;
    }

    QString model = carQuery.value(0).toString();
    QString availableFrom = carQuery.value(1).toString();
    QString availableTo = carQuery.value(2).toString();
    double pricePerDay = carQuery.value(3).toDouble();

    // Find all users watching this car.
    QSqlQuery watchQuery;
    watchQuery.prepare(
        "SELECT userId, maxPrice, desiredFrom, desiredTo "
        "FROM Watchlist "
        "WHERE carId = :carId"
        );
    watchQuery.bindValue(":carId", carId);

    if (!watchQuery.exec()) {
        return;
    }

    while (watchQuery.next()) {
        int userId = watchQuery.value(0).toInt();
        double maxPrice = watchQuery.value(1).toDouble();
        QString desiredFrom = watchQuery.value(2).toString();
        QString desiredTo = watchQuery.value(3).toString();

        bool priceMatches = pricePerDay <= maxPrice;
        bool datesMatch = availableFrom <= desiredFrom && availableTo >= desiredTo;

        // Observer notification happens only when watch preferences match.
        if (priceMatches && datesMatch) {
            QSqlQuery notificationQuery;
            notificationQuery.prepare(
                "INSERT INTO Notifications(userId, message) "
                "VALUES (:userId, :message)"
                );
            notificationQuery.bindValue(":userId", userId);
            notificationQuery.bindValue(
                ":message",
                "Watched car is available: " + model + " (Car ID: " + QString::number(carId) + ")"
                );
            notificationQuery.exec();
        }
    }
}

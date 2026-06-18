#include "CarManager.h"
#include <QSqlQuery>

bool CarManager::addCar(const Car &car, int &carId, QString &message)
{
    // Insert the owner's car listing into the database.
    QSqlQuery query;
    query.prepare(
        "INSERT INTO Cars("
        "ownerId, model, year, mileage, location, "
        "availableFrom, availableTo, pricePerDay) "
        "VALUES ("
        ":ownerId, :model, :year, :mileage, :location, "
        ":availableFrom, :availableTo, :pricePerDay)"
        );

    query.bindValue(":ownerId", car.getOwnerId());
    query.bindValue(":model", car.getModel());
    query.bindValue(":year", car.getYear());
    query.bindValue(":mileage", car.getMileage());
    query.bindValue(":location", car.getLocation());
    query.bindValue(":availableFrom", car.getAvailableFrom());
    query.bindValue(":availableTo", car.getAvailableTo());
    query.bindValue(":pricePerDay", car.getPricePerDay());

    if (!query.exec()) {
        message = "Car listing failed";
        return false;
    }

    carId = query.lastInsertId().toInt();
    message = "Car listed successfully";
    return true;
}

bool CarManager::searchCars(const QString &location,
                            const QString &fromDate,
                            const QString &toDate,
                            QString &results,
                            QString &message)
{
    // Find cars available in the requested location and date range.
    QSqlQuery query;
    query.prepare(
        "SELECT id, model, year, mileage, location, availableFrom, availableTo, pricePerDay "
        "FROM Cars "
        "WHERE location = :location "
        "AND availableFrom <= :fromDate "
        "AND availableTo >= :toDate"
        );
    query.bindValue(":location", location);
    query.bindValue(":fromDate", fromDate);
    query.bindValue(":toDate", toDate);

    if (!query.exec()) {
        message = "Car search failed";
        return false;
    }

    results.clear();

    while (query.next()) {
        results += "Car ID: " + query.value(0).toString() + "\n";
        results += "Model: " + query.value(1).toString() + "\n";
        results += "Year: " + query.value(2).toString() + "\n";
        results += "Mileage: " + query.value(3).toString() + "\n";
        results += "Location: " + query.value(4).toString() + "\n";
        results += "Available: " + query.value(5).toString() + " to " + query.value(6).toString() + "\n";
        results += "Price/day: $" + query.value(7).toString() + "\n\n";
    }

    if (results.isEmpty()) {
        results = "No cars found";
    }

    message = "Search successful";
    return true;
}

bool CarManager::updateCar(int carId,
                           int ownerId,
                           const QString &availableFrom,
                           const QString &availableTo,
                           double pricePerDay,
                           QString &message)
{
    // Only the owner can update their own car listing.
    QSqlQuery query;
    query.prepare(
        "UPDATE Cars "
        "SET availableFrom = :availableFrom, "
        "availableTo = :availableTo, "
        "pricePerDay = :pricePerDay "
        "WHERE id = :carId "
        "AND ownerId = :ownerId"
        );

    query.bindValue(":availableFrom", availableFrom);
    query.bindValue(":availableTo", availableTo);
    query.bindValue(":pricePerDay", pricePerDay);
    query.bindValue(":carId", carId);
    query.bindValue(":ownerId", ownerId);

    if (!query.exec()) {
        message = "Car update failed";
        return false;
    }

    if (query.numRowsAffected() == 0) {
        message = "Car not found, or this is not your car";
        return false;
    }

    message = "Car listing updated successfully";
    return true;
}

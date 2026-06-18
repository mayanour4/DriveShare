#ifndef CARMANAGER_H
#define CARMANAGER_H

#include "Car.h"
#include <QString>

// Helper class for car listing and car search database logic.
class CarManager
{
public:
    // Adds a car to the Cars table.
    bool addCar(const Car &car, int &carId, QString &message);

    // Searches for cars by location and date range.
    bool searchCars(const QString &location,
                    const QString &fromDate,
                    const QString &toDate,
                    QString &results,
                    QString &message);

    // Updates availability and price for a car owned by the current user.
    bool updateCar(int carId,
                   int ownerId,
                   const QString &availableFrom,
                   const QString &availableTo,
                   double pricePerDay,
                   QString &message);
};

#endif // CARMANAGER_H

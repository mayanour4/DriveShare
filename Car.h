#ifndef CAR_H
#define CAR_H

#include <QString>

// Stores one car listing.
class Car
{
public:
    Car();

    int getOwnerId() const;
    QString getModel() const;
    int getYear() const;
    int getMileage() const;
    QString getLocation() const;
    QString getAvailableFrom() const;
    QString getAvailableTo() const;
    double getPricePerDay() const;

private:
    // Car fields saved to the Cars table.
    int ownerId;
    QString model;
    int year;
    int mileage;
    QString location;
    QString availableFrom;
    QString availableTo;
    double pricePerDay;

    friend class CarBuilder;
};

// Builder: builds a Car step by step from the listing form.
class CarBuilder
{
public:
    CarBuilder();

    // Each setter returns the builder so calls can be chained.
    CarBuilder& setOwnerId(int ownerId);
    CarBuilder& setModel(const QString& model);
    CarBuilder& setYear(int year);
    CarBuilder& setMileage(int mileage);
    CarBuilder& setLocation(const QString& location);
    CarBuilder& setAvailableFrom(const QString& availableFrom);
    CarBuilder& setAvailableTo(const QString& availableTo);
    CarBuilder& setPricePerDay(double pricePerDay);

    // Gives back the completed Car.
    Car build() const;

private:
    Car car;
};

#endif // CAR_H

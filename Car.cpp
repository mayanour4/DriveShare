#include "Car.h"

Car::Car()
    : ownerId(-1), year(0), mileage(0), pricePerDay(0.0)
{
}

// These getters let the UI/database code read the completed Car object.
int Car::getOwnerId() const
{
    return ownerId;
}

QString Car::getModel() const
{
    return model;
}

int Car::getYear() const
{
    return year;
}

int Car::getMileage() const
{
    return mileage;
}

QString Car::getLocation() const
{
    return location;
}

QString Car::getAvailableFrom() const
{
    return availableFrom;
}

QString Car::getAvailableTo() const
{
    return availableTo;
}

double Car::getPricePerDay() const
{
    return pricePerDay;
}

CarBuilder::CarBuilder()
{
}

// Builder setters fill in one car field at a time.
CarBuilder& CarBuilder::setOwnerId(int ownerId)
{
    car.ownerId = ownerId;
    return *this;
}

CarBuilder& CarBuilder::setModel(const QString& model)
{
    car.model = model;
    return *this;
}

CarBuilder& CarBuilder::setYear(int year)
{
    car.year = year;
    return *this;
}

CarBuilder& CarBuilder::setMileage(int mileage)
{
    car.mileage = mileage;
    return *this;
}

CarBuilder& CarBuilder::setLocation(const QString& location)
{
    car.location = location;
    return *this;
}

CarBuilder& CarBuilder::setAvailableFrom(const QString& availableFrom)
{
    car.availableFrom = availableFrom;
    return *this;
}

CarBuilder& CarBuilder::setAvailableTo(const QString& availableTo)
{
    car.availableTo = availableTo;
    return *this;
}

CarBuilder& CarBuilder::setPricePerDay(double pricePerDay)
{
    car.pricePerDay = pricePerDay;
    return *this;
}

Car CarBuilder::build() const
{
    // Return the finished Car object.
    return car;
}

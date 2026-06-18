#include "DatabaseManager.h"
#include <QSqlQuery>

QSqlDatabase DatabaseManager::db;

bool DatabaseManager::openDatabase()
{
    // Open the local SQLite database file.
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("driveshare.db");

    if (!db.open()) {
        return false;
    }

    QSqlQuery query;

    // create Users table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "email TEXT UNIQUE,"
            "password TEXT,"
            "securityQuestion1 TEXT,"
            "securityAnswer1 TEXT,"
            "securityQuestion2 TEXT,"
            "securityAnswer2 TEXT,"
            "securityQuestion3 TEXT,"
            "securityAnswer3 TEXT,"
            "balance REAL DEFAULT 1000)"
            )) {
        return false;
    }

    // seed test user
    if (!query.exec(
            "INSERT OR IGNORE INTO Users("
            "email, password, "
            "securityQuestion1, securityAnswer1, "
            "securityQuestion2, securityAnswer2, "
            "securityQuestion3, securityAnswer3, "
            "balance) "
            "VALUES ("
            "'test@test.com', '1234', "
            "'What is your first car?', 'Ford', "
            "'What city were you born in?', 'Dearborn', "
            "'What is your favorite color?', 'Blue', "
            "1000.0)"
            )) {
        return false;
    }

    // create Cars table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Cars ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "ownerId INTEGER,"
            "model TEXT,"
            "year INTEGER,"
            "mileage INTEGER,"
            "location TEXT,"
            "availableFrom TEXT,"
            "availableTo TEXT,"
            "pricePerDay REAL)"
            )) {
        return false;
    }

    // create Bookings table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Bookings ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "carId INTEGER,"
            "renterId INTEGER,"
            "ownerId INTEGER,"
            "startDate TEXT,"
            "endDate TEXT,"
            "status TEXT,"
            "totalPrice REAL)"
            )) {
        return false;
    }

    // create Notifications table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Notifications ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "userId INTEGER,"
            "message TEXT)"
            )) {
        return false;
    }

    // create Watchlist table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Watchlist ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "userId INTEGER,"
            "carId INTEGER,"
            "maxPrice REAL,"
            "desiredFrom TEXT,"
            "desiredTo TEXT)"
            )) {
        return false;
    }

    // create Messages table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Messages ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "senderId INTEGER,"
            "receiverId INTEGER,"
            "message TEXT)"
            )) {
        return false;
    }

    return true;
}

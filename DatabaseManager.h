#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

// Opens the SQLite database and creates required tables.
class DatabaseManager
{
public:
    // Called once when the app starts.
    static bool openDatabase();

private:
    static QSqlDatabase db;
};

#endif // DATABASEMANAGER_H

#include "AuthManager.h"
#include <QSqlQuery>

bool AuthManager::login(const QString &email, const QString &password, int &userId)
{
    // Check if the email and password match a user in the database.
    QSqlQuery query;
    query.prepare(
        "SELECT id FROM Users "
        "WHERE email = :email AND password = :password"
        );

    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        userId = query.value(0).toInt();
        return true;
    }

    return false;
}

bool AuthManager::registerUser(const QString &email, const QString &password,
                               const QString &question1, const QString &answer1,
                               const QString &question2, const QString &answer2,
                               const QString &question3, const QString &answer3)
{
    // Save a new user with three security questions.
    QSqlQuery query;
    query.prepare(
        "INSERT INTO Users("
        "email, password, "
        "securityQuestion1, securityAnswer1, "
        "securityQuestion2, securityAnswer2, "
        "securityQuestion3, securityAnswer3, "
        "balance) "
        "VALUES ("
        ":email, :password, "
        ":q1, :a1, "
        ":q2, :a2, "
        ":q3, :a3, "
        "1000)"
        );

    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.bindValue(":q1", question1);
    query.bindValue(":a1", answer1);
    query.bindValue(":q2", question2);
    query.bindValue(":a2", answer2);
    query.bindValue(":q3", question3);
    query.bindValue(":a3", answer3);

    return query.exec();
}

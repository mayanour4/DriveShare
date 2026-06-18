#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>

// Handles login and registration database checks.
class AuthManager
{
public:
    // Returns true when the user exists and stores the user id.
    bool login(const QString &email, const QString &password, int &userId);

    // Creates a user account with security questions.
    bool registerUser(const QString &email, const QString &password,
                      const QString &question1, const QString &answer1,
                      const QString &question2, const QString &answer2,
                      const QString &question3, const QString &answer3);
};

#endif // AUTHMANAGER_H

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

// Singleton: one shared session stores the logged-in user.
class SessionManager
{
public:
    // Access the one shared session object.
    static SessionManager* getInstance();

    // Save the current user.
    void login(int userId);

    // Read current session data.
    int getUserId() const;
    bool isLoggedIn() const;

private:
    // Private constructor prevents creating extra sessions.
    SessionManager();

    int currentUserId;
    bool loggedIn;
};

#endif // SESSIONMANAGER_H

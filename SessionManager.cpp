#include "SessionManager.h"

SessionManager::SessionManager()
    : currentUserId(-1), loggedIn(false)
{
}

SessionManager* SessionManager::getInstance()
{
    // Singleton instance is created once and shared everywhere.
    static SessionManager instance;
    return &instance;
}

void SessionManager::login(int userId)
{
    // Store the current logged-in user.
    currentUserId = userId;
    loggedIn = true;
}

int SessionManager::getUserId() const
{
    return currentUserId;
}

bool SessionManager::isLoggedIn() const
{
    return loggedIn;
}

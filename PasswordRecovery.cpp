#include "PasswordRecovery.h"

PasswordRecovery::PasswordRecovery()
    : next(nullptr)
{
}

PasswordRecovery::~PasswordRecovery()
{
}

void PasswordRecovery::setNext(PasswordRecovery *nextHandler)
{
    // Link this question checker to the next checker in the chain.
    next = nextHandler;
}

bool PasswordRecovery::handle(const QString &correctAnswer1, const QString &userAnswer1,
                              const QString &correctAnswer2, const QString &userAnswer2,
                              const QString &correctAnswer3, const QString &userAnswer3)
{
    // If this answer is wrong, stop the recovery process.
    if (!check(correctAnswer1, userAnswer1,
               correctAnswer2, userAnswer2,
               correctAnswer3, userAnswer3)) {
        return false;
    }

    // If this answer is right, continue to the next handler.
    if (next != nullptr) {
        return next->handle(correctAnswer1, userAnswer1,
                            correctAnswer2, userAnswer2,
                            correctAnswer3, userAnswer3);
    }

    return true;
}

bool Question1Handler::check(const QString &correctAnswer1, const QString &userAnswer1,
                             const QString &, const QString &,
                             const QString &, const QString &)
{
    // First security answer check.
    return correctAnswer1.trimmed().toLower() == userAnswer1.trimmed().toLower();
}

bool Question2Handler::check(const QString &, const QString &,
                             const QString &correctAnswer2, const QString &userAnswer2,
                             const QString &, const QString &)
{
    // Second security answer check.
    return correctAnswer2.trimmed().toLower() == userAnswer2.trimmed().toLower();
}

bool Question3Handler::check(const QString &, const QString &,
                             const QString &, const QString &,
                             const QString &correctAnswer3, const QString &userAnswer3)
{
    // Third security answer check.
    return correctAnswer3.trimmed().toLower() == userAnswer3.trimmed().toLower();
}

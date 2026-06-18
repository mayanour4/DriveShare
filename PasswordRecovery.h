#ifndef PASSWORDRECOVERY_H
#define PASSWORDRECOVERY_H

#include <QString>

// Chain of Responsibility: each handler checks one security answer.
class PasswordRecovery
{
public:
    PasswordRecovery();
    virtual ~PasswordRecovery();

    // Connects this handler to the next question handler.
    void setNext(PasswordRecovery *nextHandler);

    // Runs this check, then passes to the next handler if correct.
    bool handle(const QString &correctAnswer1, const QString &userAnswer1,
                const QString &correctAnswer2, const QString &userAnswer2,
                const QString &correctAnswer3, const QString &userAnswer3);

protected:
    // Each child class checks one answer.
    virtual bool check(const QString &correctAnswer1, const QString &userAnswer1,
                       const QString &correctAnswer2, const QString &userAnswer2,
                       const QString &correctAnswer3, const QString &userAnswer3) = 0;

private:
    PasswordRecovery *next;
};

// Checks answer 1.
class Question1Handler : public PasswordRecovery
{
protected:
    bool check(const QString &correctAnswer1, const QString &userAnswer1,
               const QString &correctAnswer2, const QString &userAnswer2,
               const QString &correctAnswer3, const QString &userAnswer3) override;
};

// Checks answer 2.
class Question2Handler : public PasswordRecovery
{
protected:
    bool check(const QString &correctAnswer1, const QString &userAnswer1,
               const QString &correctAnswer2, const QString &userAnswer2,
               const QString &correctAnswer3, const QString &userAnswer3) override;
};

// Checks answer 3.
class Question3Handler : public PasswordRecovery
{
protected:
    bool check(const QString &correctAnswer1, const QString &userAnswer1,
               const QString &correctAnswer2, const QString &userAnswer2,
               const QString &correctAnswer3, const QString &userAnswer3) override;
};

#endif // PASSWORDRECOVERY_H

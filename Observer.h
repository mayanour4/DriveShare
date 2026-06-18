#ifndef OBSERVER_H
#define OBSERVER_H

// Observer: watches car changes and notifies matching renters.
class Observer
{
public:
    virtual ~Observer();
    // Called when a car is added or changed.
    virtual void notifyCarChanged(int carId) = 0;
};

// Checks the watchlist and creates notifications.
class WatchObserver : public Observer
{
public:
    void notifyCarChanged(int carId) override;
};

#endif // OBSERVER_H

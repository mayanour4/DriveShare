#ifndef BOOKINGMANAGER_H
#define BOOKINGMANAGER_H

#include <QString>

// Helper class for booking logic so MainWindow can stay focused on the UI.
class BookingManager
{
public:
    // Creates a booking if the car exists, dates are valid, and there is no overlap.
    bool bookCar(int carId,
                 int renterId,
                 const QString &startDate,
                 const QString &endDate,
                 int &bookingId,
                 double &totalPrice,
                 QString &message);
};

#endif // BOOKINGMANAGER_H

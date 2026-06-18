#ifndef PAYMENTPROXY_H
#define PAYMENTPROXY_H

#include <QString>

// Proxy: controls access to the fake payment process.
class PaymentProxy
{
public:
    // Moves fake money from renter to owner and updates booking status.
    bool processPayment(int bookingId, int renterId, double &amountPaid, QString &errorMessage);
};

#endif // PAYMENTPROXY_H

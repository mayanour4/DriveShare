#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AuthManager.h"
#include "BookingManager.h"
#include "SessionManager.h"
#include "PasswordRecovery.h"
#include "Car.h"
#include "CarManager.h"
#include "PaymentProxy.h"
#include "Observer.h"
#include <QSqlQuery>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showLoggedOutView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLoggedOutView()
{
    // Mediator: show login tools, hide app tools.
    setAuthWidgetsVisible(true);
    setAppWidgetsVisible(false);
}

void MainWindow::showLoggedInView()
{
    // Mediator: hide login tools, show app tools.
    setAuthWidgetsVisible(false);
    setAppWidgetsVisible(true);
}

void MainWindow::setAuthWidgetsVisible(bool visible)
{
    // Login/register/password recovery group.
    QWidget *widgets[] = {
        ui->label,
        ui->label_2,
        ui->emailEdit,
        ui->label_3,
        ui->passwordEdit,
        ui->forgotPassButton,
        ui->loginButton,
        ui->registerButton,
        ui->label_4,
        ui->question1Edit,
        ui->answer1Edit,
        ui->question2Edit,
        ui->answer2Edit,
        ui->question3Edit,
        ui->answer3Edit
    };

    for (QWidget *widget : widgets) {
        widget->setVisible(visible);
    }
}

void MainWindow::setAppWidgetsVisible(bool visible)
{
    // Main DriveShare feature group shown after login.
    QWidget *widgets[] = {
        ui->carListingLabel,
        ui->carModelLabel,
        ui->carModelEdit,
        ui->carYearLabel,
        ui->carYearEdit,
        ui->carMileageLabel,
        ui->carMileageEdit,
        ui->carLocationLabel,
        ui->carLocationEdit,
        ui->availableFromLabel,
        ui->availableFromEdit,
        ui->availableToLabel,
        ui->availableToEdit,
        ui->pricePerDayLabel,
        ui->pricePerDayEdit,
        ui->addCarButton,

        ui->updateListingLabel,
        ui->updateCarIdLabel,
        ui->updateCarIdEdit,
        ui->updateFromLabel,
        ui->updateFromEdit,
        ui->updateToLabel,
        ui->updateToEdit,
        ui->updatePriceLabel,
        ui->updatePriceEdit,
        ui->updateCarButton,

        ui->searchCarsLabel,
        ui->searchLocationLabel,
        ui->searchLocationEdit,
        ui->searchFromLabel,
        ui->searchFromEdit,
        ui->searchToLabel,
        ui->searchToEdit,
        ui->searchCarsButton,

        ui->bookingLabel,
        ui->bookingCarIdLabel,
        ui->bookingCarIdEdit,
        ui->bookingFromLabel,
        ui->bookingFromEdit,
        ui->bookingToLabel,
        ui->bookingToEdit,
        ui->bookCarButton,

        ui->paymentLabel,
        ui->paymentBookingIdLabel,
        ui->paymentBookingIdEdit,
        ui->payButton,

        ui->viewNotificationsButton,
        ui->watchlistLabel,
        ui->watchCarIdLabel,
        ui->watchCarIdEdit,
        ui->watchMaxPriceLabel,
        ui->watchMaxPriceEdit,
        ui->watchFromLabel,
        ui->watchFromEdit,
        ui->watchToLabel,
        ui->watchToEdit,
        ui->watchCarButton,

        ui->messagesLabel,
        ui->messageReceiverEmailLabel,
        ui->messageReceiverEmailEdit,
        ui->messageTextLabel,
        ui->messageTextEdit,
        ui->sendMessageButton,
        ui->viewMessagesButton
    };

    for (QWidget *widget : widgets) {
        widget->setVisible(visible);
    }
}

void MainWindow::on_loginButton_clicked()
{
    // Login uses AuthManager, then SessionManager stores the session.
    QString email = ui->emailEdit->text();
    QString password = ui->passwordEdit->text();

    if(email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    AuthManager authManager;
    int userId = -1;

    if(authManager.login(email, password, userId)) {
        QMessageBox::information(this, "Success", "Login successful!");


        SessionManager::getInstance()->login(userId);
        showLoggedInView();
    }
    else {
        QMessageBox::critical(this, "Failed", "Invalid login");
    }
}

void MainWindow::on_addCarButton_clicked()
{
    // Owner lists a car after logging in.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in before listing a car");
        return;
    }

    QString model = ui->carModelEdit->text();
    QString yearText = ui->carYearEdit->text();
    QString mileageText = ui->carMileageEdit->text();
    QString location = ui->carLocationEdit->text();
    QString availableFrom = ui->availableFromEdit->text();
    QString availableTo = ui->availableToEdit->text();
    QString priceText = ui->pricePerDayEdit->text();

    if (model.isEmpty() || yearText.isEmpty() || mileageText.isEmpty() ||
        location.isEmpty() || availableFrom.isEmpty() || availableTo.isEmpty() ||
        priceText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all car fields");
        return;
    }

    int year = yearText.toInt();
    int mileage = mileageText.toInt();
    double pricePerDay = priceText.toDouble();

    // Builder pattern creates the car listing object.
    Car car = CarBuilder()
        .setOwnerId(SessionManager::getInstance()->getUserId())
        .setModel(model)
        .setYear(year)
        .setMileage(mileage)
        .setLocation(location)
        .setAvailableFrom(availableFrom)
        .setAvailableTo(availableTo)
        .setPricePerDay(pricePerDay)
        .build();

    CarManager carManager;
    int carId = -1;
    QString message;

    if (carManager.addCar(car, carId, message)) {
        // Observer pattern checks if any watched cars now match.
        WatchObserver observer;
        observer.notifyCarChanged(carId);

        QMessageBox::information(this, "Success", message);
    } else {
        QMessageBox::critical(this, "Failed", message);
    }
}

void MainWindow::on_updateCarButton_clicked()
{
    // Owner updates availability and price for one of their cars.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in before updating a car");
        return;
    }

    QString carIdText = ui->updateCarIdEdit->text();
    QString availableFrom = ui->updateFromEdit->text();
    QString availableTo = ui->updateToEdit->text();
    QString priceText = ui->updatePriceEdit->text();

    if (carIdText.isEmpty() || availableFrom.isEmpty() ||
        availableTo.isEmpty() || priceText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all update fields");
        return;
    }

    int carId = carIdText.toInt();

    CarManager carManager;
    QString message;

    if (carManager.updateCar(carId,
                             SessionManager::getInstance()->getUserId(),
                             availableFrom,
                             availableTo,
                             priceText.toDouble(),
                             message)) {
        // Observer pattern checks watchers after the listing changes.
        WatchObserver observer;
        observer.notifyCarChanged(carId);

        QMessageBox::information(this, "Success", message);
    } else {
        QMessageBox::warning(this, "Failed", message);
    }
}

void MainWindow::on_searchCarsButton_clicked()
{
    // Renter searches for cars by location and date range.
    QString location = ui->searchLocationEdit->text();
    QString fromDate = ui->searchFromEdit->text();
    QString toDate = ui->searchToEdit->text();

    if (location.isEmpty() || fromDate.isEmpty() || toDate.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter location, from date, and to date");
        return;
    }

    CarManager carManager;
    QString results;
    QString message;

    if (carManager.searchCars(location, fromDate, toDate, results, message)) {
        QMessageBox::information(this, "Search Results", results);
    } else {
        QMessageBox::critical(this, "Failed", message);
    }
}

void MainWindow::on_bookCarButton_clicked()
{
    // Renter books a car if dates do not overlap with another booking.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in before booking a car");
        return;
    }

    QString carIdText = ui->bookingCarIdEdit->text();
    QString startDate = ui->bookingFromEdit->text();
    QString endDate = ui->bookingToEdit->text();

    if (carIdText.isEmpty() || startDate.isEmpty() || endDate.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter car ID, from date, and to date");
        return;
    }

    BookingManager bookingManager;
    int bookingId = -1;
    double totalPrice = 0;
    QString message;

    bool success = bookingManager.bookCar(
        carIdText.toInt(),
        SessionManager::getInstance()->getUserId(),
        startDate,
        endDate,
        bookingId,
        totalPrice,
        message
        );

    if (success) {
        QMessageBox::information(this, "Success",
                                 "Booking successful! Booking ID: " + QString::number(bookingId) +
                                     "\nTotal price: $" + QString::number(totalPrice));
    } else {
        QMessageBox::warning(this, "Failed", message);
    }
}

void MainWindow::on_payButton_clicked()
{
    // Payment is handled through the PaymentProxy class.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in before paying");
        return;
    }

    QString bookingIdText = ui->paymentBookingIdEdit->text();

    if (bookingIdText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a booking ID");
        return;
    }

    int bookingId = bookingIdText.toInt();
    double amountPaid = 0.0;
    QString errorMessage;

    PaymentProxy paymentProxy;

    // Proxy pattern simulates secure payment processing.
    bool success = paymentProxy.processPayment(
        bookingId,
        SessionManager::getInstance()->getUserId(),
        amountPaid,
        errorMessage
        );

    if (success) {
        QMessageBox::information(this, "Success",
                                 "Payment successful! Amount paid: $" + QString::number(amountPaid));
    } else {
        QMessageBox::critical(this, "Payment Failed", errorMessage);
    }
}

void MainWindow::on_viewNotificationsButton_clicked()
{
    // Show in-app notifications for the logged-in user.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in to view notifications");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT message FROM Notifications "
        "WHERE userId = :userId "
        "ORDER BY id DESC"
        );
    query.bindValue(":userId", SessionManager::getInstance()->getUserId());

    if (!query.exec()) {
        QMessageBox::critical(this, "Failed", "Could not load notifications");
        return;
    }

    QString notifications;

    while (query.next()) {
        notifications += "- " + query.value(0).toString() + "\n";
    }

    if (notifications.isEmpty()) {
        QMessageBox::information(this, "Notifications", "No notifications yet");
    } else {
        QMessageBox::information(this, "Notifications", notifications);
    }
}

void MainWindow::on_watchCarButton_clicked()
{
    // Add a car to the watchlist for Observer notifications.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in before watching a car");
        return;
    }

    QString carIdText = ui->watchCarIdEdit->text();
    QString maxPriceText = ui->watchMaxPriceEdit->text();
    QString desiredFrom = ui->watchFromEdit->text();
    QString desiredTo = ui->watchToEdit->text();

    if (carIdText.isEmpty() || maxPriceText.isEmpty() ||
        desiredFrom.isEmpty() || desiredTo.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all watch fields");
        return;
    }

    int carId = carIdText.toInt();
    double maxPrice = maxPriceText.toDouble();

    QSqlQuery carQuery;
    carQuery.prepare("SELECT id FROM Cars WHERE id = :carId");
    carQuery.bindValue(":carId", carId);

    if (!carQuery.exec() || !carQuery.next()) {
        QMessageBox::critical(this, "Failed", "Car not found");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO Watchlist(userId, carId, maxPrice, desiredFrom, desiredTo) "
        "VALUES (:userId, :carId, :maxPrice, :desiredFrom, :desiredTo)"
        );
    query.bindValue(":userId", SessionManager::getInstance()->getUserId());
    query.bindValue(":carId", carId);
    query.bindValue(":maxPrice", maxPrice);
    query.bindValue(":desiredFrom", desiredFrom);
    query.bindValue(":desiredTo", desiredTo);

    if (query.exec()) {
        WatchObserver observer;
        observer.notifyCarChanged(carId);
        QMessageBox::information(this, "Success", "Car added to watchlist!");
    } else {
        QMessageBox::critical(this, "Failed", "Could not watch car");
    }
}

void MainWindow::on_sendMessageButton_clicked()
{
    // Send an in-app message to another user by email.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in before sending a message");
        return;
    }

    QString receiverEmail = ui->messageReceiverEmailEdit->text();
    QString message = ui->messageTextEdit->text();

    if (receiverEmail.isEmpty() || message.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter receiver email and message");
        return;
    }

    QSqlQuery userQuery;
    userQuery.prepare("SELECT id FROM Users WHERE email = :email");
    userQuery.bindValue(":email", receiverEmail);

    if (!userQuery.exec() || !userQuery.next()) {
        QMessageBox::critical(this, "Failed", "Receiver email not found");
        return;
    }

    int receiverId = userQuery.value(0).toInt();

    QSqlQuery messageQuery;
    messageQuery.prepare(
        "INSERT INTO Messages(senderId, receiverId, message) "
        "VALUES (:senderId, :receiverId, :message)"
        );
    messageQuery.bindValue(":senderId", SessionManager::getInstance()->getUserId());
    messageQuery.bindValue(":receiverId", receiverId);
    messageQuery.bindValue(":message", message);

    if (!messageQuery.exec()) {
        QMessageBox::critical(this, "Failed", "Message failed");
        return;
    }

    QSqlQuery notificationQuery;
    notificationQuery.prepare(
        "INSERT INTO Notifications(userId, message) "
        "VALUES (:userId, :message)"
        );
    notificationQuery.bindValue(":userId", receiverId);
    notificationQuery.bindValue(":message", "You received a new message");
    notificationQuery.exec();

    QMessageBox::information(this, "Success", "Message sent!");
}

void MainWindow::on_viewMessagesButton_clicked()
{
    // Show received messages for the logged-in user.
    if (!SessionManager::getInstance()->isLoggedIn()) {
        QMessageBox::warning(this, "Error", "Please log in to view messages");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT Users.email, Messages.message "
        "FROM Messages "
        "JOIN Users ON Messages.senderId = Users.id "
        "WHERE Messages.receiverId = :userId "
        "ORDER BY Messages.id DESC"
        );
    query.bindValue(":userId", SessionManager::getInstance()->getUserId());

    if (!query.exec()) {
        QMessageBox::critical(this, "Failed", "Could not load messages");
        return;
    }

    QString messages;

    while (query.next()) {
        messages += "From: " + query.value(0).toString() + "\n";
        messages += query.value(1).toString() + "\n\n";
    }

    if (messages.isEmpty()) {
        QMessageBox::information(this, "Messages", "No messages yet");
    } else {
        QMessageBox::information(this, "Messages", messages);
    }
}

void MainWindow::on_forgotPassButton_clicked()
{
    // Password recovery uses the three-answer chain.
    QString email = ui->emailEdit->text();
    QString newPassword = ui->passwordEdit->text();
    QString userAnswer1 = ui->answer1Edit->text();
    QString userAnswer2 = ui->answer2Edit->text();
    QString userAnswer3 = ui->answer3Edit->text();

    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your email");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT securityQuestion1, securityAnswer1, "
        "securityQuestion2, securityAnswer2, "
        "securityQuestion3, securityAnswer3 "
        "FROM Users WHERE email = :email"
        );
    query.bindValue(":email", email);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Failed", "No account found with that email");
        return;
    }

    QString question1 = query.value(0).toString();
    QString correctAnswer1 = query.value(1).toString();
    QString question2 = query.value(2).toString();
    QString correctAnswer2 = query.value(3).toString();
    QString question3 = query.value(4).toString();
    QString correctAnswer3 = query.value(5).toString();

    ui->question1Edit->setText(question1);
    ui->question2Edit->setText(question2);
    ui->question3Edit->setText(question3);

    if (newPassword.isEmpty() ||
        userAnswer1.isEmpty() ||
        userAnswer2.isEmpty() ||
        userAnswer3.isEmpty()) {
        QMessageBox::information(this, "Security Questions",
                                 "Enter your answers and new password, then click Forgot Password again.");
        return;
    }

    Question1Handler question1Handler;
    Question2Handler question2Handler;
    Question3Handler question3Handler;

    // Chain of Responsibility checks all three security answers.
    question1Handler.setNext(&question2Handler);
    question2Handler.setNext(&question3Handler);

    bool answersCorrect = question1Handler.handle(
        correctAnswer1, userAnswer1,
        correctAnswer2, userAnswer2,
        correctAnswer3, userAnswer3
        );

    if (!answersCorrect) {
        QMessageBox::critical(this, "Failed", "Security answers are incorrect");
        return;
    }

    query.prepare(
        "UPDATE Users SET password = :password "
        "WHERE email = :email"
        );
    query.bindValue(":password", newPassword);
    query.bindValue(":email", email);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Password reset successful!");
    } else {
        QMessageBox::critical(this, "Failed", "Password reset failed");
    }
}

void MainWindow::on_registerButton_clicked()
{
    // Registration saves login info and security questions.
    QString email = ui->emailEdit->text();
    QString password = ui->passwordEdit->text();

    QString q1 = ui->question1Edit->text();
    QString a1 = ui->answer1Edit->text();
    QString q2 = ui->question2Edit->text();
    QString a2 = ui->answer2Edit->text();
    QString q3 = ui->question3Edit->text();
    QString a3 = ui->answer3Edit->text();

    if (email.isEmpty() || password.isEmpty() ||
        q1.isEmpty() || a1.isEmpty() ||
        q2.isEmpty() || a2.isEmpty() ||
        q3.isEmpty() || a3.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    AuthManager authManager;

    if (authManager.registerUser(email, password, q1, a1, q2, a2, q3, a3)) {
        QMessageBox::information(this, "Success", "Registration successful!");
    } else {
        QMessageBox::critical(this, "Failed", "Registration failed. Email may already exist.");
    }
}

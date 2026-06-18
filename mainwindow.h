#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// MainWindow is the UI mediator for the application.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Qt auto-connects these slots to matching button names.
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_forgotPassButton_clicked();
    void on_addCarButton_clicked();
    void on_updateCarButton_clicked();
    void on_searchCarsButton_clicked();
    void on_bookCarButton_clicked();
    void on_payButton_clicked();
    void on_viewNotificationsButton_clicked();
    void on_watchCarButton_clicked();
    void on_sendMessageButton_clicked();
    void on_viewMessagesButton_clicked();

private:
    // Mediator helpers: MainWindow controls which UI sections are visible.
    void showLoggedOutView();
    void showLoggedInView();
    // These grouped helpers keep showLoggedOutView/showLoggedInView short.
    void setAuthWidgetsVisible(bool visible);
    void setAppWidgetsVisible(bool visible);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminrequests.h"
#include "userrequests.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAdminpbClicked();
    void onStdpbClicked();
    void onAdminCancelpbClicked();
    void onUserCancelpbClicked();
    void onAdminloginpbClicked();
    void onUserloginpbClicked();
    void onQuitpbClicked();

    void onAdmintoHomePushButtonClicked();
    void onUsertoHomePushButtonClicked();

    void onClientResponseReceived();



private:
    Ui::MainWindow *ui;
    AdminRequests *adminReq;
    UserRequests *userReq;
};
#endif // MAINWINDOW_H

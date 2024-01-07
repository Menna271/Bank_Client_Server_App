#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "QDebug"
#include <QMessageBox>

Client client;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Replacing default error-prone functions and connecting the new ones*/
    connect(ui->adminpb, &ui->adminpb->clicked, this, &MainWindow::onAdminpbClicked);
    connect(ui->stdpb, &ui->stdpb->clicked, this, &MainWindow::onStdpbClicked);
    connect(ui->adminCancelpb, &ui->adminCancelpb->clicked, this, &MainWindow::onAdminCancelpbClicked);
    connect(ui->userCancelpb, &ui->userCancelpb->clicked, this, &MainWindow::onUserCancelpbClicked);
    connect(ui->adminloginpb, &ui->adminloginpb->clicked, this, &MainWindow::onAdminloginpbClicked);
    connect(ui->userloginpb, &ui->userloginpb->clicked, this, &MainWindow::onUserloginpbClicked);
    connect(ui->quitpb, &ui->quitpb->clicked, this, &MainWindow::onQuitpbClicked);

    /* Connecting client signal with mainwindow slot*/
    connect (&client, &Client::login_response_received, this, &MainWindow::onClientResponseReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAdminpbClicked()
{
    client.connect_to_server("menna");
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onStdpbClicked()
{
    client.connect_to_server("menna");
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onAdminCancelpbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onUserCancelpbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onAdminloginpbClicked()
{
    QString username = ui->adminusernamele->text();
    QString password = ui->adminpasswordle->text();

    QByteArray data;
    qint32 request = 0;
    QString usertype = "Admin";
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << usertype << username << password;

    client.sendData(data);
}

void MainWindow::onUserloginpbClicked()
{
    QString username = ui->usernamele->text();
    QString password = ui->userpasswordle->text();

    QByteArray data;
    qint32 request = 0;
    QString usertype = "StandardUser";
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << usertype << username << password;

    client.sendData(data);
}

void MainWindow::onClientResponseReceived()
{
    if(client.getLoginStatus())
    {
        if(ui->stackedWidget->currentIndex() == 1)
        {
            this->hide();
            adminReq = new AdminRequests(this);
            adminReq->show();
            connect (adminReq, &AdminRequests::AdmintoHomePushButtonClicked,
                    this, &MainWindow::onAdmintoHomePushButtonClicked);
        }

        if(ui->stackedWidget->currentIndex() == 2)
        {
            this->hide();
            userReq = new UserRequests(this);
            userReq->show();
            connect (userReq, &UserRequests::UsertoHomePushButtonClicked,
                    this, &MainWindow::onUsertoHomePushButtonClicked);
        }
    }
    else
    {
        QMessageBox::warning(this,"Login","Username or password is not correct.");
    }
}

void MainWindow::onAdmintoHomePushButtonClicked()
{
    adminReq->close();
    this->show();
}

void MainWindow::onUsertoHomePushButtonClicked()
{
    userReq->close();
    this->show();
}

void MainWindow::onQuitpbClicked()
{
    QByteArray data;
    qint32 request = 10;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request;
    client.sendData(data);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Quit", "Are you sure you want to quit?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
}


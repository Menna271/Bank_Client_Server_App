#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <qlocalserver.h>
#include <qlocalsocket.h>
#include "baseuser.h"
#include <QThread>
#include <QDebug>
#include <QDataStream>

class Server : public QObject, public BaseUser
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

signals:
    void finished();

public slots:
    void onNewConnection();
    void onReadyRead();

private:
    QLocalServer* m_server;
    QLocalSocket *client_socket;
    bool isUserLoggedin;

    enum Requests
    {
        login, getNumber, viewBalance, transactionHistory,
        Database, createUser, deleteUser, updateUser,
        newTransaction, moneyTransfer, quit
    };
};

#endif // SERVER_H

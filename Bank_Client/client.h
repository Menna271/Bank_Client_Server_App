#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <qlocalsocket.h>
#include <QJsonArray>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void connect_to_server(QString serverName);
    void sendData(QString info);
    bool getLoginStatus();
    QString getAccountNumber();
    qint32 getAccountBalance();
    QJsonArray getTransactionsHistory();
    QJsonArray getAdminData();
    QJsonArray getUserData();
    bool getNewUserValidity();
    bool getAccountDeletedConfirmation();
    qint32 getUserUpdateConfirmation();
    bool getNewTransactionStatus();
    qint32 getMoneyTransferStatus();

signals:
    void login_response_received();
    void account_number_received();
    void account_balance_received();
    void transactions_history_received();
    void database_received();
    void new_user_validity_received();
    void account_deletion_received();
    void user_update_received();
    void new_transaction_received();
    void money_transfer_received();

public slots:
    void onReadyRead();

private:
    QLocalSocket* m_client;
    bool m_isUserLogged;
    QString m_accountNumber;
    qint32 m_accountBalance;
    QJsonArray m_admin_data;
    QJsonArray m_user_data;
    QJsonArray m_transactions_history;
    bool m_is_new_user_valid;
    bool m_is_account_deleted;
    qint32 m_is_user_updated;
    bool m_is_transaction_possible;
    qint32 m_is_money_transfer_possible;
    enum ResponseCode
    {
        Login, AccountNumber, AccountBalance,
        TransactionHistory, Database,
        NewUser, DeleteUser, UpdateUser,
        NewTransaction, MoneyTransfer
    };
    //QString response;
};

#endif // CLIENT_H

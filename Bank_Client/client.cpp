#include "client.h"
#include <QDebug>

Client::Client(QObject *parent) : QObject{parent}
{
    m_client = new QLocalSocket(this);

    connect (m_client, &QLocalSocket::readyRead, this, &Client::onReadyRead);

}

void Client::connect_to_server(QString serverName)
{
    m_client->abort();
    m_client->connectToServer(serverName);
}

void Client::sendData(QString info)
{
    m_client->write(info.toUtf8(), info.length());
    m_client->flush();
}

void Client::onReadyRead()
{
    if(m_client->bytesAvailable())
    {
        QDataStream in(m_client);
        in.setVersion(QDataStream::Qt_6_6);
        qint32 code;
        in >> code;
        switch (code)
        {
        case Login:
        {
            in >> m_isUserLogged;
            emit login_response_received();
            break;
        }

        case AccountNumber:
        {
            in >> m_accountNumber;
            emit account_number_received();
            break;
        }

        case AccountBalance:
        {
            in >> m_accountBalance;
            emit account_balance_received();
            break;
        }

        case TransactionHistory:
        {
            in >> m_transactions_history;
            emit transactions_history_received();
            break;
        }

        case Database:
        {
            in >> m_admin_data >> m_user_data >> m_transactions_history;
            emit database_received();
            break;
        }

        case NewUser:
        {
            in >> m_is_new_user_valid;
            emit new_user_validity_received();
            break;
        }

        case DeleteUser:
        {
            in >> m_is_account_deleted;
            emit account_deletion_received();
            break;
        }

        case UpdateUser:
        {
            in >> m_is_user_updated;
            emit user_update_received();
            break;
        }

        case NewTransaction:
        {
            in >> m_is_transaction_possible;
            emit new_transaction_received();
            break;
        }

        case MoneyTransfer:
        {
            in >> m_is_money_transfer_possible;
            emit money_transfer_received();
            break;
        }
        }
    }
}

bool Client::getLoginStatus()
{
    return m_isUserLogged;
}

QString Client::getAccountNumber()
{
    return m_accountNumber;
}

qint32 Client::getAccountBalance()
{
    return m_accountBalance;
}

QJsonArray Client::getTransactionsHistory()
{
    return m_transactions_history;
}

QJsonArray Client::getAdminData()
{
    return m_admin_data;
}

QJsonArray Client::getUserData()
{
    return m_user_data;
}

bool Client::getNewUserValidity()
{
    return m_is_new_user_valid;
}

bool Client::getAccountDeletedConfirmation()
{
    return m_is_account_deleted;
}

qint32 Client::getUserUpdateConfirmation()
{
    return m_is_user_updated;
}

bool Client::getNewTransactionStatus()
{
    return m_is_transaction_possible;
}

qint32 Client::getMoneyTransferStatus()
{
    return m_is_money_transfer_possible;
}

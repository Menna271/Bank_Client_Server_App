#include "server.h"

Server::Server(QObject *parent) : QObject{parent}
{
    m_server = new QLocalServer();
    m_server->listen("menna");

    qDebug() << "Server started.";

    connect(m_server, &QLocalServer::newConnection,
            this, &Server::onNewConnection);

}

void Server::onNewConnection()
{
    QThread* thread = new QThread();
    Server* server = new Server();
    server->moveToThread(thread);
    client_socket = m_server->nextPendingConnection();
    connect(client_socket, &QLocalSocket::readyRead,
            server, &Server::onReadyRead);
    connect(thread, &QThread::started,
            server, &Server::onReadyRead, Qt::QueuedConnection);

    thread->start();

}

void Server::onReadyRead()
{
    qDebug() << "Server ready to read" << QThread::currentThreadId();
    QLocalSocket *client_socket = qobject_cast<QLocalSocket *>(sender());

    /* Creating TLS to avoid overwriting the variables by other threads */
    static thread_local QString currentUser = "";
    static thread_local QString currentUsername = "";

    if (client_socket)
    {
        QDataStream in(client_socket);
        in.setVersion(QDataStream::Qt_6_6);

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_6);

        qint32 reqKey;
        in >> reqKey;

        switch(reqKey)
        {
        case login:
        {

            QString user_type,username,password;
            in >> user_type >> username >> password;
            currentUser = user_type;
            qDebug() << "Thread " << QThread::currentThreadId() << ": user type = " << currentUser;
            if(user_type == "StandardUser")
                currentUsername = username;

            bool response = loginCheck(user_type,username,password);

            out << login << response;
            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }
        case getNumber:
        {
            QString acc_number;
            if (currentUser == "StandardUser")
                acc_number = getAccountNumber(currentUsername);
            else
            {
                QString usrn;
                in >> usrn;
                acc_number = getAccountNumber(usrn);
            }
            out << getNumber << acc_number;
            client_socket->write(data, data.length());
            client_socket->flush();

            break;
        }

        case viewBalance:
        {
            QString num;
            qint32 acc_balance;
            if (currentUser == "StandardUser")
            {
                num = getAccountNumber(currentUsername);
                acc_balance = viewAccountBalance(num);
            }
            else
            {
                in >> num;
                acc_balance = viewAccountBalance(num);
            }

            out << viewBalance << acc_balance;
            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case transactionHistory:
        {
            QString num;
            if(currentUser == "StandardUser")
            {
                num = getAccountNumber(currentUsername);
            }
            else
            {
                in >> num;
            }
            qint32 cnt;
            in >> cnt;
            QJsonArray history = viewTransactionHistory(num,cnt);
            out << transactionHistory << history;
            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case Database:
        {
            arrayExtraction("Admins");
            out << Database << admin;
            arrayExtraction("StandardUser");
            out << user;
            arrayExtraction("Transactions");
            out << transactions;

            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case createUser:
        {
            QString username,password,fullname,age,balance;
            in >> username >> password >> fullname
                >> age >> balance;
            bool response = createNewUser(username,password,fullname,age,balance);
            out << createUser << response;

            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case deleteUser:
        {
            QString num;
            in >> num;
            bool response = DeleteUser(num);
            out << deleteUser << response;
            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case updateUser:
        {
            QString accnum,item,dataValue;
            in >> accnum >> item >> dataValue;
            qint32 response = UpdateUser(accnum,item,dataValue);

            out << updateUser << response;
            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case newTransaction:
        {
            QString accNum = getAccountNumber(currentUsername);
            QString transactionKey, amount;
            in >> transactionKey >> amount;
            bool response = MakeNewTransaction(accNum,transactionKey,amount);
            qDebug() << response;

            out << newTransaction << response;
            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case moneyTransfer:
        {
            QString sender = getAccountNumber(currentUsername);
            QString accNum, amount;
            in >> accNum >> amount;
            qint32 response = TransferMoney(sender,accNum,amount);
            qDebug() << response;

            out << moneyTransfer << response;
            client_socket->write(data, data.length());
            client_socket->flush();
            break;
        }

        case quit:
        {
            emit finished();
            QThread::currentThread()->terminate();
            QThread::currentThread()->deleteLater();
            break;
        }
        }
    }
}

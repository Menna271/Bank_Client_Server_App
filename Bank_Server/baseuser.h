#ifndef BASEUSER_H
#define BASEUSER_H

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>
#include <QThreadStorage>
#include <QDebug>
#include <QDate>
#include <cmath>
#include <QRandomGenerator>

class BaseUser
{
public:
    BaseUser();
    QString generateAccountNumber();
    void arrayExtraction(QString arrKey);
    bool loginCheck(QString userType, QString name, QString pass);
    QString getAccountNumber(QString username);
    qint32 viewAccountBalance(QString accountnumber);
    QJsonArray viewTransactionHistory(QString accountnumber, qint32 count);
    bool createNewUser(QString username,QString password,QString fullname,QString age,QString balance);
    bool DeleteUser(QString accountNum);
    qint32 UpdateUser(QString num, QString key, QString data);
    bool MakeNewTransaction(QString accNum, QString code, QString amount);
    qint32 TransferMoney(QString sender, QString receiver, QString amount);
protected:
    QString path = "D:/Qt/Qt_Workspace/Bank_Client_Server_Final_Project/";
    QFile file = path + "BankDataBase.json";
    QJsonObject obj;
    static thread_local QJsonArray admin;
    static thread_local QJsonArray user;
    static thread_local QJsonArray transactions;
    QMutex mutex;
};

#endif // BASEUSER_H

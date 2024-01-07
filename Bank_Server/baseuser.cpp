#include "baseuser.h"

BaseUser::BaseUser() {}

/* Creating TLS to avoid overwriting the arrays by other threads */
thread_local QJsonArray BaseUser::admin = QJsonArray();
thread_local QJsonArray BaseUser::user = QJsonArray();
thread_local QJsonArray BaseUser::transactions = QJsonArray();

void BaseUser::arrayExtraction(QString arrKey)
{
    file.open(QFile::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    obj = doc.object();
    if (arrKey == "Admin")
    {
        admin = obj["Admins"].toArray();
    }

    else if (arrKey == "StandardUser")
    {
        user = obj["StandardUsers"].toArray();
    }

    else if (arrKey == "Transactions")
    {
        transactions = obj["Transactions"].toArray();
    }
}

QString BaseUser::generateAccountNumber()
{
    quint32 randomNumber = QRandomGenerator::global()->bounded(1000000);
    QString accnum = QString::number(randomNumber).rightJustified(6, '0');
    arrayExtraction("StandardUser");
    QJsonObject item;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item = user[i].toObject();
        if(item.value("Accountnumber") == accnum)
        {
            return generateAccountNumber();
        }
    }
    return accnum;
}

bool BaseUser::loginCheck(QString userType, QString name, QString pass)
{
    arrayExtraction(userType);
    QJsonObject item;
    if (userType == "Admin")
    {
        for (qint32 i = 0; i < admin.size(); i++)
        {
            item = admin[i].toObject();
            if((item.value("Username") == name) && (item.value("Password") == pass.toInt()))
            {
                return true;
            }
        }
    }
    else if (userType == "StandardUser")
    {
        for (qint32 i = 0; i < user.size(); i++)
        {
            item = user[i].toObject();
            if((item.value("Username") == name) && (item.value("Password") == pass.toInt()))
            {
                return true;
            }
        }
    }
    return false;
}

QString BaseUser::getAccountNumber(QString username)
{
    arrayExtraction("StandardUser");

    QJsonObject item;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item = user[i].toObject();
        if(item.value("Username") == username)
        {
            QString accNumber = item.value("Accountnumber").toString();
            qDebug() << accNumber;
            return accNumber;
        }
    }
    return "";

}

qint32 BaseUser::viewAccountBalance(QString accountnumber)
{
    arrayExtraction("StandardUser");

    QJsonObject item;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item = user[i].toObject();
        if(item.value("Accountnumber") == accountnumber)
        {
            qint32 accBalance = item.value("Balance").toInt();
            return accBalance;
        }
    }
    return 0;

}

QJsonArray BaseUser::viewTransactionHistory(QString accountnumber, qint32 count)
{
    QJsonArray extractedTransfers;
    bool accountNumberExists;
    arrayExtraction("Transactions");
    QJsonObject item;
    for (qint32 i = 0; i < transactions.size(); i++)
    {
        item = transactions[i].toObject();
        if (item.value("Accountnumber") == accountnumber)
        {
            transactions = item.value("History").toArray();
            accountNumberExists = true;
            break;
        }
        accountNumberExists = false;
    }
    /* If account number doesn't exist return an empty array */
    if(!accountNumberExists)
        return extractedTransfers;

    // Array size check
    if (count > transactions.size())
        count = transactions.size();

    for (qint32 i = 0; i < count; i++)
    {
        extractedTransfers.append(transactions.at(i));
    }
    qDebug() << extractedTransfers;

    return extractedTransfers;
}

bool BaseUser::createNewUser(QString username,QString password,QString fullname,QString age,QString balance)
{
    arrayExtraction("StandardUser");
    arrayExtraction("Transactions");

    QJsonObject item;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item = user[i].toObject();
        if(item.value("Username") == username)
        {
            return false;
        }
    }

    QString accnum = generateAccountNumber();

    QJsonObject newItem;
    QJsonObject newtransactionArray;
    newItem["Username"] = username;
    newItem["Password"] = password.toInt();
    newItem["Fullname"] = fullname;
    newItem["Accountnumber"] = accnum;
    newItem["Age"] = age.toInt();
    newItem["Balance"] = balance.toInt();

    user.append(newItem);
    obj["StandardUsers"] = user;

    newtransactionArray["Accountnumber"] = accnum;
    newtransactionArray["History"];

    transactions.append(newtransactionArray);
    obj["Transactions"] = transactions;

    mutex.lock();
    if (!file.open(QFile::WriteOnly))
    {
        return false;
    }
    QJsonDocument new_doc(obj);
    file.write(new_doc.toJson());
    file.close();
    mutex.unlock();

    return true;
}

bool BaseUser::DeleteUser(QString accountNum)
{
    bool isUserDeleted, isHistoryDeleted;
    arrayExtraction("StandardUser");
    arrayExtraction("Transactions");

    QJsonObject item;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item = user[i].toObject();
        if(item.value("Accountnumber") == accountNum)
        {
            user.removeAt(i);
            isUserDeleted = true;
            break;
        }
        else
            isUserDeleted = false;
    }
    for (qint32 i = 0; i < transactions.size(); i++)
    {
        item = transactions[i].toObject();
        if(item.value("Accountnumber") == accountNum)
        {
            transactions.removeAt(i);
            isHistoryDeleted = true;
            break;
        }
        else
            isHistoryDeleted = false;
    }
    if(isUserDeleted && isHistoryDeleted)
    {
        obj["StandardUsers"] = user;
        obj["Transactions"] = transactions;

        mutex.lock();
        if (!file.open(QFile::WriteOnly))
        {
            return false;
        }
        QJsonDocument new_doc(obj);
        file.write(new_doc.toJson());
        file.close();
        mutex.unlock();

        return true;
    }
    return false;
}

qint32 BaseUser::UpdateUser(QString num, QString key, QString data)
{
    arrayExtraction("StandardUser");
    QJsonObject item;
    if(key == "Username")
    {
        for (qint32 i = 0; i < user.size(); i++)
        {
            item = user[i].toObject();
            if(item.value("Username") == data)
            {
                return 2;
            }
        }
    }

    for (qint32 i = 0; i < user.size(); i++)
    {
        item = user[i].toObject();
        if(item.value("Accountnumber") == num)
        {
            qDebug() << num;
            if (item.value(key).isString())
                item[key] = data;
            else
                item[key] = data.toInt();

            user.replace(i,item);

            mutex.lock();
            if (!file.open(QFile::WriteOnly))
            {
                return 0;
            }

            obj["StandardUsers"] = user;
            QJsonDocument new_doc(obj);
            file.write(new_doc.toJson());
            file.close();
            mutex.unlock();

            return 1;
        }
    }
    return 0;
}

bool BaseUser::MakeNewTransaction(QString accNum, QString code, QString amount)
{
    qint32 money;

    if (code == "Deposit")
        money = amount.toInt();
    else
        money = -(amount.toInt());

    QJsonObject item1;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item1 = user[i].toObject();
        if(item1.value("Accountnumber") == accNum)
        {
            if ((item1.value("Balance").toInt() < amount.toInt()) && (code == "Withdraw"))
            {
                return false;
            }
            else
            {
                item1["Balance"] = item1.value("Balance").toInt() + money;
                user.replace(i,item1);
                break;
            }
        }
    }

    QJsonObject item2;
    arrayExtraction("Transactions");

    for (qint32 i = 0; i < transactions.size(); i++)
    {
        item2 = transactions[i].toObject();
        if (item2.value("Accountnumber") == accNum)
        {
            QJsonArray history = item2.value("History").toArray();
            QJsonObject item;
            QDate currentDate = QDate::currentDate();
            QString dateString = currentDate.toString("yyyy-MM-dd");
            item["Date"] = dateString;
            item["Amount"] = money;
            history.insert(0,item);
            item2["History"] = history;
            transactions.replace(i,item2);
            break;
        }
    }

    mutex.lock();
    if (!file.open(QFile::WriteOnly))
    {
        return false;
    }

    obj["StandardUsers"] = user;
    obj["Transactions"] = transactions;
    QJsonDocument new_doc(obj);
    file.write(new_doc.toJson());
    file.close();
    mutex.unlock();

    return true;
}

qint32 BaseUser::TransferMoney(QString sender, QString receiver, QString amount)
{
    QJsonObject item1;
    bool isReceiverNumberValid;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item1 = user[i].toObject();
        if(item1.value("Accountnumber") == sender)
        {
            if (item1.value("Balance").toInt() < amount.toInt())
            {
                return 2;
            }
            else
            {
                item1["Balance"] = item1.value("Balance").toInt() - amount.toInt();
                user.replace(i,item1);
                break;
            }
        }
    }

    QJsonObject item3;
    for (qint32 i = 0; i < user.size(); i++)
    {
        item3 = user[i].toObject();
        if(item3.value("Accountnumber") == receiver)
        {
            item3["Balance"] = item3.value("Balance").toInt() + amount.toInt();
            user.replace(i,item3);
            isReceiverNumberValid = true;
            break;

        }
        else
            isReceiverNumberValid = false;
    }
    if(!isReceiverNumberValid)
        return 0;

    QJsonObject item2;
    arrayExtraction("Transactions");

    for (qint32 i = 0; i < transactions.size(); i++)
    {
        item2 = transactions[i].toObject();
        if (item2.value("Accountnumber") == sender)
        {
            QJsonArray history = item2.value("History").toArray();
            QJsonObject item;
            QDate currentDate = QDate::currentDate();
            QString dateString = currentDate.toString("yyyy-MM-dd");
            item["Date"] = dateString;
            item["Amount"] = -(amount.toInt());
            history.insert(0,item);
            item2["History"] = history;
            transactions.replace(i,item2);
            break;
        }
    }

    QJsonObject item4;
    for (qint32 i = 0; i < transactions.size(); i++)
    {
        item4 = transactions[i].toObject();
        if (item4.value("Accountnumber") == receiver)
        {
            QJsonArray history = item4.value("History").toArray();
            QJsonObject item;
            QDate currentDate = QDate::currentDate();
            QString dateString = currentDate.toString("yyyy-MM-dd");
            item["Date"] = dateString;
            item["Amount"] = amount.toInt();
            history.insert(0,item);
            item4["History"] = history;
            transactions.replace(i,item4);
            break;
        }
    }

    mutex.lock();
    if (!file.open(QFile::WriteOnly))
    {
        return 0;
    }

    obj["StandardUsers"] = user;
    obj["Transactions"] = transactions;
    QJsonDocument new_doc(obj);
    file.write(new_doc.toJson());
    file.close();
    mutex.unlock();

    return 1;
}

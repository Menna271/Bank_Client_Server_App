#include "adminrequests.h"
#include "ui_adminrequests.h"
#include "client.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

extern Client client;

AdminRequests::AdminRequests(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminRequests)
{
    ui->setupUi(this);

    /* Replacing default error-prone functions and connecting the new ones */
    connect(ui->accNumpb, ui->accNumpb->clicked, this, &AdminRequests::onAccNumpbClicked);
    connect(ui->accBalancepb, ui->accBalancepb->clicked, this, &AdminRequests::onAccBalancepbClicked);
    connect(ui->numenterpb, ui->numenterpb->clicked, this, &AdminRequests::onNumenterpbClicked);
    connect(ui->numtohomepb, ui->numtohomepb->clicked, this, &AdminRequests::onNumtohomepbClicked);
    connect(ui->balancenterpb, ui->balancenterpb->clicked, this, &AdminRequests::onBalancenterpbClicked);
    connect(ui->balancetohomepb, ui->balancetohomepb->clicked, this, &AdminRequests::onBalancetohomepbClicked);
    connect(ui->historypb, ui->historypb->clicked, this, &AdminRequests::onHistorypbClicked);
    connect(ui->historyenterpb, ui->historyenterpb->clicked, this, &AdminRequests::onHistoryenterpbClicked);
    connect(ui->historytohomepb, ui->historytohomepb->clicked, this, &AdminRequests::onHistorytohomepbClicked);
    connect(ui->createuserpb, ui->createuserpb->clicked, this, &AdminRequests::onCreateuserpbClicked);
    connect(ui->delUserpb, ui->delUserpb->clicked, this, &AdminRequests::onDelUserpbClicked);
    connect(ui->updateuserpb, ui->updateuserpb->clicked, this, &AdminRequests::onUpdateuserpbClicked);
    connect(ui->databasepb, ui->databasepb->clicked, this, &AdminRequests::onDatabasepbClicked);
    connect(ui->createusertohomepb, ui->createusertohomepb->clicked, this, &AdminRequests::onCreateusertohomepbClicked);
    connect(ui->deletetohomepb, ui->deletetohomepb->clicked,  this, &AdminRequests::onDeletetohomepbClicked);
    connect(ui->updatetohomepb, ui->updatetohomepb->clicked, this, &AdminRequests::onUpdatetohomepbClicked);
    connect(ui->databasetohomepb, ui->databasetohomepb->clicked, this, &AdminRequests::onDatabasetohomepbClicked);
    connect(ui->newUserpb, ui->newUserpb->clicked, this, &AdminRequests::onNewUserpbClicked);
    connect(ui->deletepb, ui->deletepb->clicked, this, &AdminRequests::onDeletepbClicked);
    connect(ui->updatepb, ui->updatepb->clicked, this, &AdminRequests::onUpdatepbClicked);
    connect(ui->homepb, ui->homepb->clicked, this, &AdminRequests::onHomepbClicked);

    /* Connecting client data received signals with UI slots */
    connect (&client, &Client::account_number_received, this, &AdminRequests::onAccountNumberReceived);
    connect (&client, &Client::account_balance_received, this, &AdminRequests::onAccountBalanceReceived);
    connect (&client, &Client::transactions_history_received, this, &AdminRequests::onTransactionsHistoryReceived);
    connect (&client, &Client::database_received, this, &AdminRequests::onDatabaseReceived);
    connect (&client, &Client::new_user_validity_received, this, &AdminRequests::onNewUserValidityReceived);
    connect (&client, &Client::account_deletion_received, this, &AdminRequests::onAccountDeletionReceived);
    connect (&client, &Client::user_update_received, this, &AdminRequests::onUserUpdateReceived);
}

AdminRequests::~AdminRequests()
{
    delete ui;
}

void AdminRequests::onAccNumpbClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void AdminRequests::onAccBalancepbClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void AdminRequests::onNumenterpbClicked()
{
    QString username = ui->usernamele->text();
    QByteArray data;
    qint32 request = 1;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << username;

    client.sendData(data);
}

void AdminRequests::onAccountNumberReceived()
{
    QString accNum = client.getAccountNumber();
    if(accNum != "")
    {
        ui->accNumle->setText(accNum);
        ui->accNumle->setAlignment(Qt::AlignCenter);
    }
    else
    {
        QMessageBox::warning(this,"Account Number","Username is not correct.");
    }
}

void AdminRequests::onNumtohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminRequests::onBalancenterpbClicked()
{
    QString acc_num = ui->accNumle_2->text();
    QByteArray data;
    qint32 request = 2;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << acc_num;

    client.sendData(data);
}

void AdminRequests::onAccountBalanceReceived()
{
    qint32 accBalance = client.getAccountBalance();
    if(accBalance)
    {
        QString balance = QString::number(accBalance);
        ui->accBalancele->setText(balance);
        ui->accBalancele->setAlignment(Qt::AlignCenter);
    }
    else
    {
        QMessageBox::warning(this,"Account Balance","Account number is not correct.");
    }
}

void AdminRequests::onBalancetohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminRequests::onHistorypbClicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void AdminRequests::onHistoryenterpbClicked()
{
    QString acc_num = ui->accNumle_3->text();
    QByteArray data;
    qint32 count = ui->countle->text().toInt();
    qint32 request = 3;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << acc_num << count;

    client.sendData(data);
}

void AdminRequests::onTransactionsHistoryReceived()
{
    QJsonArray jsonData = client.getTransactionsHistory();
    if(!jsonData.isEmpty())
    {
        ui->historyTable->setRowCount(jsonData.size());

        QString date;
        QString amount;
        for(qint32 row = 0; row < jsonData.size(); row++)
        {
            QJsonObject obj = jsonData[row].toObject();
            date = obj.value("Date").toString();
            amount = QString::number(obj.value("Amount").toInt());
            qDebug() << amount;
            ui->historyTable->setItem(row, 0, new QTableWidgetItem(date));
            ui->historyTable->setItem(row, 1, new QTableWidgetItem(amount));
        }
    }
    else
    {
        QMessageBox::warning(this,"Transaction History","Account number is not correct.");
    }
}

void AdminRequests::onHistorytohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminRequests::onDatabasepbClicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    QByteArray data;
    qint32 request = 4;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request;

    client.sendData(data);
}

void AdminRequests::onDatabaseReceived()
{
    QJsonArray admins = client.getAdminData();
    QJsonArray clients = client.getUserData();
    QJsonArray history = client.getTransactionsHistory();
    ui->adminTable->setRowCount(admins.size());
    ui->clientTable->setRowCount(clients.size());

    QString username, password, fullName, age, accNum, balance, date, amount;
    for(qint32 row = 0; row < clients.size(); row++)
    {
        QJsonObject obj = clients[row].toObject();
        username = obj.value("Username").toString();
        fullName = obj.value("Fullname").toString();
        accNum = obj.value("Accountnumber").toString();
        password = QString::number(obj.value("Password").toInt());
        age = QString::number(obj.value("Age").toInt());
        balance = QString::number(obj.value("Balance").toInt());

        ui->clientTable->setItem(row, 0, new QTableWidgetItem(username));
        ui->clientTable->setItem(row, 1, new QTableWidgetItem(password));
        ui->clientTable->setItem(row, 2, new QTableWidgetItem(fullName));
        ui->clientTable->setItem(row, 3, new QTableWidgetItem(age));
        ui->clientTable->setItem(row, 4, new QTableWidgetItem(accNum));
        ui->clientTable->setItem(row, 5, new QTableWidgetItem(balance));
    }

    for(qint32 row = 0; row < admins.size(); row++)
    {
        QJsonObject obj = admins[row].toObject();
        username = obj.value("Username").toString();
        password = QString::number(obj.value("Password").toInt());

        ui->adminTable->setItem(row, 0, new QTableWidgetItem(username));
        ui->adminTable->setItem(row, 1, new QTableWidgetItem(password));
    }

    qint32 i = 0;
    for(qint32 row = 0; row < history.size(); row++)
    {
        ui->transactionsTable->insertRow(i);
        QJsonObject obj = history[row].toObject();
        accNum = obj.value("Accountnumber").toString();

        QJsonArray transaction = obj.value("History").toArray();
        for (qint32 j = 0; j < transaction.size(); j++)
        {
            ui->transactionsTable->insertRow(i);
            QJsonObject item = transaction[j].toObject();
            date = item.value("Date").toString();
            amount = QString::number(item.value("Amount").toInt());
            qDebug() << amount;
            ui->transactionsTable->setItem(i, 0, new QTableWidgetItem(accNum));
            ui->transactionsTable->setItem(i, 1, new QTableWidgetItem(date));
            ui->transactionsTable->setItem(i, 2, new QTableWidgetItem(amount));
            i++;
        }
        i++;
    }
}

void AdminRequests::onDatabasetohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminRequests::onHomepbClicked()
{
    emit AdmintoHomePushButtonClicked();
}

void AdminRequests::onNewUserpbClicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void AdminRequests::onCreateuserpbClicked()
{
    QString new_username = ui->cusernamele->text();
    QString new_password = ui->cpasswordle->text();
    if(new_password.length() != 4)
    {
        QMessageBox::warning(this,"New User","Password must be 4 digits.");
    }
    else
    {
        QString new_fullname = ui->cfullnamele->text();
        QString new_age = ui->cagele->text();
        QString new_balance = ui->cbalancele->text();

        QByteArray data;
        qint32 request = 5;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_6_6);
        stream << request << new_username << new_password << new_fullname
               << new_age << new_balance;

        client.sendData(data);
    }
}

void AdminRequests::onNewUserValidityReceived()
{
    bool response = client.getNewUserValidity();
    if(response)
        QMessageBox::information(this,"New User","New user was created successfully.");
    else
        QMessageBox::warning(this,"New User","Username is already taken. Try again.");
}

void AdminRequests::onCreateusertohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminRequests::onDelUserpbClicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void AdminRequests::onDeletepbClicked()
{
    QString acc_num = ui->accNumtodeletele->text();
    QByteArray data;
    qint32 request = 6;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << acc_num;

    client.sendData(data);
}

void AdminRequests::onAccountDeletionReceived()
{
    bool response = client.getAccountDeletedConfirmation();
    if(response)
        QMessageBox::information(this,"Delete Account","Account was deleted successfully.");
    else
        QMessageBox::warning(this,"Delete Account","Account number doesn't exist. Try again.");
}

void AdminRequests::onDeletetohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminRequests::onUpdateuserpbClicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void AdminRequests::onUpdatepbClicked()
{
    QString accNum = ui->updateaccnumle->text();
    QString dataItem = ui->comboBox->currentText();
    QString newData = ui->updatedatale->text();
    qint32 request = 7;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    if(dataItem == "Password")
    {
        if(newData.length() != 4)
        {
            QMessageBox::warning(this,"Update User","Password must be 4 digits.");
        }
        else
        {
            stream << request << accNum << dataItem << newData;
            client.sendData(data);
        }
    }
    else
    {
        stream << request << accNum << dataItem << newData;
        client.sendData(data);
    }
}

void AdminRequests::onUserUpdateReceived()
{
    qint32 response = client.getUserUpdateConfirmation();
    if(response == 0)
        QMessageBox::warning(this,"Update User","Account number doesn't exist. Try again.");
    else if(response == 1)
        QMessageBox::information(this,"Update User","User updated successfully.");
    else if(response == 2)
        QMessageBox::warning(this,"Update User","Username already exists. Try again");
}

void AdminRequests::onUpdatetohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


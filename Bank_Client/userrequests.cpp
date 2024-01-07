#include "userrequests.h"
#include "ui_userrequests.h"
#include "client.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

extern Client client;

UserRequests::UserRequests(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserRequests)
{
    ui->setupUi(this);

    /* Replacing default error-prone functions and connecting the new ones */
    connect(ui->accNumpb, ui->accNumpb->clicked, this, &UserRequests::onAccNumpbClicked);
    connect(ui->accBalancepb, ui->accBalancepb->clicked, this, &UserRequests::onAccBalancepbClicked);
    connect(ui->historypb, ui->historypb->clicked, this, &UserRequests::onHistorypbClicked);
    connect(ui->newTransactionpb, ui->newTransactionpb->clicked, this, &UserRequests::onNewTransactionpbClicked);
    connect(ui->moneyTransferpb, ui->moneyTransferpb->clicked, this, &UserRequests::onMoneyTransferpbClicked);
    connect(ui->numtohomepb, ui->numtohomepb->clicked, this, &UserRequests::onNumtohomepbClicked);
    connect(ui->balancetohomepb, ui->balancetohomepb->clicked, this, &UserRequests::onBalancetohomepbClicked);
    connect(ui->historytohomepb, ui->historytohomepb->clicked, this, &UserRequests::onHistorytohomepbClicked);
    connect(ui->newtransactiontohomepb, ui->newtransactiontohomepb->clicked, this, &UserRequests::onNewtransactiontohomepbClicked);
    connect(ui->moneytransfertohomepb, ui->moneytransfertohomepb->clicked, this, &UserRequests::onMoneytransfertohomepbClicked);
    connect(ui->historyenterpb, ui->historyenterpb->clicked, this, &UserRequests::onHistoryenterpbClicked);
    connect(ui->usertransactionenterpb, ui->usertransactionenterpb->clicked, this, &UserRequests::onUsertransactionenterpbClicked);
    connect(ui->transferenterpb, ui->transferenterpb->clicked, this, &UserRequests::onTransferenterpbClicked);
    connect(ui->homepb, ui->homepb->clicked, this, &UserRequests::onHomepbClicked);

    /* Connecting client data received signals with UI slots */
    connect (&client, &Client::account_number_received, this, &UserRequests::onAccountNumberReceived);
    connect (&client, &Client::account_balance_received, this, &UserRequests::onAccountBalanceReceived);
    connect (&client, &Client::transactions_history_received, this, &UserRequests::onTransactionsHistoryReceived);
    connect (&client, &Client::new_transaction_received, this, &UserRequests::onNewTransactionReceived);
    connect (&client, &Client::money_transfer_received, this, &UserRequests::onMoneyTransferReceived);
}

UserRequests::~UserRequests()
{
    delete ui;
}

void UserRequests::onHomepbClicked()
{
    emit UsertoHomePushButtonClicked();
}

void UserRequests::onAccNumpbClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    qint32 request = 1;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request;

    client.sendData(data);
}

void UserRequests::onNumtohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserRequests::onAccBalancepbClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    QByteArray data;
    qint32 request = 2;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request;

    client.sendData(data);
}

void UserRequests::onBalancetohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserRequests::onHistorypbClicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void UserRequests::onHistorytohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserRequests::onNewTransactionpbClicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void UserRequests::onNewtransactiontohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserRequests::onMoneyTransferpbClicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void UserRequests::onMoneytransfertohomepbClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserRequests::onAccountNumberReceived()
{
    QString accNum = client.getAccountNumber();
    ui->accNumle->setText(accNum);
    ui->accNumle->setAlignment(Qt::AlignCenter);
}

void UserRequests::onAccountBalanceReceived()
{
    qint32 accBalance = client.getAccountBalance();
    QString balance = QString::number(accBalance);
    ui->accBalancele->setText(balance);
    ui->accBalancele->setAlignment(Qt::AlignCenter);
}

void UserRequests::onHistoryenterpbClicked()
{
    QByteArray data;
    qint32 count = ui->countle->text().toInt();
    qint32 request = 3;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << count;

    client.sendData(data);
}

void UserRequests::onTransactionsHistoryReceived()
{
    QJsonArray jsonData = client.getTransactionsHistory();
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

void UserRequests::onUsertransactionenterpbClicked()
{
    QString transactionKey = ui->comboBox->currentText();
    QString amount = ui->transactionamountle->text();
    qint32 request = 8;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << transactionKey << amount;

    client.sendData(data);
}

void UserRequests::onNewTransactionReceived()
{
    bool response = client.getNewTransactionStatus();
    if(response)
        QMessageBox::information(this,"New Transaction","Transaction successful.");
    else
        QMessageBox::warning(this,"New Transaction","Transaction declined. Funds not enough");
}

void UserRequests::onTransferenterpbClicked()
{
    QString accNum = ui->accnumfortransferle->text();
    QString amount = ui->transferamountle->text();
    qint32 request = 9;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_6);
    stream << request << accNum << amount;

    client.sendData(data);
}

void UserRequests::onMoneyTransferReceived()
{
    qint32 response = client.getMoneyTransferStatus();
    if(response == 1)
        QMessageBox::information(this,"New Transfer","Transfer successful.");
    else if(response == 2)
        QMessageBox::warning(this,"New Transfer","Transfer declined. Funds not enough");
    else if(response == 0)
        QMessageBox::warning(this,"New Transfer","Account number doesn't exist.");
}

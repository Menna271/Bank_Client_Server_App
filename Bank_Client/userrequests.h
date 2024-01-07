#ifndef USERREQUESTS_H
#define USERREQUESTS_H

#include <QDialog>

namespace Ui {
class UserRequests;
}

class UserRequests : public QDialog
{
    Q_OBJECT

public:
    explicit UserRequests(QWidget *parent = nullptr);
    ~UserRequests();

signals:
    void UsertoHomePushButtonClicked();

private slots:
    /* Push buttons slots */
    void onHomepbClicked();
    void onAccNumpbClicked();
    void onNumtohomepbClicked();
    void onAccBalancepbClicked();
    void onBalancetohomepbClicked();
    void onHistorypbClicked();
    void onHistorytohomepbClicked();
    void onNewTransactionpbClicked();
    void onNewtransactiontohomepbClicked();
    void onMoneyTransferpbClicked();
    void onMoneytransfertohomepbClicked();
    void onHistoryenterpbClicked();
    void onUsertransactionenterpbClicked();
    void onTransferenterpbClicked();

    /* Data received slots */
    void onAccountNumberReceived();
    void onAccountBalanceReceived();
    void onTransactionsHistoryReceived();
    void onNewTransactionReceived();
    void onMoneyTransferReceived();

private:
    Ui::UserRequests *ui;
};

#endif // USERREQUESTS_H

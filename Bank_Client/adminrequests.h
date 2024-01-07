#ifndef ADMINREQUESTS_H
#define ADMINREQUESTS_H

#include <QDialog>

namespace Ui {
class AdminRequests;
}

class AdminRequests : public QDialog
{
    Q_OBJECT

public:
    explicit AdminRequests(QWidget *parent = nullptr);
    ~AdminRequests();

signals:
    void AdmintoHomePushButtonClicked();

private slots:
    /* Push buttons slots */
    void onNumenterpbClicked();
    void onAccNumpbClicked();
    void onNumtohomepbClicked();
    void onBalancenterpbClicked();
    void onBalancetohomepbClicked();
    void onAccBalancepbClicked();
    void onHistoryenterpbClicked();
    void onHistorytohomepbClicked();
    void onHistorypbClicked();
    void onDatabasepbClicked();
    void onDatabasetohomepbClicked();
    void onHomepbClicked();
    void onNewUserpbClicked();
    void onCreateuserpbClicked();
    void onCreateusertohomepbClicked();
    void onDelUserpbClicked();
    void onDeletepbClicked();
    void onDeletetohomepbClicked();
    void onUpdateuserpbClicked();
    void onUpdatepbClicked();
    void onUpdatetohomepbClicked();

    /* Data received slots */
    void onAccountNumberReceived();
    void onAccountBalanceReceived();
    void onTransactionsHistoryReceived();
    void onDatabaseReceived();
    void onNewUserValidityReceived();
    void onAccountDeletionReceived();
    void onUserUpdateReceived();

private:
    Ui::AdminRequests *ui;
};

#endif // ADMINREQUESTS_H

#ifndef TPGWIDGET_H
#define TPGWIDGET_H

#include "abstractwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
//#include <memory>
#include "tpgcontroller.h"

class TPGWidget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit TPGWidget(std::string,QWidget *parent = 0);
//    TPGWidget(std::string name);
    ~TPGWidget();

public slots:
    void deviceConnect();
    void deviceDisconnect();

    void updateStatus(void *data);
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);
    void controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp);

    void updateStatusLabel(QString info);

protected:
    void closeEvent(QCloseEvent* e);

private:
//    std::string instanceName;
    tpg_controller *controller;
    bool connectionState;
    //create layout procedures and variables
    QVBoxLayout *mainLayout;
    QLabel *statusLabel;
    QLabel *connectionStatus;
    QLineEdit *connectionIP;
    QLineEdit *connectionPort;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    void createLayout();
    void drawLine();
    void createConnectionSection();

    void connectSignals();
};

#endif // TPGWIDGET_H

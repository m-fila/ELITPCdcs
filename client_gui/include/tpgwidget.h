#ifndef TPGWIDGET_H
#define TPGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <memory>
#include "tpgcontroller.h"

class TPGWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TPGWidget(QWidget *parent = 0);
    TPGWidget(const char *name);
    ~TPGWidget();

public slots:
    void deviceConnect();
    void deviceDisconnect();

    void updateStatus(void *data);
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);

    void updateStatusLabel(QString info);

protected:
    void closeEvent(QCloseEvent* e);

private:
    const char *instanceName;
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

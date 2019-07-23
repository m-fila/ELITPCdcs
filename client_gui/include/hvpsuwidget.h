#ifndef HVPSUWIDGET_H
#define HVPSUWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QLCDNumber>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <memory>
#include "include/hvcontroller.h"
//#include "DT1415Containers.h"
#include "kled.h"


namespace Ui {
class HVpsuWidget;
}

class HVpsuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HVpsuWidget(QWidget *parent = 0);
    ~HVpsuWidget();
    hv_controller *HVController;
public slots:
    void deviceConnect();
    void deviceDisconnect();
//    void onConnect();
//    void onDisconnect();
    void updateStatus(void *data);
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);

//    void updateMeasurements(DT1415Measurements measurements);
//    void updateChannelStatus(DT1415ChannelStatus channelStatus);

    void onPressed();
    void offPressed();
    void setVPressed();
    void changeNamePressed();
    void updateChannelStatus(QString info);
protected:
    void closeEvent(QCloseEvent* e);

private:
    Ui::HVpsuWidget *ui;
//    std::unique_ptr<HVpsuController> HVpsuControllerPtr;
    QString CHxCustomName[9];

    //create layout procedures and variables
    QGroupBox *allTabCHx[9];
    QLCDNumber *allTabCHvoltage[9];
    KLed *allTabLed[9];
    QPushButton *allTabKill[9];
    QRadioButton *allTabOn[9];
    QRadioButton *allTabOff[9];
    QLabel *allTabVset[9];
    QPushButton *allTabSetV[9];
    QLabel *allTabImon[9];
    QLabel *allTabIset[9];

    //tab CH x containers;
    QWidget *tabCHx[9];
    QLabel *tabCHxCustomName[9];
    QPushButton *tabCHxChangeName[9];

    bool isRemotePrevious;
    bool initialUpdate;
    bool connectionState;

    void loadConfig();
    void saveConfig();
    void setChannelName(int channelno);
    void setChannelsNames();
    void createAllChannelsTab();
    void createChannelTabs();
    void drawLine(QLayout *layout);
    void connectSignals();
};

#endif // HVPSUWIDGET_H

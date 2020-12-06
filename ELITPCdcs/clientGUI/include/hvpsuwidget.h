#ifndef HVPSUWIDGET_H
#define HVPSUWIDGET_H

#include "abstractwidget.h"
#include "dt1415etcodes.h"
#include "dt1470etcodes.h"
#include "hvcontroller.h"
#include "kled.h"
#include "n1471codes.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include "ui_hvpsuwidget.h"
#include <QGroupBox>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <map>
#include <vector>

namespace Ui {
class HVpsuWidget;
}

class HVpsuWidget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit HVpsuWidget(std::string name, int channelsNumber = 9, QWidget *parent = 0);
    ~HVpsuWidget();
    Ui::HVpsuWidget *ui;
  public slots:

    void updateStatus(void *data) override;
    void updateMeasurements(void *data);
    virtual void updateConfiguration(void *data);
    //   void controllerInit(UA_Client *, UA_ClientConfig *,
    //                       UA_CreateSubscriptionResponse) override;

    void allOnPressed();
    void allOffPressed();
    void onPressed();
    void offPressed();
    void setVPressed();
    void setVMAXPressed();
    void setCurrentPressed();
    void setRUPPressed();
    void setRDWNPressed();
    void changeNamePressed();
    void updateStatusLabel(QString info);

  protected:
    const int channelsNumber;
    std::vector<QString> CHxCustomName = std::vector<QString>(channelsNumber + 1);
    // create layout procedures and variables
    std::vector<QGroupBox *> allTabCHx = std::vector<QGroupBox *>(channelsNumber + 1);
    std::vector<QLCDNumber *> allTabCHvoltage =
        std::vector<QLCDNumber *>(channelsNumber + 1);
    ;
    std::vector<KLed *> allTabLed = std::vector<KLed *>(channelsNumber + 1);
    std::vector<QPushButton *> allTabKill =
        std::vector<QPushButton *>(channelsNumber + 1);
    std::vector<QLabel *> allTabVset = std::vector<QLabel *>(channelsNumber + 1);
    std::vector<QPushButton *> allTabSetV =
        std::vector<QPushButton *>(channelsNumber + 1);
    std::vector<QLabel *> allTabImon = std::vector<QLabel *>(channelsNumber + 1);
    std::vector<QLabel *> allTabIset = std::vector<QLabel *>(channelsNumber + 1);
    std::vector<QRadioButton *> allTabOn = std::vector<QRadioButton *>(channelsNumber);
    std::vector<QRadioButton *> allTabOff = std::vector<QRadioButton *>(channelsNumber);
    QPushButton *allOn;
    QPushButton *allOff;
    // tab CH x containers;
    std::vector<QWidget *> tabCHx = std::vector<QWidget *>(channelsNumber + 1);
    std::vector<QLabel *> tabCHxCustomName = std::vector<QLabel *>(channelsNumber + 1);
    std::vector<QPushButton *> tabCHxChangeName =
        std::vector<QPushButton *>(channelsNumber + 1);
    std::vector<QLabel *> tabCHxSTATUS = std::vector<QLabel *>(channelsNumber);
    std::vector<QPushButton *> tabCHxSetVMAX = std::vector<QPushButton *>(channelsNumber);
    std::vector<QLabel *> tabCHxVMAX = std::vector<QLabel *>(channelsNumber);
    std::vector<QPushButton *> tabCHxSetIset = std::vector<QPushButton *>(channelsNumber);
    std::vector<QLabel *> tabCHxIset = std::vector<QLabel *>(channelsNumber);
    std::vector<QPushButton *> tabCHxSetRUP = std::vector<QPushButton *>(channelsNumber);
    std::vector<QLabel *> tabCHxRUP = std::vector<QLabel *>(channelsNumber);
    std::vector<QPushButton *> tabCHxSetRDWN = std::vector<QPushButton *>(channelsNumber);
    std::vector<QLabel *> tabCHxRDWN = std::vector<QLabel *>(channelsNumber);
    // tab CH x measurements panel
    std::vector<QLCDNumber *> tabCHxvoltage = std::vector<QLCDNumber *>(channelsNumber);
    std::vector<KLed *> tabCHxLed = std::vector<KLed *>(channelsNumber);
    std::vector<QPushButton *> tabCHxKill = std::vector<QPushButton *>(channelsNumber);
    std::vector<QRadioButton *> tabCHxOn = std::vector<QRadioButton *>(channelsNumber);
    std::vector<QRadioButton *> tabCHxOff = std::vector<QRadioButton *>(channelsNumber);
    std::vector<QLabel *> tabCHxVset = std::vector<QLabel *>(channelsNumber);
    std::vector<QPushButton *> tabCHxSetV = std::vector<QPushButton *>(channelsNumber);
    std::vector<QLabel *> tabCHxImon = std::vector<QLabel *>(channelsNumber);

    bool isRemote;
    //  bool initialUpdate;
    bool connectionState;
    std::vector<int> enabled = std::vector<int>(channelsNumber, false);
    std::vector<int> ON = std::vector<int>(channelsNumber, false);
    void loadConfig();
    void saveConfig();
    void setChannelName(int channelno);
    void setChannelsNames();
    void createAllChannelsTab();
    void createChannelTabs();
    void drawLine(QLayout *layout);
    void connectSignals();
};

class DT1415Widget : public HVpsuWidget {
    Q_OBJECT
  public:
    DT1415Widget(std::string name, int enabledChannels = 8, QWidget *parent = 0);
};

class DT1470Widget : public HVpsuWidget {
    Q_OBJECT
  public:
    DT1470Widget(std::string name, int enabledChannels = 4, QWidget *parent = 0);
    void updateConfiguration(void *data) final;
};

class N1471Widget : public HVpsuWidget {
    Q_OBJECT
  public:
    N1471Widget(std::string name, int enabledChannels = 4, QWidget *parent = 0);
    void updateConfiguration(void *data) final;
};

#endif  // HVPSUWIDGET_H

#ifndef TPGWIDGET_H
#define TPGWIDGET_H

#include "DCSRelayWidget.h"
#include "abstractwidget.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include "tpg362codes.h"
#include "tpgcontroller.h"
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <vector>
class TPGWidget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit TPGWidget(std::string name, QWidget *parent = 0);
    ~TPGWidget();

  public slots:

    void updateStatus(void *data) override;
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);
    void updateRelay(void *data);

    void changeNamePressed();
    void changeRelay(int nr, RelayStruct values);

  private:
    std::vector<DCSRelayWidget *> relayWidgets;

    bool connectionState;
    // create layout procedures and variables
    QVBoxLayout *mainLayout;
    QLabel *connectionStatus;
    QLineEdit *connectionIP;
    QLineEdit *connectionPort;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    QTabWidget *tab;
    QGroupBox *mBox[2];
    QLCDNumber *mVacuum[2];
    QLabel *mStatusLabel[2];
    QLabel *mStatus[2];
    QPushButton *cNameButton[2];
    QLabel *cNameLabel[2];
    QString cCustomName[2];

    void createLayout();
    void drawLine();
    void createMTab();
    void createCTab();
    void createHTab();
    void createRTab();
    void setChannelName(int channelno);
    void setChannelsNames();
    void connectSignals();
    void loadConfig();
    void saveConfig();
};

#endif  // TPGWIDGET_H

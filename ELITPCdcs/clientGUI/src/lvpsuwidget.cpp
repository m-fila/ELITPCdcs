#include "lvpsuwidget.h"
#include "ui_lvpsuwidget.h"
#include <QInputDialog>
#include <QSettings>

LVpsuWidget::LVpsuWidget(std::string name, QWidget *parent)
    : AbstractWidget(new lv_controller(name), name, parent), ui(new Ui::LVpsuWidget) {
    ui->setupUi(this);
    ui->tcpLayout->addWidget(tcp);
    profile = new DCSProfileWidget(this);
    ui->profileLayout->addWidget(profile);
    loadConfig();
    setChannelsNames();
    connectSignals();
}

LVpsuWidget::~LVpsuWidget() { delete ui; }

void LVpsuWidget::connectSignals() {
    AbstractWidget::connectSignals();

    connect(ui->CH1on, SIGNAL(clicked(bool)), this, SLOT(setCH1ON()));
    connect(ui->CH2on, SIGNAL(clicked(bool)), this, SLOT(setCH2ON()));
    connect(ui->CH1off, SIGNAL(clicked(bool)), this, SLOT(setCH1OFF()));
    connect(ui->CH2off, SIGNAL(clicked(bool)), this, SLOT(setCH2OFF()));
    connect(ui->outputON, SIGNAL(clicked(bool)), this, SLOT(setOutputON()));
    connect(ui->outputOFF, SIGNAL(clicked(bool)), this, SLOT(setOutputOFF()));

    connect(ui->setName1, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName2, SIGNAL(pressed()), this, SLOT(changeNamePressed()));

    connect(ui->CH1confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH2confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH1confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));
    connect(ui->CH2confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));

    connect(profile, SIGNAL(applyProfile()), dynamic_cast<lv_controller *>(controller),
            SLOT(callApplyProfile()));
    connect(profile, SIGNAL(saveProfile(std::string)),
            dynamic_cast<lv_controller *>(controller),
            SLOT(callSaveProfile(std::string)));
    connect(profile, SIGNAL(setProfile(std::string)),
            dynamic_cast<lv_controller *>(controller), SLOT(callSetProfile(std::string)));
    connect(dynamic_cast<lv_controller *>(controller),
            SIGNAL(selectedProfileChanged(void *)), profile,
            SLOT(updateSelectedProfile(void *)));
    connect(dynamic_cast<lv_controller *>(controller),
            SIGNAL(enabledProfilesChanged(void *)), profile,
            SLOT(updateEnabledProfiles(void *)));
}

// void LVpsuWidget::controllerInit(UA_Client *client, UA_ClientConfig *config,
//                                 UA_CreateSubscriptionResponse resp) {
//    dynamic_cast<lv_controller *>(controller)->opcInit(client, config, resp);
//}

void LVpsuWidget::updateStatus(void *data) {
    AbstractWidget::updateStatus(data);
    bool isConnected = *static_cast<bool *>(data);
    connectionState = isConnected;
    if(isConnected) {
        ui->CH1on->setEnabled(!deviceSettings.CH1);
        ui->CH1off->setEnabled(deviceSettings.CH1);
        ui->CH2on->setEnabled(!deviceSettings.CH2);
        ui->CH2off->setEnabled(deviceSettings.CH2);
        ui->outputON->setEnabled(!deviceSettings.Output);
        ui->outputOFF->setEnabled(deviceSettings.Output);
        ui->CH1confVSet->setEnabled(true);
        ui->CH2confVSet->setEnabled(true);
        ui->CH1confISet->setEnabled(true);
        ui->CH2confISet->setEnabled(true);
    } else {

        ui->CH1on->setEnabled(false);
        ui->CH1off->setEnabled(false);
        ui->CH2on->setEnabled(false);
        ui->CH2off->setEnabled(false);
        ui->outputON->setEnabled(false);
        ui->outputOFF->setEnabled(false);

        ui->CH1voltage->display(0);
        ui->CH1current->display(0);
        ui->CH2voltage->display(0);
        ui->CH2current->display(0);

        ui->CH1confVSet->setEnabled(false);
        ui->CH2confVSet->setEnabled(false);
        ui->CH1confISet->setEnabled(false);
        ui->CH2confISet->setEnabled(false);
        ui->CH1confV->display(0);
        ui->CH1confI->display(0);
        ui->CH2confV->display(0);
        ui->CH2confI->display(0);
    }
}

void LVpsuWidget::updateMeasurements(void *data) {
    UA_HMPm measurements = *static_cast<UA_HMPm *>(data);
    if(measurements.voltageSize) {
        QString val;
        val.sprintf("%.3f", measurements.voltage[0]);
        ui->CH1voltage->display(val);
        val.sprintf("%.4f", measurements.current[0]);
        ui->CH1current->display(val);
        ui->CH1on->setEnabled(!measurements.ch[0] && connectionState);
        ui->CH1off->setEnabled(measurements.ch[0] && connectionState);
        val.sprintf("%.3f", measurements.voltage[1]);
        ui->CH2voltage->display(val);
        val.sprintf("%.4f", measurements.current[1]);
        ui->CH2current->display(val);
        ui->CH2on->setEnabled(!measurements.ch[1] && connectionState);
        ui->CH2off->setEnabled(measurements.ch[1] && connectionState);

        ui->outputON->setEnabled(!measurements.output && connectionState);
        ui->outputOFF->setEnabled(measurements.output && connectionState);

        deviceSettings.CH1 = measurements.ch[0];
        deviceSettings.CH2 = measurements.ch[1];
        deviceSettings.Output = measurements.output;
    }
}
void LVpsuWidget::updateConfiguration(void *data) {
    UA_HMPc configuration = *static_cast<UA_HMPc *>(data);
    if(configuration.voltageSetSize) {
        QString val;
        val.sprintf("%.2f", configuration.voltageSet[0]);
        ui->CH1volatgeSet->display(val);
        ui->CH1confV->display(val);
        val.sprintf("%.2f", configuration.currentSet[0]);
        ui->CH1currentSet->display(val);
        ui->CH1confI->display(val);
        val.sprintf("%.2f", configuration.voltageSet[1]);
        ui->CH2voltageSet->display(val);
        ui->CH2confV->display(val);
        val.sprintf("%.2f", configuration.currentSet[1]);
        ui->CH2currentSet->display(val);
        ui->CH2confI->display(val);
    }
}

void LVpsuWidget::setCH1ON() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(1, true);
}
void LVpsuWidget::setCH2ON() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(2, true);
}
void LVpsuWidget::setCH1OFF() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(1, false);
}
void LVpsuWidget::setCH2OFF() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(2, false);
}
void LVpsuWidget::setOutputON() {
    dynamic_cast<lv_controller *>(controller)->callSetOutput(true);
}
void LVpsuWidget::setOutputOFF() {
    dynamic_cast<lv_controller *>(controller)->callSetOutput(false);
}

void LVpsuWidget::loadConfig() {
    AbstractWidget::loadConfig();
    QString configkey;
    for(int i = 0; i != 2; ++i) {
        configkey.sprintf("%s/CustomName%i", instanceName.c_str(), i);
        customName[i] = QSettings().value(configkey).toString();
    }
}

void LVpsuWidget::saveConfig() {
    AbstractWidget::saveConfig();
    QString configkey;
    for(int i = 0; i != 2; ++i) {
        configkey.sprintf("%s/CustomName%i", instanceName.c_str(), i);
        QSettings().setValue(configkey, customName[i]);
    }
}

void LVpsuWidget::setChannelName(int channelno) {
    // QString title= tr("CH %1        ").arg(channelno+1);
    // title.append(customName[channelno]);
    QString title = customName[channelno];

    // set name on ALL Channles Tab

    QString label;
    // set name on CH x tab (... if empty)
    if(customName[channelno].isEmpty())
        label = QString("...");
    else
        label = customName[channelno];
    switch(channelno) {
    case 0:
        ui->groupBox1->setTitle(title);
        ui->customNameLabel1->setText(label);
        break;
    case 1:
        ui->groupBox2->setTitle(title);
        ui->customNameLabel2->setText(label);
        break;
    default:
        break;
    }
}
void LVpsuWidget::setChannelsNames() {
    for(int i = 0; i != 2; ++i) {
        setChannelName(i);
    }
}

void LVpsuWidget::changeNamePressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    if(ui->setName1 == obj) {
        i = 0;
    } else if(ui->setName2 == obj) {
        i = 1;
    }
    QString newName = QInputDialog::getText(this, tr("Set CH %1 name").arg(i + 1),
                                            tr("CH %1 name:").arg(i + 1),
                                            QLineEdit::Normal, customName[i], &ok);
    if(ok) {
        customName[i] = newName;
        setChannelName(i);
    }
}

void LVpsuWidget::setVPressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    double val;
    if(ui->CH1confVSet == obj) {
        i = 0;
        val = ui->CH1volatgeSet->value();
    } else if(ui->CH2confVSet == obj) {
        i = 1;
        val = ui->CH2voltageSet->value();
    }
    QString label;
    if(customName[i].isEmpty())
        label = tr("CH %1  [Volts]:").arg(i + 1);
    else
        label = tr("CH %1  \"%2\"  [Volts]:").arg(i + 1).arg(customName[i]);
    double d = QInputDialog::getDouble(this, tr("Set CH %1 V").arg(i + 1), label, val, 0,
                                       20, 1, &ok);
    if(ok) {
        dynamic_cast<lv_controller *>(controller)->callSetVoltage(i + 1, d);
    }
}

void LVpsuWidget::setIPressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    double val;
    if(ui->CH1confISet == obj) {
        i = 0;
        val = ui->CH1currentSet->value();
    } else if(ui->CH2confISet == obj) {
        i = 1;
        val = ui->CH2currentSet->value();
    }
    QString label;
    if(customName[i].isEmpty())
        label = tr("CH %1  [Ampers]:").arg(i + 1);
    else
        label = tr("CH %1  \"%2\"  [Ampers]:").arg(i + 1).arg(customName[i]);
    double d = QInputDialog::getDouble(this, tr("Set CH %1 I").arg(i + 1), label, val, 0,
                                       10, 1, &ok);
    if(ok) {
        dynamic_cast<lv_controller *>(controller)->callSetCurrent(i + 1, d);
    }
}

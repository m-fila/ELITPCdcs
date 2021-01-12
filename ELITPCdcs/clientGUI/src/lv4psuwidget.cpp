#include "lv4psuwidget.h"
#include "ui_lv4psuwidget.h"
//#include "src/ConnectionParameters.h"
#include <QInputDialog>
#include <QSettings>
#include <iostream>

LV4psuWidget::LV4psuWidget(std::string name, QWidget *parent)
    : AbstractWidget(new lv_controller(name), name, parent), ui(new Ui::LV4psuWidget) {
    ui->setupUi(this);
    ui->tcpLayout->addWidget(tcp);
    ui->verticalLayout_16->addWidget(&deviceInfoLabel);
    loadConfig();
    setChannelsNames();
    connectSignals();
}

LV4psuWidget::~LV4psuWidget() { delete ui; }

void LV4psuWidget::connectSignals() {
    AbstractWidget::connectSignals();

    connect(ui->CH1on, SIGNAL(clicked(bool)), this, SLOT(setCH1ON()));
    connect(ui->CH2on, SIGNAL(clicked(bool)), this, SLOT(setCH2ON()));
    connect(ui->CH3on, SIGNAL(clicked(bool)), this, SLOT(setCH3ON()));
    connect(ui->CH4on, SIGNAL(clicked(bool)), this, SLOT(setCH4ON()));
    connect(ui->CH1off, SIGNAL(clicked(bool)), this, SLOT(setCH1OFF()));
    connect(ui->CH2off, SIGNAL(clicked(bool)), this, SLOT(setCH2OFF()));
    connect(ui->CH3off, SIGNAL(clicked(bool)), this, SLOT(setCH3OFF()));
    connect(ui->CH4off, SIGNAL(clicked(bool)), this, SLOT(setCH4OFF()));
    connect(ui->outputON, SIGNAL(clicked(bool)), this, SLOT(setOutputON()));
    connect(ui->outputOFF, SIGNAL(clicked(bool)), this, SLOT(setOutputOFF()));

    connect(ui->setName1, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName2, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName3, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName4, SIGNAL(pressed()), this, SLOT(changeNamePressed()));

    connect(ui->CH1confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH2confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH3confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH4confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH1confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));
    connect(ui->CH2confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));
    connect(ui->CH3confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));
    connect(ui->CH4confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));
}

void LV4psuWidget::updateStatus(void *data) {
    AbstractWidget::updateStatus(data);
    bool isConnected = *static_cast<bool *>(data);
    connectionState = isConnected;
    if(isConnected) {
        ui->CH1on->setEnabled(!deviceSettings.CH1);
        ui->CH1off->setEnabled(deviceSettings.CH1);
        ui->CH2on->setEnabled(!deviceSettings.CH2);
        ui->CH2off->setEnabled(deviceSettings.CH2);
        ui->CH3on->setEnabled(!deviceSettings.CH3);
        ui->CH3off->setEnabled(deviceSettings.CH3);
        ui->CH4on->setEnabled(!deviceSettings.CH4);
        ui->CH4off->setEnabled(deviceSettings.CH4);

        ui->outputON->setEnabled(!deviceSettings.Output);
        ui->outputOFF->setEnabled(deviceSettings.Output);

        ui->CH1confVSet->setEnabled(true);
        ui->CH2confVSet->setEnabled(true);
        ui->CH3confVSet->setEnabled(true);
        ui->CH4confVSet->setEnabled(true);
        ui->CH1confISet->setEnabled(true);
        ui->CH2confISet->setEnabled(true);
        ui->CH3confISet->setEnabled(true);
        ui->CH4confISet->setEnabled(true);
    } else {
        ui->CH1on->setEnabled(false);
        ui->CH1off->setEnabled(false);
        ui->CH2on->setEnabled(false);
        ui->CH2off->setEnabled(false);
        ui->CH3on->setEnabled(false);
        ui->CH3off->setEnabled(false);
        ui->CH4on->setEnabled(false);
        ui->CH4off->setEnabled(false);
        ui->outputON->setEnabled(false);
        ui->outputOFF->setEnabled(false);

        ui->CH1voltage->display(0);
        ui->CH1current->display(0);
        ui->CH2voltage->display(0);
        ui->CH2current->display(0);
        ui->CH3voltage->display(0);
        ui->CH3current->display(0);
        ui->CH4voltage->display(0);
        ui->CH4current->display(0);

        ui->CH1confVSet->setEnabled(false);
        ui->CH2confVSet->setEnabled(false);
        ui->CH3confVSet->setEnabled(false);
        ui->CH4confVSet->setEnabled(false);
        ui->CH1confISet->setEnabled(false);
        ui->CH2confISet->setEnabled(false);
        ui->CH3confISet->setEnabled(false);
        ui->CH4confISet->setEnabled(false);
        ui->CH1confV->display(0);
        ui->CH1confI->display(0);
        ui->CH2confV->display(0);
        ui->CH2confI->display(0);
        ui->CH3confV->display(0);
        ui->CH3confI->display(0);
        ui->CH4confV->display(0);
        ui->CH4confI->display(0);
    }
}

void LV4psuWidget::updateMeasurements(void *data) {
    UA_HMPm measurements = *static_cast<UA_HMPm *>(data);
    if(measurements.voltageSize) {
        ui->CH1voltage->display(measurements.voltage[0]);
        ui->CH1current->display(measurements.current[0]);
        ui->CH1on->setEnabled(!measurements.ch[0] && connectionState);
        ui->CH1off->setEnabled(measurements.ch[0] && connectionState);
        ui->CH2voltage->display(measurements.voltage[1]);
        ui->CH2current->display(measurements.current[1]);
        ui->CH2on->setEnabled(!measurements.ch[1] && connectionState);
        ui->CH2off->setEnabled(measurements.ch[1] && connectionState);
        ui->CH3voltage->display(measurements.voltage[2]);
        ui->CH3current->display(measurements.current[2]);
        ui->CH3on->setEnabled(!measurements.ch[2] && connectionState);
        ui->CH3off->setEnabled(measurements.ch[2] && connectionState);
        ui->CH4voltage->display(measurements.voltage[3]);
        ui->CH4current->display(measurements.current[3]);
        ui->CH4on->setEnabled(!measurements.ch[3] && connectionState);
        ui->CH4off->setEnabled(measurements.ch[3] && connectionState);
        ui->outputON->setEnabled(!measurements.output && connectionState);
        ui->outputOFF->setEnabled(measurements.output && connectionState);
        deviceSettings.CH1 = measurements.ch[0];
        deviceSettings.CH2 = measurements.ch[1];
        deviceSettings.CH3 = measurements.ch[2];
        deviceSettings.CH4 = measurements.ch[3];
        deviceSettings.Output = measurements.output;
    }
}
void LV4psuWidget::updateConfiguration(void *data) {
    UA_HMPc configuration = *static_cast<UA_HMPc *>(data);
    if(configuration.voltageSetSize) {
        ui->CH1voltageSet->display(configuration.voltageSet[0]);
        ui->CH1currentSet->display(configuration.currentSet[0]);
        ui->CH2voltageSet->display(configuration.voltageSet[1]);
        ui->CH2currentSet->display(configuration.currentSet[1]);
        ui->CH3voltageSet->display(configuration.voltageSet[2]);
        ui->CH3currentSet->display(configuration.currentSet[2]);
        ui->CH4voltageSet->display(configuration.voltageSet[3]);
        ui->CH4currentSet->display(configuration.currentSet[3]);
        ui->CH1confV->display(configuration.voltageSet[0]);
        ui->CH1confI->display(configuration.currentSet[0]);
        ui->CH2confV->display(configuration.voltageSet[1]);
        ui->CH2confI->display(configuration.currentSet[1]);
        ui->CH3confV->display(configuration.voltageSet[2]);
        ui->CH3confI->display(configuration.currentSet[2]);
        ui->CH4confV->display(configuration.voltageSet[3]);
        ui->CH4confI->display(configuration.currentSet[3]);
    }
}

void LV4psuWidget::setCH1ON() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(1, true);
}
void LV4psuWidget::setCH2ON() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(2, true);
}
void LV4psuWidget::setCH3ON() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(3, true);
}
void LV4psuWidget::setCH4ON() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(4, true);
}
void LV4psuWidget::setCH1OFF() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(1, false);
}
void LV4psuWidget::setCH2OFF() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(2, false);
}
void LV4psuWidget::setCH3OFF() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(3, false);
}
void LV4psuWidget::setCH4OFF() {
    dynamic_cast<lv_controller *>(controller)->callSetChannel(4, false);
}
void LV4psuWidget::setOutputON() {
    dynamic_cast<lv_controller *>(controller)->callSetOutput(true);
}
void LV4psuWidget::setOutputOFF() {
    dynamic_cast<lv_controller *>(controller)->callSetOutput(false);
}

void LV4psuWidget::loadConfig() {
    AbstractWidget::loadConfig();

    QString configkey;
    for(int i = 0; i != 4; ++i) {
        configkey.sprintf("%s/CustomName%i", instanceName.c_str(), i);
        customName[i] = QSettings().value(configkey).toString();
    }
}

void LV4psuWidget::saveConfig() {
    AbstractWidget::saveConfig();

    QString configkey;
    for(int i = 0; i != 4; ++i) {
        configkey.sprintf("%s/CustomName%i", instanceName.c_str(), i);
        QSettings().setValue(configkey, customName[i]);
    }
}

void LV4psuWidget::setChannelName(int channelno) {
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
    case 2:
        ui->groupBox3->setTitle(title);
        ui->customNameLabel3->setText(label);
        break;
    case 3:
        ui->groupBox4->setTitle(title);
        ui->customNameLabel4->setText(label);
        break;
    default:
        break;
    }
}
void LV4psuWidget::setChannelsNames() {
    for(int i = 0; i < 4; i++) {
        setChannelName(i);
    }
}
void LV4psuWidget::changeNamePressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    if(ui->setName1 == obj) {
        i = 0;
    } else if(ui->setName2 == obj) {
        i = 1;
    } else if(ui->setName3 == obj) {
        i = 2;
    } else if(ui->setName4 == obj) {
        i = 3;
    }
    QString newName = QInputDialog::getText(this, tr("Set CH %1 name").arg(i + 1),
                                            tr("CH %1 name:").arg(i + 1),
                                            QLineEdit::Normal, customName[i], &ok);
    if(ok) {
        customName[i] = newName;
        setChannelName(i);
    }
}
void LV4psuWidget::setVPressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    double val;
    if(ui->CH1confVSet == obj) {
        i = 0;
        val = ui->CH1voltageSet->value();

    } else if(ui->CH2confVSet == obj) {
        i = 1;
        val = ui->CH2voltageSet->value();
    } else if(ui->CH3confVSet == obj) {
        i = 2;
        val = ui->CH3voltageSet->value();
    } else if(ui->CH4confVSet == obj) {
        i = 3;
        val = ui->CH4voltageSet->value();
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

void LV4psuWidget::setIPressed() {
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
    } else if(ui->CH3confISet == obj) {
        i = 2;
        val = ui->CH3currentSet->value();
    } else if(ui->CH4confISet == obj) {
        i = 3;
        val = ui->CH4currentSet->value();
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

#include "hvpsuwidget.h"
#include "utilsQt.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QVBoxLayout>

HVpsuWidget::HVpsuWidget(std::string name, int channelsNumber, QWidget *parent)
    : AbstractWidget(new hv_controller(name), name, true, parent),
      ui(new Ui::HVpsuWidget), channelsNumber(channelsNumber) {
    ui->setupUi(this);
    ui->tcpLayout->addWidget(tcp);
    ui->verticalLayout->addWidget(&deviceInfoLabel);
    loadConfig();  // need to be called first because create functions use some
                   // settings!
    createAllChannelsTab();
    createChannelTabs();
    setChannelsNames();
    connectSignals();
}

HVpsuWidget::~HVpsuWidget() { delete ui; }

void HVpsuWidget::connectSignals() { AbstractWidget::connectSignals(); }

void HVpsuWidget::updateStatus(UA_DataValue *data) {
    AbstractWidget::updateStatus(data);
    bool isConnected = *static_cast<bool *>(data->value.data);
    connectionState = isConnected;
    if(isConnected) {
        for(int i = 0; i < channelsNumber; i++) {
            allTabOn[i]->setEnabled(enabled[i]);
            allTabOff[i]->setEnabled(enabled[i]);
            allTabOn[i]->setChecked(ON[i]);
            allTabOff[i]->setChecked(!ON[i]);
            // allTabKill[i]->setEnabled(enabled[i]);
            allTabSetV[i]->setEnabled(enabled[i]);
            allTabLed[i]->setState(static_cast<KLed::State>(ON[i]));
            allTabVset[i]->setEnabled(true);
            allTabImon[i]->setEnabled(true);

            tabCHxSetRUP[i]->setEnabled(enabled[i]);
            tabCHxSetRDWN[i]->setEnabled(enabled[i]);
            tabCHxSetVMAX[i]->setEnabled(enabled[i]);
            tabCHxSetIset[i]->setEnabled(enabled[i]);
            tabCHxSTATUS[i]->setEnabled(true);
            tabCHxRUP[i]->setEnabled(true);
            tabCHxRDWN[i]->setEnabled(true);
            tabCHxVMAX[i]->setEnabled(true);
            tabCHxIset[i]->setEnabled(true);

            tabCHxOn[i]->setEnabled(enabled[i]);
            tabCHxOff[i]->setEnabled(enabled[i]);
            tabCHxOn[i]->setChecked(ON[i]);
            tabCHxOff[i]->setChecked(!ON[i]);
            // tabCHxKill[i]->setEnabled(enabled[i]);
            tabCHxSetV[i]->setEnabled(enabled[i]);
            tabCHxLed[i]->setState(static_cast<KLed::State>(ON[i]));
            tabCHxVset[i]->setEnabled(true);
            tabCHxImon[i]->setEnabled(true);
            tabCHxSTATUS[i]->setEnabled(true);
            allTabStatus[i]->setEnabled(true);
        }
        allTabLed[channelsNumber]->setState((KLed::State)(true));
        allTabVset[channelsNumber]->setEnabled(true);
        allOn->setEnabled(isRemote);
        allOff->setEnabled(isRemote);
    } else {
        for(int i = 0; i < channelsNumber; i++) {
            allTabOn[i]->setEnabled(false);
            allTabOff[i]->setEnabled(false);
            // allTabKill[i]->setEnabled(false);
            allTabSetV[i]->setEnabled(false);
            allTabLed[i]->setState(static_cast<KLed::State>(false));
            allTabVset[i]->setEnabled(false);
            allTabImon[i]->setEnabled(false);

            tabCHxSetRUP[i]->setEnabled(false);
            tabCHxSetRDWN[i]->setEnabled(false);
            tabCHxSetVMAX[i]->setEnabled(false);
            tabCHxSetIset[i]->setEnabled(false);
            tabCHxSTATUS[i]->setEnabled(false);
            tabCHxRUP[i]->setEnabled(false);
            tabCHxRDWN[i]->setEnabled(false);
            tabCHxVMAX[i]->setEnabled(false);
            tabCHxIset[i]->setEnabled(false);

            tabCHxOn[i]->setEnabled(false);
            tabCHxOff[i]->setEnabled(false);
            tabCHxKill[i]->setEnabled(false);
            tabCHxSetV[i]->setEnabled(false);
            tabCHxLed[i]->setState(static_cast<KLed::State>(false));
            tabCHxVset[i]->setEnabled(false);
            tabCHxImon[i]->setEnabled(false);
            tabCHxSTATUS[i]->setEnabled(false);
            allTabStatus[i]->setEnabled(false);
        }
        allTabLed[channelsNumber]->setState(static_cast<KLed::State>(false));
        allTabVset[channelsNumber]->setEnabled(false);
        allOn->setEnabled(false);
        allOff->setEnabled(false);
    }
    allTabKill[channelsNumber]->setEnabled(connectionState);
}

void HVpsuWidget::updateMeasurements(UA_DataValue *data) {
    UA_DT1415m measurements = *static_cast<UA_DT1415m *>(data->value.data);
    if(measurements.voltageSize) {
        QString val;
        double totalVoltage = 0;
        for(int i = 0; i < channelsNumber; i++) {
            totalVoltage += measurements.voltage[i];
            val.sprintf("%.1lf", measurements.voltage[i]);
            allTabCHvoltage[i]->display(val);
            tabCHxvoltage[i]->display(val);
            val.sprintf("%.3lf", measurements.current[i]);
            allTabImon[i]->setText(val);
            tabCHxImon[i]->setText(val);
        }
        val.sprintf("%.1lf", totalVoltage);
        allTabCHvoltage[channelsNumber]->display(val);
    }
}
void HVpsuWidget::updateConfiguration(UA_DataValue *data) {
    auto timestamp = DCSUtils::getTimestamp(data);
    UA_DT1415c channelStatus = *static_cast<UA_DT1415c *>(data->value.data);
    if(channelStatus.statusSize) {
        // bool ON;//, enabled;
        QString val;
        double totalVoltageSet = 0;
        for(int i = 0; i < channelsNumber; ++i) {
            DT1415ETcodes::ChannelStatus chanStat =
                static_cast<DT1415ETcodes::ChannelStatus>(channelStatus.status[i]);
            enabled[i] =
                channelStatus.isRemote &&
                !static_cast<bool>(chanStat & DT1415ETcodes::ChannelStatus::ISDIS);
            ON[i] = static_cast<bool>(chanStat & DT1415ETcodes::ChannelStatus::ON);
            tabCHxLed[i]->setState(static_cast<KLed::State>(ON[i] && connectionState));
            allTabOn[i]->setEnabled(enabled[i] && connectionState);
            allTabOff[i]->setEnabled(enabled[i] && connectionState);
            // allTabKill[i]->setEnabled(enabled[i] && connectionState);
            allTabSetV[i]->setEnabled(enabled[i] && connectionState);
            allTabLed[i]->setState(static_cast<KLed::State>(ON[i] && connectionState));
            allTabOn[i]->setChecked(ON[i] && connectionState);
            allTabOff[i]->setChecked((!ON[i]) && connectionState);

            val = QString::fromStdString(DT1415ETcodes::translateChannelStatus(chanStat));
            tabCHxSTATUS[i]->setText(val);
            allTabStatus[i]->setText(val);
            auto palette = allTabStatus[i]->palette();
            if(static_cast<bool>(chanStat & DT1415ETcodes::ChannelStatus::ON)) {
                palette.setColor(QPalette::WindowText, Qt::darkGreen);
            } else if(static_cast<bool>(chanStat &
                                        (DT1415ETcodes::ChannelStatus::OVC |
                                         DT1415ETcodes::ChannelStatus::OVV |
                                         DT1415ETcodes::ChannelStatus::UNV |
                                         DT1415ETcodes::ChannelStatus::TRIP |
                                         DT1415ETcodes::ChannelStatus::OVP |
                                         DT1415ETcodes::ChannelStatus::TWN |
                                         DT1415ETcodes::ChannelStatus::OVT |
                                         DT1415ETcodes::ChannelStatus::KILL |
                                         DT1415ETcodes::ChannelStatus::INTLK |
                                         DT1415ETcodes::ChannelStatus::FAIL))) {
                palette.setColor(QPalette::WindowText, Qt::red);
                tabCHxTrace[i]->addItem(val, timestamp);
            } else {
                palette.setColor(QPalette::WindowText, Qt::black);
            }
            allTabStatus[i]->setPalette(palette);
            tabCHxSTATUS[i]->setPalette(palette);
            tabCHxSetRUP[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetRDWN[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetIset[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetVMAX[i]->setEnabled(enabled[i] && connectionState);

            tabCHxOn[i]->setEnabled(enabled[i] && connectionState);
            tabCHxOff[i]->setEnabled(enabled[i] && connectionState);
            // tabCHxKill[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetV[i]->setEnabled(enabled[i] && connectionState);

            tabCHxOn[i]->setChecked(ON[i] && connectionState);
            tabCHxOff[i]->setChecked((!ON[i]) && connectionState);

            totalVoltageSet += channelStatus.voltageSet[i];
            val.sprintf("%6.1lf", channelStatus.voltageSet[i]);
            allTabVset[i]->setText(val);
            tabCHxVset[i]->setText(val);
            val = QString::number(channelStatus.rup[i]);
            tabCHxRUP[i]->setText(val);
            val = QString::number(channelStatus.rdown[i]);
            tabCHxRDWN[i]->setText(val);
            val.sprintf("%4.1lf", channelStatus.voltageMax[i]);
            tabCHxVMAX[i]->setText(val);
            val.sprintf("%3.1lf", channelStatus.currentSet[i]);
            tabCHxIset[i]->setText(val);
            std::string std;
            if(channelStatus.polarity[i].length != 0) {
                std =
                    std::string(reinterpret_cast<char *>(channelStatus.polarity[i].data),
                                channelStatus.polarity[i].length);
            }
            tabCHxPOLARITY[i]->setText(QString::fromStdString(std));
        }
        val.sprintf("%6.1lf", totalVoltageSet);
        allTabVset[channelsNumber]->setText(val);
        allOn->setEnabled(channelStatus.isRemote && connectionState);
        allOff->setEnabled(channelStatus.isRemote && connectionState);
        // allTabKill[channelsNumber]->setEnabled(channelStatus.isRemote);
        isRemote = channelStatus.isRemote;
    }
}

void HVpsuWidget::onPressed() {
    QObject *obj = sender();
    int i;
    for(i = 0; i < channelsNumber; i++) {
        if((allTabOn[i] == obj) || (tabCHxOn[i] == obj))
            dynamic_cast<hv_controller *>(controller)->callSetChannel(i, true);
    }
}

void HVpsuWidget::offPressed() {
    QObject *obj = sender();
    for(int i = 0; i < channelsNumber; i++) {
        if((allTabOff[i] == obj) || (tabCHxOff[i] == obj))
            dynamic_cast<hv_controller *>(controller)->callSetChannel(i, false);
    }
}

void HVpsuWidget::allOnPressed() {
    dynamic_cast<hv_controller *>(controller)->callSetChannel(channelsNumber, true);
}

void HVpsuWidget::allOffPressed() {
    dynamic_cast<hv_controller *>(controller)->callSetChannel(channelsNumber, false);
}

void HVpsuWidget::setVPressed() {
    QObject *obj = sender();
    bool ok;
    for(int i = 0; i < channelsNumber; i++) {
        if((allTabSetV[i] == obj) || (tabCHxSetV[i] == obj)) {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 voltage").arg(i),
                                               label, allTabVset[i]->text().toDouble(), 0,
                                               8100, 1, &ok);
            if(ok) {
                QString val;
                val.sprintf("%6.1lf", d);
                // allTabVset[i]->setText(val);
                // TODO: check VtotalMax (software treshold). if sum > thr, drop input
                // and show message or set max mallowed and show message?
                dynamic_cast<hv_controller *>(controller)->callSetVoltage(i, d);
            }
        }
    }
}
void HVpsuWidget::setVMAXPressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    for(i = 0; i < channelsNumber; i++) {
        if(tabCHxSetVMAX[i] == obj) {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 Vmax").arg(i), label,
                                               tabCHxVMAX[i]->text().toDouble(), 0, 8100,
                                               1, &ok);
            if(ok) {
                QString val;
                val.sprintf("%6.1lf", d);
                QMessageBox msgBox;
                msgBox.setText(
                    QString::asprintf("Changing CH %i Vmax to %.1lf V.\nConfirm?", i, d));
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Save);
                if(msgBox.exec() == QMessageBox::Ok) {
                    tabCHxVMAX[i]->setText(val);
                    dynamic_cast<hv_controller *>(controller)->callSetVoltageMax(i, d);
                }
            }
        }
    }
}

void HVpsuWidget::setCurrentPressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    for(i = 0; i < channelsNumber; i++) {
        if(tabCHxSetIset[i] == obj) {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [uAmpers]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [uAmpers]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 ISet").arg(i), label,
                                               tabCHxIset[i]->text().toDouble(), 0, 1000,
                                               1, &ok);
            if(ok) {
                QString val;
                val.sprintf("%6.1lf", d);
                QMessageBox msgBox;
                msgBox.setText(QString::asprintf(
                    "Changing CH %i Iset to %.1lf uA.\nConfirm?", i, d));  // thx Magda
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Save);
                if(msgBox.exec() == QMessageBox::Ok) {
                    tabCHxIset[i]->setText(val);
                    dynamic_cast<hv_controller *>(controller)->callSetCurrent(i, d);
                }
            }
        }
    }
}

void HVpsuWidget::setRUPPressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    for(i = 0; i < channelsNumber; i++) {
        if(tabCHxSetRUP[i] == obj) {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts/s]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts/s]:").arg(i).arg(CHxCustomName[i]);

            auto d = QInputDialog::getInt(this, tr("Set CH %1 RUP").arg(i), label,
                                          tabCHxRUP[i]->text().toInt(), 0, 1000, 1, &ok);
            if(ok) {
                auto val = QString::number(d);
                tabCHxRUP[i]->setText(val);
                dynamic_cast<hv_controller *>(controller)->callSetRampUp(i, d);
            }
        }
    }
}
void HVpsuWidget::setRDWNPressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    for(i = 0; i < channelsNumber; i++) {
        if(tabCHxSetRDWN[i] == obj) {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts/s]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts/s]:").arg(i).arg(CHxCustomName[i]);

            auto d = QInputDialog::getInt(this, tr("Set CH %1 RDWN").arg(i), label,
                                          tabCHxRDWN[i]->text().toInt(), 0, 1000, 1, &ok);
            if(ok) {
                auto val = QString::number(d);
                tabCHxRDWN[i]->setText(val);
                dynamic_cast<hv_controller *>(controller)->callSetRampDown(i, d);
            }
        }
    }
}

void HVpsuWidget::changeNamePressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    for(i = 0; i < channelsNumber + 1; i++) {
        if(tabCHxChangeName[i] == obj) {
            QString newName = QInputDialog::getText(
                this, tr("Set CH %1 name").arg(i), tr("CH %1 name:").arg(i),
                QLineEdit::Normal, CHxCustomName[i], &ok);
            if(ok) {
                CHxCustomName[i] = newName;
                setChannelName(i);
            }
        }
    }
}

// create layout procedures
void HVpsuWidget::loadConfig() {
    AbstractWidget::loadConfig();

    int i;
    QString configkey;
    for(i = 0; i < channelsNumber + 1; i++) {
        configkey.sprintf("%s/CustomName%i", instanceName.c_str(), i);
        CHxCustomName[i] = QSettings().value(configkey).toString();
    }
}

void HVpsuWidget::saveConfig() {
    AbstractWidget::saveConfig();

    int i;
    QString configkey;
    for(i = 0; i < channelsNumber + 1; i++) {
        configkey.sprintf("%s/CustomName%i", instanceName.c_str(), i);
        QSettings().setValue(configkey, CHxCustomName[i]);
    }
}

void HVpsuWidget::setChannelName(int channelno) {
    QString title;
    if(channelno == channelsNumber)
        title = tr("TOTAL");
    else
        title = tr("CH %1        ").arg(channelno);

    title.append(CHxCustomName[channelno]);
    title.append("    ");
    // set name on ALL Channles Tab
    allTabCHx[channelno]->setTitle(title);
    // set name on CH x tab (... if empty)
    if(CHxCustomName[channelno].isEmpty())
        tabCHxCustomName[channelno]->setText("...");
    else
        tabCHxCustomName[channelno]->setText(CHxCustomName[channelno]);
}

void HVpsuWidget::setChannelsNames() {
    int i;
    for(i = 0; i < channelsNumber + 1; i++) {
        setChannelName(i);
    }
}

void HVpsuWidget::createAllChannelsTab() {
    int i;
    // define layout for ALL tab
    QVBoxLayout *allTabLayout = new QVBoxLayout;
    for(i = 0; i < channelsNumber + 1; i++) {
        // Generate GroupBoxes
        allTabCHx[i] = new QGroupBox();
        // allChannelsTabCHx[i]->setStyleSheet("QGroupBox {border: 1px solid
        // silver;margin-top: 6px;font: bold;} QGroupBox::title {subcontrol-origin:
        // margin;left: 7px; padding: 0px 5px 0px 5px;}");

        // define new layout for Groupbox and add customized controls
        QHBoxLayout *hbox = new QHBoxLayout();
                if(i % 2) {
            allTabCHx[i]->setStyleSheet("QGroupBox{background-color : transparent;}");
        }
        // add customized controls to GroupBox layout
        // begin LED
        allTabLed[i] = new KLed();
        allTabLed[i]->setFixedSize(20, 20);
        allTabLed[i]->off();
        allTabLed[i]->setColor(Qt::red);

        hbox->addWidget(allTabLed[i]);
        // end LED

        // Begin enable
        QHBoxLayout *hl = new QHBoxLayout();
        if(i != channelsNumber) {
            allTabOn[i] = new QRadioButton("ON");
            allTabOff[i] = new QRadioButton("OFF");
            allTabOn[i]->setLayoutDirection(Qt::RightToLeft);
            allTabOff[i]->setChecked(true);
            allTabOn[i]->setChecked(false);
            allTabOn[i]->setEnabled(false);
            allTabOff[i]->setEnabled(false);
            connect(allTabOn[i], SIGNAL(pressed()), this, SLOT(onPressed()));
            connect(allTabOff[i], SIGNAL(pressed()), this, SLOT(offPressed()));
            hl->addWidget(allTabOn[i]);
            hl->addWidget(allTabOff[i]);
        } else {
            allOn = new QPushButton("ON");
            allOff = new QPushButton("OFF");
            allOn->setFixedWidth(50);
            allOff->setFixedWidth(50);
            connect(allOn, SIGNAL(pressed()), this, SLOT(allOnPressed()));
            connect(allOff, SIGNAL(pressed()), this, SLOT(allOffPressed()));
            hl->addWidget(allOn);
            hl->addWidget(allOff);
        }
        hbox->addLayout(hl);
        // End enable

        // begin measured voltages
        allTabCHvoltage[i] = new QLCDNumber();
        allTabCHvoltage[i]->setDigitCount(6);
        allTabCHvoltage[i]->setSegmentStyle(QLCDNumber::Flat);
        allTabCHvoltage[i]->setMinimumSize(QSize(110, 36));
        QPalette palette = allTabCHvoltage[i]->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        allTabCHvoltage[i]->setPalette(palette);

        allTabCHvoltage[i]->display(0.00);

        hbox->addWidget(allTabCHvoltage[i]);
        // end measured voltagese

        hbox->addSpacing(20);

        // begin Vset
        QLabel *VsetLabel;
        VsetLabel = new QLabel("VSET [V]:");
        hbox->addWidget(VsetLabel);

        allTabVset[i] = new QLabel("");
        allTabVset[i]->setFixedWidth(80);
        QFont font = allTabVset[i]->font();
        font.setPointSize(14);
        font.setBold(true);
        allTabVset[i]->setFont(font);
        allTabVset[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QString val;
        val.sprintf("  %6.1lf", 0.0);
        allTabVset[i]->setText(val);

        hbox->addWidget(allTabVset[i]);
        // end Vset

        // begin setV
        allTabSetV[i] = new QPushButton("Set V");
        allTabSetV[i]->setFixedWidth(40);
        allTabSetV[i]->setEnabled(false);
        if(i != channelsNumber) {
            hbox->addWidget(allTabSetV[i]);
            connect(allTabSetV[i], SIGNAL(pressed()), this, SLOT(setVPressed()));
        }
        // end setV

        hbox->addSpacing(20);

        // begin IMON and ISET
        QGridLayout *Igrid = new QGridLayout();
        QLabel *ImonLabel = new QLabel("IMON [uA]: ");
        Igrid->addWidget(ImonLabel, 0, 0, 1, 1);

        allTabImon[i] = new QLabel("");
        allTabImon[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        val.sprintf("%7.3lf", 0.0);
        auto iFont = allTabImon[i]->font();
        iFont.setPointSize(14);
        allTabImon[i]->setFont(iFont);
        allTabImon[i]->setText(val);
        Igrid->addWidget(allTabImon[i], 0, 1, 1, 1);

        if(i != channelsNumber)
            hbox->addLayout(Igrid);
        // end IMON and ISET

        hbox->addSpacing(20);
        hbox->addStretch();
        // begin kill button
        allTabKill[i] = new QPushButton("KILL");
        if(i == channelsNumber) {
            connect(allTabKill[i], SIGNAL(pressed()),
                    dynamic_cast<hv_controller *>(controller), SLOT(callClearAlarm()));

            allTabKill[i]->setFixedWidth(40);
            // palette = allTabKill[i]->palette();
            // palette.setColor(QPalette::ButtonText, Qt::red);
            allTabKill[i]->setPalette(palette);
            allTabKill[i]->setEnabled(false);

            hbox->addWidget(allTabKill[i]);
            // end kill button
        } else {
            hbox->addWidget(new QLabel("Status: "));
            allTabStatus[i] = new QLabel("");
            allTabStatus[i]->setFixedWidth(100);
            hbox->addWidget(allTabStatus[i]);
        }
        // set layout for GroupBox
        allTabCHx[i]->setLayout(hbox);
        // add generated groupbox to tab layout
        allTabLayout->addWidget(allTabCHx[i]);

        // draw line before total box
        if(i == channelsNumber - 1)
            drawLine(allTabLayout);
    }
    // set layout for ALL tab
    allTabLayout->addStretch();
    ui->AllSettings->setLayout(allTabLayout);
}

void HVpsuWidget::createChannelTabs() {
    int i;
    for(i = 0; i < channelsNumber; i++) {
        tabCHx[i] = new QWidget();
        // create tab layout
        QVBoxLayout *qvb = new QVBoxLayout();

        // create custom name box
        QGroupBox *channelCustomNameBox = new QGroupBox("Channel custom name");
        QHBoxLayout *qhbCustomName = new QHBoxLayout();
        tabCHxCustomName[i] = new QLabel("");
        qhbCustomName->addWidget(tabCHxCustomName[i]);
        qhbCustomName->addStretch();

        tabCHxChangeName[i] = new QPushButton("Change name");
        connect(tabCHxChangeName[i], SIGNAL(pressed()), this, SLOT(changeNamePressed()));
        qhbCustomName->addWidget(tabCHxChangeName[i]);

        channelCustomNameBox->setLayout(qhbCustomName);
        qvb->addWidget(channelCustomNameBox);

        // create channel polarity box
        QGroupBox *channelPolarityBox = new QGroupBox("Channel polarity");
        QHBoxLayout *qhbPolarity = new QHBoxLayout();
        tabCHxPOLARITY[i] = new QLabel(".");
        qhbPolarity->addWidget(tabCHxPOLARITY[i]);
        qhbPolarity->addStretch();
        channelPolarityBox->setLayout(qhbPolarity);
        qvb->addWidget(channelPolarityBox);
        // create channel status box
        QGroupBox *channelStatusBox = new QGroupBox("Channel status");
        QHBoxLayout *qhbStatus = new QHBoxLayout();
        tabCHxSTATUS[i] = new QLabel(".");
        qhbStatus->addWidget(tabCHxSTATUS[i]);
        qhbStatus->addStretch();
        channelStatusBox->setLayout(qhbStatus);
        qvb->addWidget(channelStatusBox);

        // create all measurements box
        QGroupBox *channelMeasurementsBox = new QGroupBox("Channel measurements");
        QHBoxLayout *qhbMeasurements = new QHBoxLayout();
        // begin led
        tabCHxLed[i] = new KLed();
        tabCHxLed[i]->setFixedSize(20, 20);
        tabCHxLed[i]->off();
        tabCHxLed[i]->setColor(Qt::red);
        qhbMeasurements->addWidget(tabCHxLed[i]);
        // end led
        // begin buttons
        tabCHxOn[i] = new QRadioButton("ON");
        tabCHxOff[i] = new QRadioButton("OFF");
        tabCHxOn[i]->setLayoutDirection(Qt::RightToLeft);
        tabCHxOff[i]->setChecked(true);
        tabCHxOn[i]->setChecked(false);
        tabCHxOn[i]->setEnabled(false);
        tabCHxOff[i]->setEnabled(false);
        connect(tabCHxOn[i], SIGNAL(pressed()), this, SLOT(onPressed()));
        connect(tabCHxOff[i], SIGNAL(pressed()), this, SLOT(offPressed()));
        qhbMeasurements->addWidget(tabCHxOn[i]);
        qhbMeasurements->addWidget(tabCHxOff[i]);
        // end buttons
        // begin voltages
        tabCHxvoltage[i] = new QLCDNumber();
        tabCHxvoltage[i]->setDigitCount(6);
        tabCHxvoltage[i]->setSegmentStyle(QLCDNumber::Flat);
        tabCHxvoltage[i]->setMinimumSize(QSize(110, 36));
        QPalette palette = tabCHxvoltage[i]->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        tabCHxvoltage[i]->setPalette(palette);
        tabCHxvoltage[i]->display(0.00);
        qhbMeasurements->addWidget(tabCHxvoltage[i]);
        // end measured voltages
        qhbMeasurements->addSpacing(20);
        // begin Vset
        QLabel *VsetLabel;
        VsetLabel = new QLabel("VSET [V]:");
        qhbMeasurements->addWidget(VsetLabel);
        tabCHxVset[i] = new QLabel("");
        tabCHxVset[i]->setFixedWidth(80);
        QFont font = tabCHxVset[i]->font();
        font.setPointSize(14);
        font.setBold(true);
        tabCHxVset[i]->setFont(font);
        tabCHxVset[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QString val;
        val.sprintf("%6.1lf", 0.0);
        tabCHxVset[i]->setText(val);
        qhbMeasurements->addWidget(tabCHxVset[i]);
        // end VSet
        tabCHxSetV[i] = new QPushButton("Set V");
        tabCHxSetV[i]->setFixedWidth(40);
        tabCHxSetV[i]->setEnabled(false);
        qhbMeasurements->addWidget(tabCHxSetV[i]);
        connect(tabCHxSetV[i], SIGNAL(pressed()), this, SLOT(setVPressed()));
        // end setV
        qhbMeasurements->addSpacing(20);
        // begin IMON and ISET
        QGridLayout *Igrid = new QGridLayout();
        QLabel *ImonLabel = new QLabel("IMON [uA]: ");
        Igrid->addWidget(ImonLabel, 0, 0, 1, 1);
        tabCHxImon[i] = new QLabel("");
        tabCHxImon[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        auto iFont = tabCHxImon[i]->font();
        iFont.setPointSize(14);
        tabCHxImon[i]->setFont(iFont);
        val.sprintf("%7.3lf", 0.0);
        tabCHxImon[i]->setText(val);
        Igrid->addWidget(tabCHxImon[i], 0, 1, 1, 1);
        qhbMeasurements->addLayout(Igrid);
        // end IMON and ISET
        qhbMeasurements->addStretch();
        // begin kill button
        tabCHxKill[i] = new QPushButton("KILL");
        tabCHxKill[i]->setFixedWidth(40);
        // palette = tabCHxKill[i]->palette();
        // palette.setColor(QPalette::ButtonText, Qt::red);
        tabCHxKill[i]->setPalette(palette);
        tabCHxKill[i]->setEnabled(false);
        qhbMeasurements->addWidget(tabCHxKill[i]);
        // end kill
        channelMeasurementsBox->setLayout(qhbMeasurements);
        qvb->addWidget(channelMeasurementsBox);

        // create channel settings box
        QGroupBox *channelSettingsBox = new QGroupBox("Channel settings");
        QVBoxLayout *qvbSettings = new QVBoxLayout();
        QHBoxLayout *qhbSettings1 = new QHBoxLayout();
        QHBoxLayout *qhbSettings2 = new QHBoxLayout();
        //  tabSTATUSx[i]=new QLabel(".");
        //  qhbSettings->addWidget(tabSTATUSx[i]);
        // qhbSettings->addStretch();
        channelSettingsBox->setLayout(qvbSettings);
        qvbSettings->addLayout(qhbSettings1);
        qvbSettings->addLayout(qhbSettings2);
        channelSettingsBox->setLayout(qhbSettings1);
        // begin VMAX
        QLabel *VMAXLabel;
        VMAXLabel = new QLabel("VMAX [V]:");
        qhbSettings1->addWidget(VMAXLabel);
        tabCHxVMAX[i] = new QLabel();
        tabCHxVMAX[i]->setFixedWidth(50);
        QFont fontMAX = tabCHxVMAX[i]->font();
        // font.setPointSize(72);
        fontMAX.setBold(true);
        tabCHxVMAX[i]->setFont(fontMAX);
        tabCHxVMAX[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QString valMAX;
        valMAX.sprintf("%6.1lf", 0.0);
        tabCHxVMAX[i]->setText(valMAX);
        qhbSettings1->addWidget(tabCHxVMAX[i]);
        tabCHxVMAX[i]->setEnabled(true);
        // end VMAX
        // begin setVMAX
        tabCHxSetVMAX[i] = new QPushButton("Set VMAX");
        // tabCHxSetVMAX[i]->setFixedWidth(70);
        tabCHxSetVMAX[i]->setEnabled(false);
        qhbSettings1->addWidget(tabCHxSetVMAX[i]);
        connect(tabCHxSetVMAX[i], SIGNAL(pressed()), this, SLOT(setVMAXPressed()));
        // end setVMAX
        qhbSettings1->addStretch();
        // begin RUP
        QLabel *RUPLabel;
        RUPLabel = new QLabel("Ramp Up [V/s]:");
        qhbSettings1->addWidget(RUPLabel);
        tabCHxRUP[i] = new QLabel("");
        tabCHxRUP[i]->setFixedWidth(50);
        QFont fontRUP = tabCHxRUP[i]->font();
        // font.setPointSize(72);
        fontRUP.setBold(true);
        tabCHxRUP[i]->setFont(fontRUP);
        tabCHxRUP[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        auto valRUP = QString::number(0);
        tabCHxRUP[i]->setText(valRUP);
        qhbSettings1->addWidget(tabCHxRUP[i]);
        tabCHxRUP[i]->setEnabled(true);
        // end RUP
        // begin setRUP
        tabCHxSetRUP[i] = new QPushButton("Set RUP");
        // tabCHxSetRUP[i]->setFixedWidth(70);
        tabCHxSetRUP[i]->setEnabled(false);
        qhbSettings1->addWidget(tabCHxSetRUP[i]);
        connect(tabCHxSetRUP[i], SIGNAL(pressed()), this, SLOT(setRUPPressed()));
        // end setRUP
        // begin Iset
        QLabel *IsetLabel;
        IsetLabel = new QLabel("Iset [uA]:\t ");
        qhbSettings2->addWidget(IsetLabel);
        tabCHxIset[i] = new QLabel();
        tabCHxIset[i]->setFixedWidth(50);
        QFont fontIset = tabCHxIset[i]->font();
        // font.setPointSize(72);
        fontIset.setBold(true);
        tabCHxIset[i]->setFont(fontMAX);
        tabCHxIset[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QString valIset;
        valIset.sprintf("%6.1lf", 0.0);
        tabCHxIset[i]->setText(valIset);
        qhbSettings2->addWidget(tabCHxIset[i]);
        tabCHxIset[i]->setEnabled(true);
        // end Iset
        // begin setIset
        tabCHxSetIset[i] = new QPushButton("Set current");
        // tabCHxSetIset[i]->setFixedWidth(70);
        tabCHxSetVMAX[i]->setEnabled(false);
        qhbSettings2->addWidget(tabCHxSetIset[i]);
        connect(tabCHxSetIset[i], SIGNAL(pressed()), this, SLOT(setCurrentPressed()));
        // end setIset

        qhbSettings2->addStretch();
        // begin RDWN
        QLabel *RDWNLabel;
        RDWNLabel = new QLabel("Ramp Down [V/s]:");
        qhbSettings2->addWidget(RDWNLabel);
        tabCHxRDWN[i] = new QLabel("");
        tabCHxRDWN[i]->setFixedWidth(50);
        QFont fontRDWN = tabCHxRDWN[i]->font();
        // font.setPointSize(72);
        fontRDWN.setBold(true);
        tabCHxRDWN[i]->setFont(fontRDWN);
        tabCHxRDWN[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        auto valRDWN = QString::number(0);
        tabCHxRDWN[i]->setText(valRDWN);
        qhbSettings2->addWidget(tabCHxRDWN[i]);
        tabCHxRDWN[i]->setEnabled(true);
        // end RDWN
        // begin setRDWN
        tabCHxSetRDWN[i] = new QPushButton("Set RDWN");
        // tabCHxSetRDWN[i]->setFixedWidth(70);
        tabCHxSetRDWN[i]->setEnabled(false);
        qhbSettings2->addWidget(tabCHxSetRDWN[i]);
        connect(tabCHxSetRDWN[i], SIGNAL(pressed()), this, SLOT(setRDWNPressed()));

        // end setRDWN

        qvb->addWidget(channelSettingsBox);
        tabCHxTrace[i] = new DCSTraceListWidget;
        qvb->addWidget(tabCHxTrace[i]);
        qvb->addStretch();

        tabCHx[i]->setLayout(qvb);
        ui->MainTab->addTab(tabCHx[i], tr("CH %1").arg(i));
    }

    // dummy channel channelsNumber - total - is specific
    tabCHx[channelsNumber] = new QWidget();
    // create tab layout
    QVBoxLayout *qvbt = new QVBoxLayout();

    // create custom name box
    QGroupBox *channelCustomNameBox = new QGroupBox("Channel custom name");
    QHBoxLayout *qhbCustomName = new QHBoxLayout();
    tabCHxCustomName[channelsNumber] = new QLabel("");
    qhbCustomName->addWidget(tabCHxCustomName[i]);

    qhbCustomName->addStretch();

    tabCHxChangeName[channelsNumber] = new QPushButton("Change name");
    connect(tabCHxChangeName[channelsNumber], SIGNAL(pressed()), this,
            SLOT(changeNamePressed()));
    qhbCustomName->addWidget(tabCHxChangeName[channelsNumber]);

    channelCustomNameBox->setLayout(qhbCustomName);
    qvbt->addWidget(channelCustomNameBox);
    // end custom name box

    qvbt->addStretch();

    tabCHx[channelsNumber]->setLayout(qvbt);
    ui->MainTab->addTab(tabCHx[channelsNumber], tr("TOTAL"));
}

void HVpsuWidget::drawLine(QLayout *layout) {
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
}

DT1415Widget::DT1415Widget(std::string name, int enabledChannels, QWidget *parent)
    : HVpsuWidget(name, enabledChannels, parent) {

    // if(enabledChannels < 8) {
    ui->HVGUI->setPixmap(
        QPixmap(QString::fromUtf8(":/images/res/hv_combo_DT14170ET_gui_pic.png")));
    //}
}

DT1470Widget::DT1470Widget(std::string name, int enabledChannels, QWidget *parent)
    : HVpsuWidget(name, enabledChannels, parent) {
    ui->HVGUI->setPixmap(
        QPixmap(QString::fromUtf8(":/images/res/hv_combo_DT14170ET_gui_pic.png")));
    // .scaled(10, 10, Qt::KeepAspectRatio));
}

N1471Widget::N1471Widget(std::string name, int enabledChannels, QWidget *parent)
    : HVpsuWidget(name, enabledChannels, parent) {
    ui->HVGUI->setPixmap(QPixmap(QString::fromUtf8(":/images/res/hvcombo_gui_pic.png")));
    //                           .scaled(10, 10, Qt::KeepAspectRatio));
}

void N1471Widget::updateConfiguration(UA_DataValue *data) {
    auto timestamp = DCSUtils::getTimestamp(data);
    UA_DT1415c channelStatus = *static_cast<UA_DT1415c *>(data->value.data);
    if(channelStatus.statusSize) {
        // bool ON;//, enabled;
        QString val;
        double totalVoltageSet = 0;
        for(int i = 0; i < channelsNumber; ++i) {
            N1471codes::ChannelStatus chanStat =
                static_cast<N1471codes::ChannelStatus>(channelStatus.status[i]);
            enabled[i] = channelStatus.isRemote &&
                         !static_cast<bool>(chanStat & N1471codes::ChannelStatus::DIS);
            ON[i] = static_cast<bool>(chanStat & N1471codes::ChannelStatus::ON);
            tabCHxLed[i]->setState(static_cast<KLed::State>(ON[i] && connectionState));
            allTabOn[i]->setEnabled(enabled[i] && connectionState);
            allTabOff[i]->setEnabled(enabled[i] && connectionState);
            // allTabKill[i]->setEnabled(enabled[i] && connectionState);
            allTabSetV[i]->setEnabled(enabled[i] && connectionState);
            allTabLed[i]->setState(static_cast<KLed::State>(ON[i] && connectionState));
            allTabOn[i]->setChecked(ON[i] && connectionState);
            allTabOff[i]->setChecked((!ON[i]) && connectionState);

            val = QString::fromStdString(N1471codes::translateChannelStatus(chanStat));
            tabCHxSTATUS[i]->setText(val);
            allTabStatus[i]->setText(val);

            auto palette = allTabStatus[i]->palette();
            if(static_cast<bool>(chanStat & N1471codes::ChannelStatus::ON)) {
                palette.setColor(QPalette::WindowText, Qt::darkGreen);
            } else if(static_cast<bool>(chanStat & (N1471codes::ChannelStatus::OVC |
                                                    N1471codes::ChannelStatus::OVV |
                                                    N1471codes::ChannelStatus::UNV |
                                                    N1471codes::ChannelStatus::MAXV |
                                                    N1471codes::ChannelStatus::TRIP |
                                                    N1471codes::ChannelStatus::OVP |
                                                    N1471codes::ChannelStatus::OVT |
                                                    N1471codes::ChannelStatus::KILL |
                                                    N1471codes::ChannelStatus::ILK |
                                                    N1471codes::ChannelStatus::NOCAL))) {
                palette.setColor(QPalette::WindowText, Qt::red);

            } else {
                palette.setColor(QPalette::WindowText, Qt::black);
            }
            allTabStatus[i]->setPalette(palette);
            tabCHxSTATUS[i]->setPalette(palette);
            tabCHxSetRUP[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetRDWN[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetIset[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetVMAX[i]->setEnabled(enabled[i] && connectionState);

            tabCHxOn[i]->setEnabled(enabled[i] && connectionState);
            tabCHxOff[i]->setEnabled(enabled[i] && connectionState);
            // tabCHxKill[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetV[i]->setEnabled(enabled[i] && connectionState);

            tabCHxOn[i]->setChecked(ON[i] && connectionState);
            tabCHxOff[i]->setChecked((!ON[i]) && connectionState);

            totalVoltageSet += channelStatus.voltageSet[i];
            val.sprintf("%6.1lf", channelStatus.voltageSet[i]);
            allTabVset[i]->setText(val);
            tabCHxVset[i]->setText(val);
            val = QString::number(channelStatus.rup[i]);
            tabCHxRUP[i]->setText(val);
            val = QString::number(channelStatus.rdown[i]);
            tabCHxRDWN[i]->setText(val);
            val.sprintf("%4.1lf", channelStatus.voltageMax[i]);
            tabCHxVMAX[i]->setText(val);
            val.sprintf("%3.1lf", channelStatus.currentSet[i]);
            tabCHxIset[i]->setText(val);

            std::string std;
            if(channelStatus.polarity[i].length != 0) {
                std =
                    std::string(reinterpret_cast<char *>(channelStatus.polarity[i].data),
                                channelStatus.polarity[i].length);
            }
            tabCHxPOLARITY[i]->setText(QString::fromStdString(std));
        }
        val.sprintf("%6.1lf", totalVoltageSet);
        allTabVset[channelsNumber]->setText(val);
        allOn->setEnabled(channelStatus.isRemote && connectionState);
        allOff->setEnabled(channelStatus.isRemote && connectionState);
        // allTabKill[channelsNumber]->setEnabled(channelStatus.isRemote);
        isRemote = channelStatus.isRemote;
    }
}

void DT1470Widget::updateConfiguration(UA_DataValue *data) {
    auto timestamp = DCSUtils::getTimestamp(data);
    UA_DT1415c channelStatus = *static_cast<UA_DT1415c *>(data->value.data);
    if(channelStatus.statusSize) {
        // bool ON;//, enabled;
        QString val;
        double totalVoltageSet = 0;
        for(int i = 0; i < channelsNumber; ++i) {
            DT1470ETcodes::ChannelStatus chanStat =
                static_cast<DT1470ETcodes::ChannelStatus>(channelStatus.status[i]);
            enabled[i] = channelStatus.isRemote &&
                         !static_cast<bool>(chanStat & DT1470ETcodes::ChannelStatus::DIS);
            ON[i] = static_cast<bool>(chanStat & DT1470ETcodes::ChannelStatus::ON);
            tabCHxLed[i]->setState(static_cast<KLed::State>(ON[i] && connectionState));
            allTabOn[i]->setEnabled(enabled[i] && connectionState);
            allTabOff[i]->setEnabled(enabled[i] && connectionState);
            // allTabKill[i]->setEnabled(enabled[i] && connectionState);
            allTabSetV[i]->setEnabled(enabled[i] && connectionState);
            allTabLed[i]->setState(static_cast<KLed::State>(ON[i] && connectionState));
            allTabOn[i]->setChecked(ON[i] && connectionState);
            allTabOff[i]->setChecked((!ON[i]) && connectionState);

            val = QString::fromStdString(DT1470ETcodes::translateChannelStatus(chanStat));
            tabCHxSTATUS[i]->setText(val);
            allTabStatus[i]->setText(val);
            auto palette = allTabStatus[i]->palette();
            if(static_cast<bool>(chanStat & DT1470ETcodes::ChannelStatus::ON)) {
                palette.setColor(QPalette::WindowText, Qt::darkGreen);
            } else if(static_cast<bool>(chanStat &
                                        (DT1470ETcodes::ChannelStatus::OVC |
                                         DT1470ETcodes::ChannelStatus::OVV |
                                         DT1470ETcodes::ChannelStatus::UNV |
                                         DT1470ETcodes::ChannelStatus::MAXV |
                                         DT1470ETcodes::ChannelStatus::TRIP |
                                         DT1470ETcodes::ChannelStatus::OVP |
                                         DT1470ETcodes::ChannelStatus::OVT |
                                         DT1470ETcodes::ChannelStatus::KILL |
                                         DT1470ETcodes::ChannelStatus::ILK |
                                         DT1470ETcodes::ChannelStatus::NOCAL))) {
                palette.setColor(QPalette::WindowText, Qt::red);
                tabCHxTrace[i]->addItem(val, timestamp);
            } else {
                palette.setColor(QPalette::WindowText, Qt::black);
            }
            allTabStatus[i]->setPalette(palette);
            tabCHxSTATUS[i]->setPalette(palette);
            tabCHxSetRUP[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetRDWN[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetIset[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetVMAX[i]->setEnabled(enabled[i] && connectionState);

            tabCHxOn[i]->setEnabled(enabled[i] && connectionState);
            tabCHxOff[i]->setEnabled(enabled[i] && connectionState);
            // tabCHxKill[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetV[i]->setEnabled(enabled[i] && connectionState);

            tabCHxOn[i]->setChecked(ON[i] && connectionState);
            tabCHxOff[i]->setChecked((!ON[i]) && connectionState);

            totalVoltageSet += channelStatus.voltageSet[i];
            val.sprintf("%6.1lf", channelStatus.voltageSet[i]);
            allTabVset[i]->setText(val);
            tabCHxVset[i]->setText(val);
            val = QString::number(channelStatus.rup[i]);
            tabCHxRUP[i]->setText(val);
            val = QString::number(channelStatus.rdown[i]);
            tabCHxRDWN[i]->setText(val);
            val.sprintf("%4.1lf", channelStatus.voltageMax[i]);
            tabCHxVMAX[i]->setText(val);
            val.sprintf("%3.1lf", channelStatus.currentSet[i]);
            tabCHxIset[i]->setText(val);
            std::string std;
            if(channelStatus.polarity[i].length != 0) {
                std =
                    std::string(reinterpret_cast<char *>(channelStatus.polarity[i].data),
                                channelStatus.polarity[i].length);
            }
            tabCHxPOLARITY[i]->setText(QString::fromStdString(std));
        }
        val.sprintf("%6.1lf", totalVoltageSet);
        allTabVset[channelsNumber]->setText(val);
        allOn->setEnabled(channelStatus.isRemote && connectionState);
        allOff->setEnabled(channelStatus.isRemote && connectionState);
        // allTabKill[channelsNumber]->setEnabled(channelStatus.isRemote);
        isRemote = channelStatus.isRemote;
    }
}

#include "include/hvpsuwidget.h"
#include "ui_hvpsuwidget.h"
#include <iostream>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QGridLayout>

HVpsuWidget::HVpsuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HVpsuWidget)
{
    ui->setupUi(this);
    loadConfig();   //need to be called first because create functions use some settings!
    createAllChannelsTab();
    createChannelTabs();
    setChannelsNames();

    ui->connectionIP->setText(QSettings().value("HVpsu/IP").toString());
    ui->connectionPort->setText(QSettings().value("HVpsu/Port").toString());

//    HVpsuController *controller = new HVpsuController();//
//    HVpsuControllerPtr.reset(controller);
    connect(ui->connect, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
//    connect(ui->disconnect, SIGNAL(clicked(bool)), controller, SLOT(deviceDisconnect()));
//    connect(controller, SIGNAL(connected()), this, SLOT(onConnect()));
//    connect(controller, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
 //   connect(controller, SIGNAL(statusChanged(QString)), this, SLOT(updateStatus(QString)));

//    qRegisterMetaType<DT1415Measurements>();
//    connect(controller, SIGNAL(measurementsReadFromDevice(DT1415Measurements)),
//            this, SLOT(updateMeasurements(DT1415Measurements)));

//    qRegisterMetaType<DT1415ChannelStatus>();
//    connect(controller, SIGNAL(channelStatusReadFromDevice(DT1415ChannelStatus)),
 //           this, SLOT(updateChannelStatus(DT1415ChannelStatus)));


}

HVpsuWidget::~HVpsuWidget()
{
//    HVpsuControllerPtr->wait();
//    HVpsuControllerPtr.reset();
    delete ui;
}

void HVpsuWidget::onConnect()
{
    ui->connect->setEnabled(false);
    ui->disconnect->setEnabled(true);
    ui->connectionStatus->setText("CONNECTED");
    QPalette palette = ui->connectionStatus->palette();
    palette.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->connectionStatus->setPalette(palette);

    ui->connectionIP->setEnabled(false);
    ui->connectionPort->setEnabled(false);

    initialUpdate = true;
//    HVpsuControllerPtr->start();
}

void HVpsuWidget::onDisconnect()
{
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->connectionStatus->setText("DISCONNECTED");
    QPalette palette = ui->connectionStatus->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    ui->connectionStatus->setPalette(palette);

    ui->statusLabel->setText("...");

    ui->connectionIP->setEnabled(true);
    ui->connectionPort->setEnabled(true);

    int i;
    for(i=0; i<9; i++)
    {
        allTabOn[i]->setEnabled(false);
        allTabOff[i]->setEnabled(false);
        allTabKill[i]->setEnabled(false);
        allTabSetV[i]->setEnabled(false);
    }
}

void HVpsuWidget::closeEvent(QCloseEvent* e)
{
 //   HVpsuControllerPtr->deviceDisconnect();
    //save settings
    QSettings().setValue("HVpsu/IP",ui->connectionIP->text());
    QSettings().setValue("HVpsu/Port",ui->connectionPort->text());
    saveConfig();

    QWidget::closeEvent(e);
}

void HVpsuWidget::updateStatus(QString info)
{
    ui->statusLabel->setText(info);
}

void HVpsuWidget::deviceConnect()
{
//   TCPConnectionParameters cp;
//   cp.IPaddress = ui->connectionIP->text().toStdString();
//   cp.port = ui->connectionPort->text().toInt();
//   HVpsuControllerPtr->deviceConnect(&cp);
}
/*
void HVpsuWidget::updateMeasurements(DT1415Measurements measurements)
{
    int i;
    QString val;
    for(i=0; i<8; i++)
    {
        val.sprintf("%.1lf", measurements.voltage[i]);
        allTabCHvoltage[i]->display(val);

        val.sprintf("%.3lf", measurements.current[i]);
        allTabImon[i]->setText(val);
    }
    val.sprintf("%.1lf", measurements.totalVolatge);
    allTabCHvoltage[8]->display(val);
}

void HVpsuWidget::updateChannelStatus(DT1415ChannelStatus channelStatus)
{
    int i;
    bool ON, enabled;
    QString val;
    for(i=0; i<8; i++)
    {
        enabled = channelStatus.isRemote & !(bool)(channelStatus.status[i] & DT1415ETchannelStatus::ISDIS);
        allTabOn[i]->setEnabled(enabled);
        allTabOff[i]->setEnabled(enabled);
        //allTabKill[i]->setEnabled(enabled);
        allTabSetV[i]->setEnabled(enabled);

        ON = (bool)(channelStatus.status[i] & DT1415ETchannelStatus::ON);
        allTabLed[i]->setState((KLed::State)(ON));
        if(!isRemotePrevious | initialUpdate) //update ON/OFF only when remote is disabled or connection is established
        {
            allTabOn[i]->setChecked(ON);
            allTabOff[i]->setChecked(!ON);

            val.sprintf("%6.1lf", channelStatus.voltageSet[i]);
            allTabVset[i]->setText(val);

            if(i==7)    //when initial update be sure to update all channels (Q: do second for ?)
                initialUpdate = false;
        }

    }
    val.sprintf("%6.1lf", channelStatus.voltageSetTotal);
    allTabVset[8]->setText(val);
    allTabOn[8]->setEnabled(channelStatus.isRemote);
    allTabOff[8]->setEnabled(channelStatus.isRemote);
    //allTabKill[8]->setEnabled(channelStatus.isRemote);

    isRemotePrevious = channelStatus.isRemote;
}
*/
void HVpsuWidget::onPressed()
{
    QObject* obj = sender();
    int i;
    for(i=0; i<9; i++)
    {
 //       if(allTabOn[i] == obj)
 //           HVpsuControllerPtr->setON(i);
            //std::cout << "ON number: " << i << std::endl;
    }
}

void HVpsuWidget::offPressed()
{
    QObject* obj = sender();
    int i;
    for(i=0; i<9; i++)
    {
//        if(allTabOff[i] == obj)
//            HVpsuControllerPtr->setOFF(i);
            //std::cout << "ON number: " << i << std::endl;
    }
}

void HVpsuWidget::setVPressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<8; i++)
    {
        if(allTabSetV[i] == obj)
        {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 voltage").arg(i),
                                          label, allTabVset[i]->text().toDouble(), 0, 1000, 1, &ok);
            if (ok)
            {
                QString val;
                val.sprintf("%6.1lf", d);
                allTabVset[i]->setText(val);
                //TODO: check VtotalMax (software treshold). if sum > thr, drop input and show message or set max mallowed and show message?
 //               HVpsuControllerPtr->setVSET(i, d);
            }
        }
    }
}

void HVpsuWidget::changeNamePressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<9; i++)
    {
        if(tabCHxChangeName[i] == obj)
        {
            QString newName = QInputDialog::getText(this, tr("Set CH %1 name").arg(i),
                                                     tr("CH %1 name:").arg(i), QLineEdit::Normal, CHxCustomName[i], &ok);
            if(ok)
            {
                CHxCustomName[i] = newName;
                setChannelName(i);
            }
        }
    }
}

//create layout procedures
void HVpsuWidget::loadConfig()
{
    int i;
    QString configkey;
    for(i=0; i<9; i++)
    {
        configkey = tr("HVpsuCH%1/CustomName").arg(i);
        CHxCustomName[i] = QSettings().value(configkey).toString();
    }
}

void HVpsuWidget::saveConfig()
{
    int i;
    QString configkey;
    for(i=0; i<9; i++)
    {
        configkey = tr("HVpsuCH%1/CustomName").arg(i);
        QSettings().setValue(configkey,CHxCustomName[i]);
    }
}

void HVpsuWidget::setChannelName(int channelno)
{
    QString title;
    if(channelno==8)
        title = tr("TOTAL        ");
    else
        title = tr("CH %1        ").arg(channelno);

    title.append(CHxCustomName[channelno]);

    //set name on ALL Channles Tab
    allTabCHx[channelno]->setTitle(title);

    //set name on CH x tab (... if empty)
    if(CHxCustomName[channelno].isEmpty())
        tabCHxCustomName[channelno]->setText("...");
    else
        tabCHxCustomName[channelno]->setText(CHxCustomName[channelno]);

}

void HVpsuWidget::setChannelsNames()
{
    int i;
    for(i=0; i<9; i++)
    {
        setChannelName(i);
    }
}

void HVpsuWidget::createAllChannelsTab()
{
    int i;
    //define layout for ALL tab
    QVBoxLayout *allTabLayout = new QVBoxLayout;
    for(i=0; i<9; i++)
    {
        //Generate GroupBoxes
        allTabCHx[i] = new QGroupBox();

        //allChannelsTabCHx[i]->setStyleSheet("QGroupBox {border: 1px solid silver;margin-top: 6px;font: bold;} QGroupBox::title {subcontrol-origin: margin;left: 7px; padding: 0px 5px 0px 5px;}");

        //define new layout for Groupbox and add customized controls
        QHBoxLayout *hbox = new QHBoxLayout();
        //add customized controls to GroupBox layout
        //begin LED
//        allTabLed[i] = new KLed();
//        allTabLed[i]->setFixedSize(20,20);
//        allTabLed[i]->off();
//        allTabLed[i]->setColor(Qt::red);

//        hbox->addWidget(allTabLed[i]);
        //end LED

        //Begin enable
        QHBoxLayout *hl = new QHBoxLayout();
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

        hbox->addLayout(hl);
        //End enable

        //begin measured voltages
        allTabCHvoltage[i] = new QLCDNumber();
        allTabCHvoltage[i]->setDigitCount(6);
        allTabCHvoltage[i]->setSegmentStyle(QLCDNumber::Flat);
        allTabCHvoltage[i]->setMinimumSize(QSize(110,36));
        QPalette palette = allTabCHvoltage[i]->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        allTabCHvoltage[i]->setPalette(palette);

        allTabCHvoltage[i]->display(0.00);

        hbox->addWidget(allTabCHvoltage[i]);
        //end measured voltagese

        hbox->addSpacing(20);

        //begin Vset
        QLabel *VsetLabel;
        VsetLabel = new QLabel("VSET [V]:");
        hbox->addWidget(VsetLabel);

        allTabVset[i] = new QLabel("");
        allTabVset[i]->setFixedWidth(40);
        QFont font = allTabVset[i]->font();
        //font.setPointSize(72);
        font.setBold(true);
        allTabVset[i]->setFont(font);
        allTabVset[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        QString val;
        val.sprintf("%6.1lf", 0.0);
        allTabVset[i]->setText(val);

        hbox->addWidget(allTabVset[i]);
        //end Vset

        //begin setV
        allTabSetV[i] = new QPushButton("Set V");
        allTabSetV[i]->setFixedWidth(40);
        allTabSetV[i]->setEnabled(false);
        if(i!=8)
        {
            hbox->addWidget(allTabSetV[i]);
            connect(allTabSetV[i], SIGNAL(pressed()), this, SLOT(setVPressed()));
        }
        //end setV

        hbox->addSpacing(20);

        //begin IMON and ISET
        QGridLayout *Igrid = new QGridLayout();
        QLabel *ImonLabel = new QLabel("IMON [uA]: ");
        Igrid->addWidget(ImonLabel, 0, 0, 1, 1);

        allTabImon[i] = new QLabel("");
        allTabImon[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        val.sprintf("%7.3lf", 0.0);
        allTabImon[i]->setText(val);
        Igrid->addWidget(allTabImon[i], 0, 1, 1, 1);

        /*QLabel *IsetLabel = new QLabel("ISET [uA]: ");
        Igrid->addWidget(IsetLabel, 1, 0, 1, 1);

        allTabIset[i] = new QLabel("");
        allTabIset[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        val.sprintf("%7.3lf", 0.0);
        allTabIset[i]->setText(val);
        Igrid->addWidget(allTabIset[i], 1, 1, 1, 1);*/

        if(i!=8)
            hbox->addLayout(Igrid);
        //end IMON and ISET

        hbox->addStretch();

        //begin kill button
        allTabKill[i] = new QPushButton("KILL");
        allTabKill[i]->setFixedWidth(40);
        //palette = allTabKill[i]->palette();
        //palette.setColor(QPalette::ButtonText, Qt::red);
        allTabKill[i]->setPalette(palette);
        allTabKill[i]->setEnabled(false);

        hbox->addWidget(allTabKill[i]);
        //end kill button

        //set layout for GroupBox
        allTabCHx[i]->setLayout(hbox);
        //add generated groupbox to tab layout
        allTabLayout->addWidget(allTabCHx[i]);

        //draw line before total box
        if(i==7)
            drawLine(allTabLayout);
    }
    //set layout for ALL tab
    ui->AllSettings->setLayout(allTabLayout);
}

void HVpsuWidget::createChannelTabs()
{
    int i;
    for(i=0; i<8; i++)
    {
        tabCHx[i] = new QWidget();
        //create tab layout
        QVBoxLayout *qvb = new QVBoxLayout();

        //create custom name box
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

        //create channel status box
        QGroupBox *channelStatusBox = new QGroupBox("Channel status");
        qvb->addWidget(channelStatusBox);

        //create all measurements box
        QGroupBox *channelMeasurementsBox = new QGroupBox("Channel measurements");
        qvb->addWidget(channelMeasurementsBox);

        //create channel settings box
        QGroupBox *channelSettingsBox = new QGroupBox("Channel settings");
        qvb->addWidget(channelSettingsBox);

        qvb->addStretch();

        tabCHx[i]->setLayout(qvb);
        ui->MainTab->addTab(tabCHx[i], tr("CH %1").arg(i));
    }

    //dummy channel 8 - total - is specific
    tabCHx[8] = new QWidget();
    //create tab layout
    QVBoxLayout *qvbt = new QVBoxLayout();

    //create custom name box
    QGroupBox *channelCustomNameBox = new QGroupBox("Channel custom name");
    QHBoxLayout *qhbCustomName = new QHBoxLayout();
    tabCHxCustomName[8] = new QLabel("");
    qhbCustomName->addWidget(tabCHxCustomName[i]);

    qhbCustomName->addStretch();

    tabCHxChangeName[8] = new QPushButton("Change name");
    connect(tabCHxChangeName[8], SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    qhbCustomName->addWidget(tabCHxChangeName[8]);

    channelCustomNameBox->setLayout(qhbCustomName);
    qvbt->addWidget(channelCustomNameBox);
    // end custom name box

    qvbt->addStretch();

    tabCHx[8]->setLayout(qvbt);
    ui->MainTab->addTab(tabCHx[8], tr("TOTAL"));
}

void HVpsuWidget::drawLine(QLayout *layout)
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
}

#include "hvpsuwidget.h"
#include "ui_hvpsuwidget.h"
#include <iostream>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QGridLayout>
#include "dialogs.h"
HVpsuWidget::HVpsuWidget(std::string name,QWidget *parent) : AbstractWidget(name,true,parent), ui(new Ui::HVpsuWidget)
{
    ui->setupUi(this);
    ui->tcpLayout->addWidget(tcp);
    loadConfig();   //need to be called first because create functions use some settings!
    createAllChannelsTab();
    createChannelTabs();
    setChannelsNames();
    HVController=new hv_controller(instanceName);
    connectSignals();
}
HVpsuWidget::HVpsuWidget(std::string name, std::string address, std::string port, QWidget *parent): HVpsuWidget(name,parent){
    if(address.size()){
        tcp->setIP(address);
    }
    if(port.size()){
        tcp->setPort(port);
    }
}

HVpsuWidget::~HVpsuWidget()
{
    delete ui;
    delete HVController;

}

void HVpsuWidget::connectSignals(){
    AbstractWidget::connectSignals();
    connect(HVController,SIGNAL(statusChanged(void*)),this,SLOT(updateStatus(void*)));
    connect(HVController,SIGNAL(measurementsChanged(void*)),this,SLOT(updateMeasurements(void*)));
    connect(HVController,SIGNAL(configurationChanged(void*)),this,SLOT(updateConfiguration(void*)));
}
void HVpsuWidget::controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp){
     HVController->opcInit(client,config,resp);
}

void HVpsuWidget::updateStatus(void *data){
    AbstractWidget::updateStatus(data);
    bool isConnected=*static_cast<bool*>(data);
    connectionState=isConnected;
    if(isConnected){
        for(int i=0; i<8; i++){
            allTabOn[i]->setEnabled(enabled[i]);
            allTabOff[i]->setEnabled(enabled[i]);
            allTabOn[i]->setChecked(ON[i]);
            allTabOff[i]->setChecked(!ON[i]);
            //allTabKill[i]->setEnabled(enabled[i]);
            allTabSetV[i]->setEnabled(enabled[i]);
            allTabLed[i]->setState((KLed::State)(ON[i]));
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
            //tabCHxKill[i]->setEnabled(enabled[i]);
            tabCHxSetV[i]->setEnabled(enabled[i]);
            tabCHxLed[i]->setState((KLed::State)(ON[i]));
            tabCHxVset[i]->setEnabled(true);
            tabCHxImon[i]->setEnabled(true);

        }
        allTabLed[8]->setState((KLed::State)(true));
        allTabVset[8]->setEnabled(true);
        allOn->setEnabled(isRemote);
        allOff->setEnabled(isRemote);
    }
    else{
        for(int i=0; i<8; i++){
            allTabOn[i]->setEnabled(false);
            allTabOff[i]->setEnabled(false);
            allTabKill[i]->setEnabled(false);
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

        }
        allTabLed[8]->setState(static_cast<KLed::State>(false));
        allTabVset[8]->setEnabled(false);
        allOn->setEnabled(false);
        allOff->setEnabled(false);
    }
}


void HVpsuWidget::updateMeasurements(void *data){
    UA_DT1415m measurements=*static_cast<UA_DT1415m*>(data);
    if(measurements.voltageSize){
        QString val;
        for(int i=0; i<8; i++)
        {
            val.sprintf("%.1lf", measurements.voltage[i]);
            allTabCHvoltage[i]->display(val);
            tabCHxvoltage[i]->display(val);
            val.sprintf("%.3lf", measurements.current[i]);
            allTabImon[i]->setText(val);
            tabCHxImon[i]->setText(val);
        }
        val.sprintf("%.1lf", measurements.totalVoltage);
        allTabCHvoltage[8]->display(val);
    }
}
void HVpsuWidget::updateConfiguration(void *data){
    UA_DT1415c channelStatus=*static_cast<UA_DT1415c*>(data);
    if(channelStatus.statusSize){
       // bool ON;//, enabled;
        QString val;
        for(int i=0; i<8; ++i){
            DT1415ETcodes::ChannelStatus chanStat=static_cast<DT1415ETcodes::ChannelStatus>(channelStatus.status[i]);
            enabled[i] = channelStatus.isRemote && !static_cast<bool>(chanStat & DT1415ETcodes::ChannelStatus::ISDIS);
            ON[i] = static_cast<bool>(chanStat & DT1415ETcodes::ChannelStatus::ON);
            tabCHxLed[i]->setState( static_cast<KLed::State>(ON[i] && connectionState ));
            allTabOn[i]->setEnabled(enabled[i] && connectionState);
            allTabOff[i]->setEnabled(enabled[i] && connectionState);
            //allTabKill[i]->setEnabled(enabled[i] && connectionState);
            allTabSetV[i]->setEnabled(enabled[i] && connectionState);
            allTabLed[i]->setState( static_cast<KLed::State>(ON[i] && connectionState ));
            allTabOn[i]->setChecked(ON[i] && connectionState);
            allTabOff[i]->setChecked((!ON[i]) && connectionState);

            val=QString::fromStdString(DT1415ETcodes::translateChannelStatus(chanStat));
            tabCHxSTATUS[i]->setText(val);
            tabCHxSetRUP[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetRDWN[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetIset[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetVMAX[i]->setEnabled(enabled[i] && connectionState);
    
            tabCHxOn[i]->setEnabled(enabled[i] && connectionState);
            tabCHxOff[i]->setEnabled(enabled[i] && connectionState);
            //tabCHxKill[i]->setEnabled(enabled[i] && connectionState);
            tabCHxSetV[i]->setEnabled(enabled[i] && connectionState);

            tabCHxOn[i]->setChecked(ON[i] && connectionState);
            tabCHxOff[i]->setChecked((!ON[i]) && connectionState);

            val.sprintf("%6.1lf", channelStatus.voltageSet[i]);
            allTabVset[i]->setText(val);
            tabCHxVset[i]->setText(val);
            val.sprintf("%3.1lf", channelStatus.rup[i]);
            tabCHxRUP[i]->setText(val);
            val.sprintf("%3.1lf", channelStatus.rdown[i]);
            tabCHxRDWN[i]->setText(val);
            val.sprintf("%4.1lf", channelStatus.voltageMax[i]);
            tabCHxVMAX[i]->setText(val);
            val.sprintf("%3.1lf", channelStatus.currentSet[i]);
            tabCHxIset[i]->setText(val);
        }
        val.sprintf("%6.1lf", channelStatus.totalVoltageSet);
        allTabVset[8]->setText(val);
        allOn->setEnabled(channelStatus.isRemote && connectionState);
        allOff->setEnabled(channelStatus.isRemote && connectionState);
        //allTabKill[8]->setEnabled(channelStatus.isRemote);
        isRemote=channelStatus.isRemote;
    }
}

void HVpsuWidget::updateStatusLabel(QString info)
{
    ui->statusLabel->setText(info);
}

void HVpsuWidget::deviceConnect()
{
    std::string IPaddress = tcp->getIP();
    int port = tcp->getPort();
    HVController->callConnect(IPaddress,port);
}

void HVpsuWidget::deviceDisconnect(){
    HVController->callDisconnect();
}


void HVpsuWidget::onPressed()
{
    QObject* obj = sender();
    int i;
    for(i=0; i<8; i++)
    {
        if((allTabOn[i] == obj) || (tabCHxOn[i] == obj))
            HVController->callSetChannel(i,true);
    }
}

void HVpsuWidget::offPressed()
{
    QObject* obj = sender();
    for(int i=0; i<8; i++)
    {
        if((allTabOff[i] == obj) || (tabCHxOff[i] == obj))
            HVController->callSetChannel(i,false);
    }
}

void HVpsuWidget::allOnPressed()
{
    HVController->callSetChannel(8,true);
}

void HVpsuWidget::allOffPressed()
{
    HVController->callSetChannel(8,false);
}

void HVpsuWidget::setVPressed()
{
    QObject* obj = sender();
    bool ok;
    for(int i=0; i<8; i++)
    {
        if((allTabSetV[i] == obj) || (tabCHxSetV[i] == obj))
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
               // allTabVset[i]->setText(val);
                //TODO: check VtotalMax (software treshold). if sum > thr, drop input and show message or set max mallowed and show message?
                HVController->callSetVoltage(i, d);
            }
        }
    }
}
void HVpsuWidget::setVMAXPressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<8; i++)
    {
        if(tabCHxSetVMAX[i] == obj)
        {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 Vmax").arg(i),
                                          label, tabCHxVMAX[i]->text().toDouble(), 0, 1000, 1, &ok);
            if (ok)
            {
                QString val;
                val.sprintf("%6.1lf", d);
                QString info;
                info.sprintf("Changing CH %i Vmax to %.1lf V.\nConfirm?",i,d);
                ConfirmDialog c(info,this);
                if(c.exec()){
                    tabCHxVMAX[i]->setText(val);
                    HVController->callSetVoltageMax(i, d);
                }
                
            }
        }
    }
}

void HVpsuWidget::setCurrentPressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<8; i++)
    {
        if(tabCHxSetIset[i] == obj)
        {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Ampers]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Ampers]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 ISet").arg(i),
                                          label, tabCHxIset[i]->text().toDouble(), 0, 1000, 1, &ok);
            if (ok)
            {
                QString val;
                val.sprintf("%6.1lf", d);
                QString info;
                info.sprintf("Changing CH %i Iset to %.1lf A.\nConfirm?",i,d);
                ConfirmDialog c(info,this);
                if(c.exec()){
                    tabCHxIset[i]->setText(val);
                    HVController->callSetCurrent(i, d);
                }
            }
        }
    }
}

void HVpsuWidget::setRUPPressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<8; i++)
    {
        if(tabCHxSetRUP[i] == obj)
        {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts/s]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts/s]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 RUP").arg(i),
                                          label, tabCHxRUP[i]->text().toDouble(), 0, 1000, 1, &ok);
            if (ok)
            {
                QString val;
                val.sprintf("%6.1lf", d);
                tabCHxRUP[i]->setText(val);
                HVController->callSetRampUp(i, d);
            }
        }
    }
}
void HVpsuWidget::setRDWNPressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<8; i++)
    {
        if(tabCHxSetRDWN[i] == obj)
        {
            QString label;
            if(CHxCustomName[i].isEmpty())
                label = tr("CH %1  [Volts/s]:").arg(i);
            else
                label = tr("CH %1  \"%2\"  [Volts/s]:").arg(i).arg(CHxCustomName[i]);

            double d = QInputDialog::getDouble(this, tr("Set CH %1 RDWN").arg(i),
                                          label, tabCHxRDWN[i]->text().toDouble(), 0, 1000, 1, &ok);
            if (ok)
            {
                QString val;
                val.sprintf("%6.1lf", d);
                tabCHxRDWN[i]->setText(val);
                HVController->callSetRampDown(i, d);
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
{   AbstractWidget::loadConfig();

    int i;
    QString configkey;
    for(i=0; i<9; i++)
    {
        configkey.sprintf("%s/CustomName%i",instanceName.c_str(),i);
        CHxCustomName[i] = QSettings().value(configkey).toString();
    }
}

void HVpsuWidget::saveConfig()
{
    AbstractWidget::saveConfig();

    int i;
    QString configkey;
    for(i=0; i<9; i++)
    {
        configkey.sprintf("%s/CustomName%i",instanceName.c_str(),i);
        QSettings().setValue(configkey,CHxCustomName[i]);
    }
}

void HVpsuWidget::setChannelName(int channelno)
{
    QString title;
    if(channelno==8)
        title = tr("TOTAL");
    else
        title = tr("CH %1        ").arg(channelno);

    title.append(CHxCustomName[channelno]);
    title.append("    ");
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
        allTabLed[i] = new KLed();
        allTabLed[i]->setFixedSize(20,20);
        allTabLed[i]->off();
        allTabLed[i]->setColor(Qt::red);

        hbox->addWidget(allTabLed[i]);
        //end LED

        //Begin enable
        QHBoxLayout *hl = new QHBoxLayout();
        if(i!=8){
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
        }
        else{
            allOn=new QPushButton("ON");
            allOff=new QPushButton("OFF");
            allOn->setFixedWidth(50);
            allOff->setFixedWidth(50);
            connect(allOn, SIGNAL(pressed()), this, SLOT(allOnPressed()));
            connect(allOff, SIGNAL(pressed()), this, SLOT(allOffPressed()));
            hl->addWidget(allOn);
            hl->addWidget(allOff);
        }
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
        QHBoxLayout *qhbStatus = new QHBoxLayout();
        tabCHxSTATUS[i]=new QLabel(".");
        qhbStatus->addWidget(tabCHxSTATUS[i]);
        qhbStatus->addStretch();
        channelStatusBox->setLayout(qhbStatus);
        qvb->addWidget(channelStatusBox);


        //create all measurements box
        QGroupBox *channelMeasurementsBox = new QGroupBox("Channel measurements");
        QHBoxLayout *qhbMeasurements=new QHBoxLayout();
        //begin led
        tabCHxLed[i] = new KLed();
        tabCHxLed[i]->setFixedSize(20,20);
        tabCHxLed[i]->off();
        tabCHxLed[i]->setColor(Qt::red);
        qhbMeasurements->addWidget(tabCHxLed[i]);
        //end led
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
        //end buttons
        //begin voltages
        tabCHxvoltage[i] = new QLCDNumber();
        tabCHxvoltage[i]->setDigitCount(6);
        tabCHxvoltage[i]->setSegmentStyle(QLCDNumber::Flat);
        tabCHxvoltage[i]->setMinimumSize(QSize(110,36));
        QPalette palette = tabCHxvoltage[i]->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        tabCHxvoltage[i]->setPalette(palette);
        tabCHxvoltage[i]->display(0.00);
        qhbMeasurements->addWidget(tabCHxvoltage[i]);
        //end measured voltages
        qhbMeasurements->addSpacing(20);
        //begin Vset
        QLabel *VsetLabel;
        VsetLabel = new QLabel("VSET [V]:");
        qhbMeasurements->addWidget(VsetLabel);
        tabCHxVset[i] = new QLabel("");
        tabCHxVset[i]->setFixedWidth(40);
        QFont font = tabCHxVset[i]->font();
        //font.setPointSize(72);
        font.setBold(true);
        tabCHxVset[i]->setFont(font);
        tabCHxVset[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
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
        //end setV
        qhbMeasurements->addSpacing(20);
        //begin IMON and ISET
        QGridLayout *Igrid = new QGridLayout();
        QLabel *ImonLabel = new QLabel("IMON [uA]: ");
        Igrid->addWidget(ImonLabel, 0, 0, 1, 1);
        tabCHxImon[i] = new QLabel("");
        tabCHxImon[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        val.sprintf("%7.3lf", 0.0);
        tabCHxImon[i]->setText(val);
        Igrid->addWidget(tabCHxImon[i], 0, 1, 1, 1);
        /*QLabel *IsetLabel = new QLabel("ISET [uA]: ");
        Igrid->addWidget(IsetLabel, 1, 0, 1, 1);
        tabCHxIset[i] = new QLabel("");
        tabCHxIset[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        val.sprintf("%7.3lf", 0.0);
        tabCHxIset[i]->setText(val);
        Igrid->addWidget(tabCHxIset[i], 1, 1, 1, 1);*/
        qhbMeasurements->addLayout(Igrid);
        //end IMON and ISET
        qhbMeasurements->addStretch();
        //begin kill button
        tabCHxKill[i] = new QPushButton("KILL");
        tabCHxKill[i]->setFixedWidth(40);
        //palette = tabCHxKill[i]->palette();
        //palette.setColor(QPalette::ButtonText, Qt::red);
        tabCHxKill[i]->setPalette(palette);
        tabCHxKill[i]->setEnabled(false);
        qhbMeasurements->addWidget(tabCHxKill[i]);
        //end kill
        channelMeasurementsBox->setLayout(qhbMeasurements);
        qvb->addWidget(channelMeasurementsBox);


        //create channel settings box
        QGroupBox *channelSettingsBox = new QGroupBox("Channel settings");
        QVBoxLayout *qvbSettings = new QVBoxLayout();
        QHBoxLayout *qhbSettings1 = new QHBoxLayout();
        QHBoxLayout *qhbSettings2 = new QHBoxLayout();
      //  tabSTATUSx[i]=new QLabel(".");
      //  qhbSettings->addWidget(tabSTATUSx[i]);
       //qhbSettings->addStretch();
        channelSettingsBox->setLayout(qvbSettings);
        qvbSettings->addLayout(qhbSettings1);
        qvbSettings->addLayout(qhbSettings2);
        channelSettingsBox->setLayout(qhbSettings1);
        // begin VMAX
        QLabel *VMAXLabel;
        VMAXLabel = new QLabel("VMAX [V]:");
        qhbSettings1->addWidget(VMAXLabel);
        tabCHxVMAX[i] = new QLabel();
        tabCHxVMAX[i]->setFixedWidth(40);
        QFont fontMAX = tabCHxVMAX[i]->font();
        //font.setPointSize(72);
        fontMAX.setBold(true);
        tabCHxVMAX[i]->setFont(fontMAX);
        tabCHxVMAX[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        QString valMAX;
        valMAX.sprintf("%6.1lf", 0.0);
        tabCHxVMAX[i]->setText(valMAX);
        qhbSettings1->addWidget(tabCHxVMAX[i]);
        tabCHxVMAX[i]->setEnabled(true);
        //end VMAX
        //begin setVMAX
        tabCHxSetVMAX[i] = new QPushButton("Set VMAX");
       // tabCHxSetVMAX[i]->setFixedWidth(70);
        tabCHxSetVMAX[i]->setEnabled(false);
        qhbSettings1->addWidget(tabCHxSetVMAX[i]);
        connect(tabCHxSetVMAX[i], SIGNAL(pressed()), this, SLOT(setVMAXPressed()));
        //end setVMAX
        qhbSettings1->addStretch();
        // begin RUP
        QLabel *RUPLabel;
        RUPLabel = new QLabel("Ramp Up [V/s]:");
        qhbSettings1->addWidget(RUPLabel);
        tabCHxRUP[i] = new QLabel("");
        tabCHxRUP[i]->setFixedWidth(40);
        QFont fontRUP = tabCHxRUP[i]->font();
        //font.setPointSize(72);
        fontRUP.setBold(true);
        tabCHxRUP[i]->setFont(fontRUP);
        tabCHxRUP[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        QString valRUP;
        valRUP.sprintf("%6.1lf", 0.0);
        tabCHxRUP[i]->setText(valRUP);
        qhbSettings1->addWidget(tabCHxRUP[i]);
        tabCHxRUP[i]->setEnabled(true);
        //end RUP
        //begin setRUP
        tabCHxSetRUP[i] = new QPushButton("Set RUP");
       // tabCHxSetRUP[i]->setFixedWidth(70);
        tabCHxSetRUP[i]->setEnabled(false);
        qhbSettings1->addWidget(tabCHxSetRUP[i]);
        connect(tabCHxSetRUP[i], SIGNAL(pressed()), this, SLOT(setRUPPressed()));
        //end setRUP
        // begin Iset
        QLabel *IsetLabel;
        IsetLabel = new QLabel("Iset [A]:\t ");
        qhbSettings2->addWidget(IsetLabel);
        tabCHxIset[i] = new QLabel();
        tabCHxIset[i]->setFixedWidth(40);
        QFont fontIset = tabCHxIset[i]->font();
        //font.setPointSize(72);
        fontIset.setBold(true);
        tabCHxIset[i]->setFont(fontMAX);
        tabCHxIset[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        QString valIset;
        valIset.sprintf("%6.1lf", 0.0);
        tabCHxIset[i]->setText(valIset);
        qhbSettings2->addWidget(tabCHxIset[i]);
        tabCHxIset[i]->setEnabled(true);
        //end Iset
        //begin setIset
        tabCHxSetIset[i] = new QPushButton("Set current");
       // tabCHxSetIset[i]->setFixedWidth(70);
        tabCHxSetVMAX[i]->setEnabled(false);
        qhbSettings2->addWidget(tabCHxSetIset[i]);
        connect(tabCHxSetIset[i], SIGNAL(pressed()), this, SLOT(setCurrentPressed()));
        //end setIset

        qhbSettings2->addStretch();
        // begin RDWN
        QLabel *RDWNLabel;
        RDWNLabel = new QLabel("Ramp Down [V/s]:");
        qhbSettings2->addWidget(RDWNLabel);
        tabCHxRDWN[i] = new QLabel("");
        tabCHxRDWN[i]->setFixedWidth(40);
        QFont fontRDWN = tabCHxRDWN[i]->font();
        //font.setPointSize(72);
        fontRDWN.setBold(true);
        tabCHxRDWN[i]->setFont(fontRDWN);
        tabCHxRDWN[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        QString valRDWN;
        valRDWN.sprintf("%6.1lf", 0.0);
        tabCHxRDWN[i]->setText(valRDWN);
        qhbSettings2->addWidget(tabCHxRDWN[i]);
        tabCHxRDWN[i]->setEnabled(true);
        //end RDWN
        //begin setRDWN
        tabCHxSetRDWN[i] = new QPushButton("Set RDWN");
       // tabCHxSetRDWN[i]->setFixedWidth(70);
        tabCHxSetRDWN[i]->setEnabled(false);
        qhbSettings2->addWidget(tabCHxSetRDWN[i]);
        connect(tabCHxSetRDWN[i], SIGNAL(pressed()), this, SLOT(setRDWNPressed()));

        //end setRDWN



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



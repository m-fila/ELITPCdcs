#include "lvpsuwidget.h"
#include "ui_lvpsuwidget.h"
//#include "src/ConnectionParameters.h"
#include <iostream>
#include <QSettings>
#include <QInputDialog>

LVpsuWidget::LVpsuWidget(std::string name, QWidget *parent) : AbstractWidget(name, parent), ui(new Ui::LVpsuWidget)
{
    ui->setupUi(this);
    ui->tcpLayout->addWidget(tcp);
    loadConfig();
    setChannelsNames();
    LVController=new lv_controller(instanceName);
    connectSignals();
}
LVpsuWidget::LVpsuWidget(std::string name, std::string address, std::string port, QWidget *parent): LVpsuWidget(name,parent){
    if(address.size()){
        tcp->setIP(address); 
    }
    if(port.size()){
        tcp->setPort(port);
    }
}
LVpsuWidget::~LVpsuWidget()
{
    delete ui;
    delete LVController;
}

void LVpsuWidget::connectSignals(){
    AbstractWidget::connectSignals();
    connect(LVController,SIGNAL(statusChanged(void*)),this,SLOT(updateStatus(void*)));
    connect(LVController,SIGNAL(measurementsChanged(void*)),this,SLOT(updateMeasurements(void*)));
    connect(LVController,SIGNAL(configurationChanged(void*)),this,SLOT(updateConfiguration(void*)));

    connect(ui->CH1on,SIGNAL(clicked(bool)),this, SLOT(setCH1ON()));
    connect(ui->CH2on,SIGNAL(clicked(bool)),this, SLOT(setCH2ON()));
    connect(ui->CH1off,SIGNAL(clicked(bool)),this, SLOT(setCH1OFF()));
    connect(ui->CH2off,SIGNAL(clicked(bool)),this, SLOT(setCH2OFF()));
    connect(ui->outputON,SIGNAL(clicked(bool)),this, SLOT(setOutputON()));
    connect(ui->outputOFF,SIGNAL(clicked(bool)),this, SLOT(setOutputOFF()));

    connect(ui->setName1, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName2, SIGNAL(pressed()), this, SLOT(changeNamePressed()));

    connect(ui->CH1confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH2confVSet, SIGNAL(pressed()), this, SLOT(setVPressed()));
    connect(ui->CH1confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));
    connect(ui->CH2confISet, SIGNAL(pressed()), this, SLOT(setIPressed()));

}

void LVpsuWidget::controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp){
     LVController->opcInit(client,config,resp);
}


void LVpsuWidget::updateStatus(void* data){
    AbstractWidget::updateStatus(data);
    bool isConnected=*static_cast<bool*>(data);
    connectionState=isConnected;
    if(isConnected){
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
    }
    else{

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

void LVpsuWidget::updateMeasurements(void* data){
    UA_HMPm measurements=*static_cast<UA_HMPm*>(data);
    if(measurements.voltageSize){
        ui->CH1voltage->display(measurements.voltage[0]);
        ui->CH1current->display(measurements.current[0]);
        ui->CH1on->setEnabled(!measurements.ch[0] && connectionState);
        ui->CH1off->setEnabled(measurements.ch[0] && connectionState);
        ui->CH2voltage->display(measurements.voltage[1]);
        ui->CH2current->display(measurements.current[1]);
        ui->CH2on->setEnabled(!measurements.ch[1] && connectionState);
        ui->CH2off->setEnabled(measurements.ch[1] && connectionState);

        ui->outputON->setEnabled(!measurements.output && connectionState);
        ui->outputOFF->setEnabled(measurements.output && connectionState);
        deviceSettings.CH1=measurements.ch[0];
        deviceSettings.CH2=measurements.ch[1];
        deviceSettings.Output=measurements.output;
    }
}
void LVpsuWidget::updateConfiguration(void* data){
    UA_HMPc configuration=*static_cast<UA_HMPc*>(data);
    if(configuration.voltageSetSize){
        ui->CH1volatgeSet->display(configuration.voltageSet[0]);
        ui->CH1currentSet->display(configuration.currentSet[0]);
        ui->CH2voltageSet->display(configuration.voltageSet[1]);
        ui->CH2currentSet->display(configuration.currentSet[1]);
        ui->CH1confV->display(configuration.voltageSet[0]);
        ui->CH1confI->display(configuration.currentSet[0]);
        ui->CH2confV->display(configuration.voltageSet[1]);
        ui->CH2confI->display(configuration.currentSet[1]);
    }
}

void LVpsuWidget::deviceConnect()
{
    std::string IPaddress = tcp->getIP();
    int port = tcp->getPort();
    LVController->callConnect(IPaddress,port);
}
void LVpsuWidget::deviceDisconnect(){
    LVController->callDisconnect();
}

void LVpsuWidget::setCH1ON(){
    LVController->callSetChannel(1,true);
}
void LVpsuWidget::setCH2ON(){
    LVController->callSetChannel(2,true);
}
void LVpsuWidget::setCH1OFF(){
    LVController->callSetChannel(1,false);
}
void LVpsuWidget::setCH2OFF(){
    LVController->callSetChannel(2,false);
}
void LVpsuWidget::setOutputON(){
    LVController->callSetOutput(true);
}
void LVpsuWidget::setOutputOFF(){
    LVController->callSetOutput(false);
}

void LVpsuWidget::loadConfig(){
    AbstractWidget::loadConfig();
    QString configkey;
    for(int i=0; i!=2;++i){
        configkey.sprintf("%s/CustomName%i",instanceName.c_str(),i);
        customName[i]= QSettings().value(configkey).toString();
    }
}

void LVpsuWidget::saveConfig(){
    AbstractWidget::saveConfig();
    QString configkey;
    for(int i=0; i!=2;++i){
        configkey.sprintf("%s/CustomName%i",instanceName.c_str(),i);
        QSettings().setValue(configkey,customName[i]);
    }
}

void LVpsuWidget::setChannelName(int channelno)
{
    //QString title= tr("CH %1        ").arg(channelno+1);
    //title.append(customName[channelno]);
    QString title=customName[channelno];

    //set name on ALL Channles Tab
    
    QString label;
    //set name on CH x tab (... if empty)
    if(customName[channelno].isEmpty())
        label=QString("...");
    else
        label=customName[channelno];
    switch (channelno)
    {
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
void LVpsuWidget::setChannelsNames(){
    for(int i=0; i!=2; ++i)
    {
        setChannelName(i);
    }
}

void LVpsuWidget::changeNamePressed(){
    QObject* obj = sender();
    bool ok;
    int i;
    if(ui->setName1==obj){
        i=0;
    }
    else if(ui->setName2==obj){
        i=1;
    }
    QString newName = QInputDialog::getText(this, tr("Set CH %1 name").arg(i+1),
                                                     tr("CH %1 name:").arg(i+1), QLineEdit::Normal, customName[i], &ok);
    if(ok){
        customName[i] = newName;
        setChannelName(i);
    }
}

void LVpsuWidget::setVPressed(){
    QObject* obj = sender();
    bool ok;
    int i;
    double val;
    if(ui->CH1confVSet==obj){
        i=0;
        val=ui->CH1volatgeSet->value();
    }
    else if(ui->CH2confVSet==obj){
        i=1;
        val=ui->CH2voltageSet->value();
    }
    QString label;
    if(customName[i].isEmpty())
        label = tr("CH %1  [Volts]:").arg(i+1);
    else
        label = tr("CH %1  \"%2\"  [Volts]:").arg(i+1).arg(customName[i]);
    double d = QInputDialog::getDouble(this, tr("Set CH %1 V").arg(i+1),
                                          label, val, 0, 20, 1, &ok);
    if(ok){
        LVController->callSetVoltage(i+1,d);
    }
}

void LVpsuWidget::setIPressed(){
    QObject* obj = sender();
    bool ok;
    int i;
    double val;
    if(ui->CH1confISet==obj){
        i=0;
        val=ui->CH1currentSet->value();
    }
    else if(ui->CH2confISet==obj){
        i=1;
        val=ui->CH2currentSet->value();
    }
    QString label;
    if(customName[i].isEmpty())
        label = tr("CH %1  [Ampers]:").arg(i+1);
    else
        label = tr("CH %1  \"%2\"  [Ampers]:").arg(i+1).arg(customName[i]);
    double d = QInputDialog::getDouble(this, tr("Set CH %1 I").arg(i+1),
                                          label, val, 0, 10, 1, &ok);
    if(ok){
        LVController->callSetCurrent(i+1,d);
    }
}
#include "include/lv4psuwidget.h"
#include "ui_lv4psuwidget.h"
//#include "src/ConnectionParameters.h"
#include <iostream>
#include <QSettings>

LV4psuWidget::LV4psuWidget(std::string name, QWidget *parent) : AbstractWidget(name, parent), ui(new Ui::LV4psuWidget)
{
    ui->setupUi(this);

    ui->connectionIP->setText(QSettings().value("LVpsuIP").toString());
    ui->connectionPort->setText(QSettings().value("LVpsuPort").toString());
    LVController=new lv_controller(instanceName);
    connectSignals();
}
LV4psuWidget::LV4psuWidget(std::string name, std::string address, std::string port, QWidget *parent): LV4psuWidget(name,parent){
    if(address.size()!=0 && port.size()!=0){
        ui->connectionIP->setText(QString::fromStdString(address));
        ui->connectionPort->setText(QString::fromStdString(port));
    }
}
LV4psuWidget::~LV4psuWidget()
{
    delete ui;
    delete LVController;
}

void LV4psuWidget::connectSignals(){
    connect(ui->connect, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
    connect(ui->disconnect, SIGNAL(clicked(bool)), this, SLOT(deviceDisconnect()));
    connect(LVController,SIGNAL(statusChanged(void*)),this,SLOT(updateStatus(void*)));
    connect(LVController,SIGNAL(measurementsChanged(void*)),this,SLOT(updateMeasurements(void*)));
    connect(LVController,SIGNAL(configurationChanged(void*)),this,SLOT(updateConfiguration(void*)));

    connect(ui->CH1on,SIGNAL(clicked(bool)),this, SLOT(setCH1ON()));
    connect(ui->CH2on,SIGNAL(clicked(bool)),this, SLOT(setCH2ON()));
    connect(ui->CH3on,SIGNAL(clicked(bool)),this, SLOT(setCH3ON()));
    connect(ui->CH4on,SIGNAL(clicked(bool)),this, SLOT(setCH4ON()));
    connect(ui->CH1off,SIGNAL(clicked(bool)),this, SLOT(setCH1OFF()));
    connect(ui->CH2off,SIGNAL(clicked(bool)),this, SLOT(setCH2OFF()));
    connect(ui->CH3off,SIGNAL(clicked(bool)),this, SLOT(setCH3OFF()));
    connect(ui->CH4off,SIGNAL(clicked(bool)),this, SLOT(setCH4OFF()));
    connect(ui->outputON,SIGNAL(clicked(bool)),this, SLOT(setOutputON()));
    connect(ui->outputOFF,SIGNAL(clicked(bool)),this, SLOT(setOutputOFF()));
}

void LV4psuWidget::controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp){
     LVController->opcInit(client,config,resp);
}


void LV4psuWidget::updateStatus(void* data){
    bool isConnected=*static_cast<bool*>(data);
    connectionState=isConnected;
    if(isConnected){
        ui->connect->setEnabled(false);
        ui->disconnect->setEnabled(true);
        ui->connectionStatus->setText("CONNECTED");
        QPalette palette = ui->connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        ui->connectionStatus->setPalette(palette);
        ui->connectionIP->setEnabled(false);
        ui->connectionPort->setEnabled(false);
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
    }
    else{
        ui->connect->setEnabled(true);
        ui->disconnect->setEnabled(false);
        ui->connectionStatus->setText("DISCONNECTED");
     //   ui->statusLabel->setText("...");
        QPalette palette = ui->connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->connectionStatus->setPalette(palette);

        ui->connectionIP->setEnabled(true);
        ui->connectionPort->setEnabled(true);

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
    }

}

void LV4psuWidget::updateMeasurements(void* data){
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
        deviceSettings.CH1=measurements.ch[0];
        deviceSettings.CH2=measurements.ch[1];
        deviceSettings.CH3=measurements.ch[2];
        deviceSettings.CH4=measurements.ch[3];
        deviceSettings.Output=measurements.output;
    }
}
void LV4psuWidget::updateConfiguration(void* data){
    UA_HMPc configuration=*static_cast<UA_HMPc*>(data);
    if(configuration.voltageSetSize){
        ui->CH1voltageSet->display(configuration.voltageSet[0]);
        ui->CH1currentSet->display(configuration.currentSet[0]);
        ui->CH2voltageSet->display(configuration.voltageSet[1]);
        ui->CH2currentSet->display(configuration.currentSet[1]);
        ui->CH3voltageSet->display(configuration.voltageSet[2]);
        ui->CH3currentSet->display(configuration.currentSet[2]);
        ui->CH4voltageSet->display(configuration.voltageSet[3]);
        ui->CH4currentSet->display(configuration.currentSet[3]);
    }
}

void LV4psuWidget::deviceConnect()
{
    std::string IPaddress = ui->connectionIP->text().toStdString();
    int port = ui->connectionPort->text().toInt();
    LVController->callConnect(IPaddress,port);
}
void LV4psuWidget::deviceDisconnect(){
    LVController->callDisconnect();
}

void LV4psuWidget::setCH1ON(){
    LVController->callSetChannel(1,true);
}
void LV4psuWidget::setCH2ON(){
    LVController->callSetChannel(2,true);
}
void LV4psuWidget::setCH3ON(){
    LVController->callSetChannel(3,true);
}
void LV4psuWidget::setCH4ON(){
    LVController->callSetChannel(4,true);
}
void LV4psuWidget::setCH1OFF(){
    LVController->callSetChannel(1,false);
}
void LV4psuWidget::setCH2OFF(){
    LVController->callSetChannel(2,false);
}
void LV4psuWidget::setCH3OFF(){
    LVController->callSetChannel(3,false);
}
void LV4psuWidget::setCH4OFF(){
    LVController->callSetChannel(4,false);
}
void LV4psuWidget::setOutputON(){
    LVController->callSetOutput(true);
}
void LV4psuWidget::setOutputOFF(){
    LVController->callSetOutput(false);
}


void LV4psuWidget::closeEvent(QCloseEvent* e)
{
    QSettings().setValue("LVpsuIP",ui->connectionIP->text());
    QSettings().setValue("LVpsuPort",ui->connectionPort->text());

    QWidget::closeEvent(e);
}

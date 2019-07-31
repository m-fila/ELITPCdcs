#include "include/lvpsuwidget.h"
#include "ui_lvpsuwidget.h"
//#include "src/ConnectionParameters.h"
#include <iostream>
#include <QSettings>

LVpsuWidget::LVpsuWidget(std::string name, QWidget *parent) : AbstractWidget(name, parent), ui(new Ui::LVpsuWidget)
{
    ui->setupUi(this);

    ui->connectionIP->setText(QSettings().value("LVpsuIP").toString());
    ui->connectionPort->setText(QSettings().value("LVpsuPort").toString());
    LVController=new lv_controller(instanceName);
    connectSignals();
}
LVpsuWidget::LVpsuWidget(std::string name, std::string address, std::string port, QWidget *parent): LVpsuWidget(name,parent){
    if(address.size()!=0 && port.size()!=0){
        ui->connectionIP->setText(QString::fromStdString(address));
        ui->connectionPort->setText(QString::fromStdString(port));
    }
}
LVpsuWidget::~LVpsuWidget()
{
    delete ui;
    delete LVController;
}

void LVpsuWidget::connectSignals(){
    connect(ui->connect, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
    connect(ui->disconnect, SIGNAL(clicked(bool)), this, SLOT(deviceDisconnect()));
    connect(LVController,SIGNAL(statusChanged(void*)),this,SLOT(updateStatus(void*)));
    connect(LVController,SIGNAL(measurementsChanged(void*)),this,SLOT(updateMeasurements(void*)));
    connect(LVController,SIGNAL(configurationChanged(void*)),this,SLOT(updateConfiguration(void*)));

    connect(ui->CH1on,SIGNAL(clicked(bool)),this, SLOT(setCH1ON()));
    connect(ui->CH2on,SIGNAL(clicked(bool)),this, SLOT(setCH2ON()));
    connect(ui->CH1off,SIGNAL(clicked(bool)),this, SLOT(setCH1OFF()));
    connect(ui->CH2off,SIGNAL(clicked(bool)),this, SLOT(setCH2OFF()));
    connect(ui->outputON,SIGNAL(clicked(bool)),this, SLOT(setOutputON()));
    connect(ui->outputOFF,SIGNAL(clicked(bool)),this, SLOT(setOutputOFF()));
}

void LVpsuWidget::controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp){
     LVController->opcInit(client,config,resp);
}


void LVpsuWidget::updateStatus(void* data){
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
        ui->outputON->setEnabled(false);
        ui->outputOFF->setEnabled(false);

        ui->CH1voltage->display(0);
        ui->CH1current->display(0);
        ui->CH2voltage->display(0);
        ui->CH2current->display(0);
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
        ui->CH2currentSet->display(configuration.voltageSet[1]);
    }
}

void LVpsuWidget::deviceConnect()
{
    std::string IPaddress = ui->connectionIP->text().toStdString();
    int port = ui->connectionPort->text().toInt();
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


void LVpsuWidget::closeEvent(QCloseEvent* e)
{
    QSettings().setValue("LVpsuIP",ui->connectionIP->text());
    QSettings().setValue("LVpsuPort",ui->connectionPort->text());

    QWidget::closeEvent(e);
}

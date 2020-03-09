#include "piweatherwidget.h"
#include "ui_piweather.h"
//#include "src/ConnectionParameters.h"
#include <iostream>
#include <QSettings>
#include <QInputDialog>

PiWeatherWidget::PiWeatherWidget(std::string name, QWidget *parent) : AbstractWidget(name, parent), ui(new Ui::PiWeatherWidget)
{
    ui->setupUi(this);
    loadConfig();
    setChannelsNames();
    controller=new piweather_controller(instanceName);
    connectSignals();
}
PiWeatherWidget::PiWeatherWidget(std::string name, std::string address, std::string port, QWidget *parent): PiWeatherWidget(name,parent){
    if(address.size()!=0 && port.size()!=0){
        ui->connectionIP->setText(QString::fromStdString(address));
        ui->connectionPort->setText(QString::fromStdString(port));
    }
}
PiWeatherWidget::~PiWeatherWidget()
{
    delete ui;
    delete controller;
}

void PiWeatherWidget::connectSignals(){
    connect(ui->connect, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
    connect(ui->disconnect, SIGNAL(clicked(bool)), this, SLOT(deviceDisconnect()));
    connect(controller,SIGNAL(statusChanged(void*)),this,SLOT(updateStatus(void*)));
    connect(controller,SIGNAL(measurementsChanged(void*)),this,SLOT(updateMeasurements(void*)));
    connect(controller,SIGNAL(configurationChanged(void*)),this,SLOT(updateConfiguration(void*)));

    connect(ui->setName1, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName2, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName3, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    connect(ui->setName4, SIGNAL(pressed()), this, SLOT(changeNamePressed()));

}

void PiWeatherWidget::controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp){
     controller->opcInit(client,config,resp);
}


void PiWeatherWidget::updateStatus(void* data){
    AbstractWidget::updateStatus(data);
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

        ui->tempDisplay1->display(0);
        ui->tempDisplay2->display(0);
        ui->tempDisplay3->display(0);
        ui->temperatureDisplay->display(0);
        ui->pressureDisplay->display(0);
    }

}

void PiWeatherWidget::updateMeasurements(void* data){
    UA_PiWeatherm measurements=*static_cast<UA_PiWeatherm*>(data);
    if(measurements.tempSize){
        ui->tempDisplay1->display(measurements.temp[0]);
        ui->tempDisplay2->display(measurements.temp[1]);
        ui->tempDisplay3->display(measurements.temp[2]);
        ui->temperatureDisplay->display(measurements.temperature);
        ui->pressureDisplay->display(measurements.pressure);
    }
}
void PiWeatherWidget::updateConfiguration(void* data){
    UA_PiWeatherc configuration=*static_cast<UA_PiWeatherc*>(data);
}

void PiWeatherWidget::deviceConnect()
{
    std::string IPaddress = ui->connectionIP->text().toStdString();
    int port = ui->connectionPort->text().toInt();
    controller->callConnect(IPaddress,port);
}
void PiWeatherWidget::deviceDisconnect(){
    controller->callDisconnect();
}

void PiWeatherWidget::loadConfig(){
    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    ui->connectionIP->setText(QSettings().value(IP.c_str()).toString());
    ui->connectionPort->setText(QSettings().value(Port.c_str()).toString());

    QString configkey;
    for(int i=0; i!=4;++i){
        configkey.sprintf("%s/CustomName%i",instanceName.c_str(),i);
        customName[i]= QSettings().value(configkey).toString();
    }
}

void PiWeatherWidget::saveConfig(){
    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    //save settings
    QSettings().setValue(IP.c_str(),ui->connectionIP->text());
    QSettings().setValue(Port.c_str(),ui->connectionPort->text());
    
    QString configkey;
    for(int i=0; i!=4;++i){
        configkey.sprintf("%s/CustomName%i",instanceName.c_str(),i);
        QSettings().setValue(configkey,customName[i]);
    }
}

void PiWeatherWidget::setChannelName(int channelno)
{
    QString title= tr("Temperature %1     ").arg(channelno+1);
    title.append(customName[channelno]);
    //QString title=customName[channelno];

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
    case 2:
        ui->groupBox3->setTitle(title);
        ui->customNameLabel3->setText(label);
        break;
    case 3:
        title= "Temperature && pressure     ";
        title.append(customName[channelno]);
        ui->groupBox4->setTitle(title);
        ui->customNameLabel4->setText(label);
        break;    
    default:
        break;
    }
}
void PiWeatherWidget::setChannelsNames(){
    for(int i=0; i!=4; ++i)
    {
        setChannelName(i);
    }
}

void PiWeatherWidget::changeNamePressed(){
    QObject* obj = sender();
    bool ok;
    int i;

    if(ui->setName1==obj){
        i=0;
    }
    else if(ui->setName2==obj){
        i=1;
    }
    else if(ui->setName3==obj){
        i=2;
    }
    else if(ui->setName4==obj){
        i=3;
    }
    QString newName = QInputDialog::getText(this, tr("Set CH %1 name").arg(i+1),
                                                     tr("CH %1 name:").arg(i+1), QLineEdit::Normal, customName[i], &ok);
    if(ok){
        customName[i] = newName;
        setChannelName(i);
    }
}

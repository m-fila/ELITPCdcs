#include "include/tpgwidget.h"
#include <string>
#include <QSettings>

TPGWidget::TPGWidget(QWidget *parent) : QWidget(parent)
{
    createLayout();
    controller = new tpg_controller("");
    connectSignals();

}

TPGWidget::TPGWidget(const char *name) : QWidget()
{   createLayout();
    instanceName = name;
    controller = new tpg_controller(instanceName);
    connectSignals();

    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    connectionIP->setText(QSettings().value(IP.c_str()).toString());
    connectionPort->setText(QSettings().value(Port.c_str()).toString());
}

TPGWidget::~TPGWidget()
{
    //delete ui;
    delete controller;
}

void TPGWidget::connectSignals()
{
    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
    connect(disconnectButton, SIGNAL(clicked(bool)), this, SLOT(deviceDisconnect()));
    connect(controller,SIGNAL(statusChanged(void*)),this,SLOT(updateStatus(void*)));
    connect(controller,SIGNAL(measurementsChanged(void*)),this,SLOT(updateMeasurements(void*)));
    connect(controller,SIGNAL(configurationChanged(void*)),this,SLOT(updateConfiguration(void*)));

}

void TPGWidget::deviceConnect()
{
    std::string IPaddress = connectionIP->text().toStdString();
    int port = connectionPort->text().toInt();
    controller->callConnect(IPaddress,port);
}

void TPGWidget::deviceDisconnect(){
    controller->callDisconnect();
}

void TPGWidget::updateStatus(void *data){
    bool isConnected=*static_cast<bool*>(data);
    connectionState=isConnected;
    if(isConnected){
        connectButton->setEnabled(false);
        disconnectButton->setEnabled(true);
        connectionStatus->setText("CONNECTED");
        QPalette palette = connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        connectionStatus->setPalette(palette);
        connectionIP->setEnabled(false);
        connectionPort->setEnabled(false);

    }
    else{
        connectButton->setEnabled(true);
        disconnectButton->setEnabled(false);
        connectionStatus->setText("DISCONNECTED");
        statusLabel->setText("...");
        QPalette palette = connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        connectionStatus->setPalette(palette);
        connectionIP->setEnabled(true);
        connectionPort->setEnabled(true);
    }
}
void TPGWidget::updateMeasurements(void *data){

}
void TPGWidget::updateConfiguration(void *data){

}

void TPGWidget::updateStatusLabel(QString info){
    statusLabel->setText(info);
}

void TPGWidget::closeEvent(QCloseEvent* e)
{
    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    //save settings
    QSettings().setValue(IP.c_str(),connectionIP->text());
    QSettings().setValue(Port.c_str(),connectionPort->text());
    QWidget::closeEvent(e);
}

void TPGWidget::createLayout()
{
    //create main layout with base size
    mainLayout = new QVBoxLayout();
    resize(400,350);

    createConnectionSection();
    drawLine();

    //todo ADD DISPLAY HERE

    mainLayout->addStretch();
    drawLine();
    statusLabel = new QLabel("...");
    mainLayout->addWidget(statusLabel);

    //set main layout at the end
    setLayout(mainLayout);
}

void TPGWidget::drawLine()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
}

void TPGWidget::createConnectionSection()
{
    //Connection status
    QLabel *connectionStatusLabel = new QLabel("Connection status: ");
    connectionStatus = new QLabel("DISCONNECTED");
    QPalette palette = connectionStatus->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    connectionStatus->setPalette(palette);

    QHBoxLayout *hb1 = new QHBoxLayout;
    hb1->addWidget(connectionStatusLabel);
    hb1->addWidget(connectionStatus);
    hb1->addStretch();

    mainLayout->addLayout(hb1);

    //Connecion parameters and buttons
    QVBoxLayout *vbIP = new QVBoxLayout();
    QLabel *connectionIPLabel = new QLabel("IP:");
    connectionIP = new QLineEdit();
    vbIP->addWidget(connectionIPLabel);
    vbIP->addWidget(connectionIP);

    QVBoxLayout *vbPort = new QVBoxLayout();
    QLabel *connectionPortLabel = new QLabel("Port:");
    connectionPort = new QLineEdit();
    vbPort->addWidget(connectionPortLabel);
    vbPort->addWidget(connectionPort);

    QVBoxLayout *vbConnect = new QVBoxLayout();
    connectButton = new QPushButton("Connect");
    disconnectButton = new QPushButton("Disconnect");
    disconnectButton->setEnabled(false);
    vbConnect->addWidget(connectButton);
    vbConnect->addWidget(disconnectButton);

    QHBoxLayout *hb2 = new QHBoxLayout;
    hb2->addLayout(vbIP);
    hb2->addLayout(vbPort);
    hb2->addLayout(vbConnect);

    mainLayout->addLayout(hb2);
}



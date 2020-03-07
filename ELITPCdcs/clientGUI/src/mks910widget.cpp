#include "mks910widget.h"
#include <string>
#include <QSettings>
#include <QInputDialog>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

MKS910Widget::MKS910Widget(std::string name,QWidget *parent) : AbstractWidget(name,parent)
{   createLayout();
//    instanceName = name;
    controller = new MKS910_controller(instanceName);
    connectSignals();
    loadConfig();
    setChannelsNames();
    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    connectionIP->setText(QSettings().value(IP.c_str()).toString());
    connectionPort->setText(QSettings().value(Port.c_str()).toString());
}
MKS910Widget::MKS910Widget(std::string name, std::string address, std::string port, QWidget *parent): MKS910Widget(name,parent){
    if(address.size()){
        connectionIP->setText(QString::fromStdString(address));
    }
    if(port.size()){
        connectionPort->setText(QString::fromStdString(port));
    }
}

MKS910Widget::~MKS910Widget()
{
    //delete ui;
    delete controller;
}

void MKS910Widget::connectSignals()
{
    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
    connect(disconnectButton, SIGNAL(clicked(bool)), this, SLOT(deviceDisconnect()));
    connect(controller,SIGNAL(statusChanged(void*)),this,SLOT(updateStatus(void*)));
    connect(controller,SIGNAL(measurementsChanged(void*)),this,SLOT(updateMeasurements(void*)));
    connect(controller,SIGNAL(configurationChanged(void*)),this,SLOT(updateConfiguration(void*)));

}
void MKS910Widget::controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp){
     controller->opcInit(client,config,resp);
}

void MKS910Widget::deviceConnect()
{
    std::string IPaddress = connectionIP->text().toStdString();
    int port = connectionPort->text().toInt();
    controller->callConnect(IPaddress,port);
}

void MKS910Widget::deviceDisconnect(){
    controller->callDisconnect();
}

void MKS910Widget::updateStatus(void *data){
    AbstractWidget::updateStatus(data);
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
        mStatus->setText("");
           mVacuum->display(0);
    }
}
void MKS910Widget::updateMeasurements(void *data){
    if(connectionState){
    UA_MKS910m measurements=*static_cast<UA_MKS910m*>(data);
    QString val;
    std::string s;
    std::ostringstream os;
    os<<std::scientific<<std::setprecision(5)<<std::uppercase<<measurements.vacuum;
    s=os.str();
    s.insert(s.size()-4," ");
    s.erase(std::remove(s.begin(), s.end(), '+'), s.end());
    val=QString::fromStdString(s);
    mVacuum->display(val);

   MKS910codes::Units units=static_cast<MKS910codes::Units>(measurements.units);
   val=QString::fromStdString(MKS910codes::unitsToString.at(units));
   mUnitLabel->setText(val);
   MKS910codes::Status status=static_cast<MKS910codes::Status>(measurements.status);
   if(status==MKS910codes::Status::O){
       val=QString::fromStdString("OK");
   }
   else{
       val=QString::fromStdString(MKS910codes::statusToString.at(status)+"fail");
   }
   mStatus->setText(val);
 //   val=QString::fromStdString(MKS910codes::statusToString.at(stat));
 //   mStatus->setText(val);
}
}
void MKS910Widget::updateConfiguration(void *data){

}

void MKS910Widget::updateStatusLabel(QString info){
    statusLabel->setText(info);
}

void MKS910Widget::closeEvent(QCloseEvent* e)
{
    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    //save settings
    QSettings().setValue(IP.c_str(),connectionIP->text());
    QSettings().setValue(Port.c_str(),connectionPort->text());

    saveConfig();
    QWidget::closeEvent(e);
}

void MKS910Widget::createLayout(){
    //create main layout with base size
    mainLayout = new QVBoxLayout();
    resize(400,350);

    createConnectionSection();
    //drawLine();

    //todo ADD DISPLAY HERE
    tab=new QTabWidget();
    mainLayout->addWidget(tab);

    createMTab();
    createHTab();
    createCTab();
    mainLayout->addStretch();
 //   drawLine();
    statusLabel = new QLabel("...");
    mainLayout->addWidget(statusLabel);

    //set main layout at the end
    setLayout(mainLayout);
}
void MKS910Widget::createMTab(){
    QWidget* mWidget=new QWidget();
    tab->addTab(mWidget,"Measurements");
    QVBoxLayout *mLayout=new QVBoxLayout();
    mWidget->setLayout(mLayout);    
   // mainLayout->addStretch();
        mBox= new QGroupBox("Pressure");
        QHBoxLayout* mhLayout= new QHBoxLayout();
        QVBoxLayout* mvLayout= new QVBoxLayout();
        mBox->setLayout(mvLayout);
        mvLayout->addLayout(mhLayout);
        mVacuum=new QLCDNumber();
        mVacuum->setDigitCount(12);
        mVacuum->setSegmentStyle(QLCDNumber::Flat);
        mVacuum->setMinimumSize(QSize(200,50));
        QPalette palette = mVacuum->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        mVacuum->setPalette(palette);
        mVacuum->display(0.00);
        mStatusLabel=new QLabel("Status: ");
        mStatusLabel->setAlignment(Qt::AlignLeft);//|Qt::AlignCenter);
        mStatus=new QLabel(".");
        mStatus->setAlignment(Qt::AlignLeft);
        QFont statusFont=mStatus->font();
        statusFont.setBold(true);
        mStatus->setFont(statusFont);
        mUnitLabel=new QLabel("mbar");
        mUnitLabel->setAlignment(Qt::AlignRight);
        mhLayout->addWidget(mStatusLabel);
        mhLayout->addWidget(mStatus);
        mhLayout->addWidget(mUnitLabel);
        QHBoxLayout* mhhLayout= new QHBoxLayout();
        mvLayout->addLayout(mhhLayout);
       // mhhLayout->addStretch();
        mhhLayout->addWidget(mVacuum);
        
        mLayout->addWidget(mBox);
        mLayout->addStretch();
    
}
void MKS910Widget::createCTab(){
    QWidget* cWidget=new QWidget();
    tab->addTab(cWidget,"Configuration");
    QVBoxLayout *cLayout=new QVBoxLayout();
    cWidget->setLayout(cLayout); 

        QGroupBox* cBox= new QGroupBox("CH ");
        cLayout->addWidget(cBox);
        QHBoxLayout* chLayout= new QHBoxLayout();
        cBox->setLayout(chLayout);
        QLabel* clabel=new QLabel("Custom name:");
        cNameLabel=new QLabel("...");
        cNameButton=new QPushButton("Change name");
        connect(cNameButton, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
        chLayout->addWidget(clabel);
        chLayout->addWidget(cNameLabel);
        chLayout->addWidget(cNameButton);

}
void MKS910Widget::createHTab(){
    QWidget* hWidget=new QWidget();
    tab->addTab(hWidget,"Historical");
    QVBoxLayout *hLayout=new QVBoxLayout();
    hWidget->setLayout(hLayout); 
}

void MKS910Widget::drawLine()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
}

void MKS910Widget::createConnectionSection()
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

void MKS910Widget::changeNamePressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<2; i++)
    {
        if(cNameButton == obj)
        {
            QString newName = QInputDialog::getText(this, tr("Set CH %1 name").arg(i+1),
                                                     tr("CH %1 name:").arg(i+1), QLineEdit::Normal, cCustomName, &ok);
            if(ok)
            {
                cCustomName = newName;
                setChannelName(i);
            }
        }
    }
}

void MKS910Widget::setChannelName(int channelno)
{
    QString title;
    title = tr("CH %1        ").arg(channelno+1);
    title.append(cCustomName);
    mBox->setTitle(title);
    //set name on CH x tab (... if empty)
    if(cCustomName.isEmpty())
        cNameLabel->setText("...");
    else
        cNameLabel->setText(cCustomName);

}

void MKS910Widget::setChannelsNames()
{
    int i;
    for(i=0; i!=2; ++i)
    {
        setChannelName(i);
    }
}

void MKS910Widget::loadConfig()
{
    int i;
    QString configkey;
    for(i=0; i!=2; ++i)
    {
        configkey = tr("MKS910CH%1/CustomName").arg(i);
        cCustomName = QSettings().value(configkey).toString();
    }
}

void MKS910Widget::saveConfig()
{
    int i;
    QString configkey;
    for(i=0; i!=2; ++i)
    {
        configkey = tr("MKS910CH%1/CustomName").arg(i);
        QSettings().setValue(configkey,cCustomName);
    }
}
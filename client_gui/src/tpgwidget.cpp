#include "include/tpgwidget.h"
#include <string>
#include <QSettings>
#include <QInputDialog>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

TPGWidget::TPGWidget(std::string name,QWidget *parent) : AbstractWidget(name,parent)
{   createLayout();
//    instanceName = name;
    controller = new tpg_controller(instanceName);
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
TPGWidget::TPGWidget(std::string name, std::string address, std::string port, QWidget *parent): TPGWidget(name,parent){
    if(address.size()!=0 && port.size()!=0){
        connectionIP->setText(QString::fromStdString(address));
        connectionPort->setText(QString::fromStdString(port));
    }
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
void TPGWidget::controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp){
     controller->opcInit(client,config,resp);
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

        for(int i=0;i!=2;++i){
            mVacuum[i]->display(0);
            mStatus[i]->setText("");
        }
    }
}
void TPGWidget::updateMeasurements(void *data){
    UA_TPG362m measurements=*static_cast<UA_TPG362m*>(data);
    if(measurements.statusSize){
        QString val;
        std::string s;
        for(int i=0;i!=2;++i){
            std::ostringstream os;
            os<<std::scientific<<std::setprecision(5)<<std::uppercase<<measurements.vacuum[i];
            s=os.str();
            s.insert(s.size()-4," ");
            s.erase(std::remove(s.begin(), s.end(), '+'), s.end());
            val=QString::fromStdString(s);
            mVacuum[i]->display(val);
            val=QString::fromStdString(status_names.at(measurements.status[i]));
            mStatus[i]->setText(val);

        }
    }
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

    saveConfig();
    QWidget::closeEvent(e);
}

void TPGWidget::createLayout()
{
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
void TPGWidget::createMTab(){
    QWidget* mWidget=new QWidget();
    tab->addTab(mWidget,"Measurements");
    QVBoxLayout *mLayout=new QVBoxLayout();
    mWidget->setLayout(mLayout);    
   // mainLayout->addStretch();
    for(int i=0; i!=2;++i){

        mBox[i]= new QGroupBox(("CH"+std::to_string(i+2)).c_str());
        QHBoxLayout* mhLayout= new QHBoxLayout();
        QVBoxLayout* mvLayout= new QVBoxLayout();
        mBox[i]->setLayout(mvLayout);
        mvLayout->addLayout(mhLayout);
        mVacuum[i]=new QLCDNumber();
        mVacuum[i]->setDigitCount(12);
        mVacuum[i]->setSegmentStyle(QLCDNumber::Flat);
        mVacuum[i]->setMinimumSize(QSize(200,50));
        QPalette palette = mVacuum[i]->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        mVacuum[i]->setPalette(palette);
        mVacuum[i]->display(0.00);
        mStatusLabel[i]=new QLabel("Status: ");
        mStatusLabel[i]->setAlignment(Qt::AlignLeft);//|Qt::AlignCenter);
        mStatus[i]=new QLabel(".");
        mStatus[i]->setAlignment(Qt::AlignLeft);
        QFont statusFont=mStatus[i]->font();
       // statusFont.setBold(true);
        mStatus[i]->setFont(statusFont);
        QLabel* unit=new QLabel("mbar");
        unit->setAlignment(Qt::AlignRight);
        mhLayout->addWidget(mStatusLabel[i]);
        mhLayout->addWidget(mStatus[i]);
        mhLayout->addWidget(unit);
        QHBoxLayout* mhhLayout= new QHBoxLayout();
        mvLayout->addLayout(mhhLayout);
       // mhhLayout->addStretch();
        mhhLayout->addWidget(mVacuum[i]);
        
        mLayout->addWidget(mBox[i]);
        mLayout->addStretch();
    }
    
}
void TPGWidget::createCTab(){
    QWidget* cWidget=new QWidget();
    tab->addTab(cWidget,"Configuration");
    QVBoxLayout *cLayout=new QVBoxLayout();
    cWidget->setLayout(cLayout); 
    for(int i=0; i!=2;++i){
        QGroupBox* cBox= new QGroupBox(("CH "+std::to_string(i+1)).c_str());
        cLayout->addWidget(cBox);
        QHBoxLayout* chLayout= new QHBoxLayout();
        cBox->setLayout(chLayout);
        QLabel* clabel=new QLabel("Custom name:");
        cNameLabel[i]=new QLabel("...");
        cNameButton[i]=new QPushButton("Change name");
        connect(cNameButton[i], SIGNAL(pressed()), this, SLOT(changeNamePressed()));
        chLayout->addWidget(clabel);
        chLayout->addWidget(cNameLabel[i]);
        chLayout->addWidget(cNameButton[i]);
    }

}
void TPGWidget::createHTab(){
    QWidget* hWidget=new QWidget();
    tab->addTab(hWidget,"Historical");
    QVBoxLayout *hLayout=new QVBoxLayout();
    hWidget->setLayout(hLayout); 
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

void TPGWidget::changeNamePressed()
{
    QObject* obj = sender();
    bool ok;
    int i;
    for(i=0; i<2; i++)
    {
        if(cNameButton[i] == obj)
        {
            QString newName = QInputDialog::getText(this, tr("Set CH %1 name").arg(i+1),
                                                     tr("CH %1 name:").arg(i+1), QLineEdit::Normal, cCustomName[i], &ok);
            if(ok)
            {
                cCustomName[i] = newName;
                setChannelName(i);
            }
        }
    }
}

void TPGWidget::setChannelName(int channelno)
{
    QString title;
    title = tr("CH %1        ").arg(channelno+1);
    title.append(cCustomName[channelno]);
    mBox[channelno]->setTitle(title);
    //set name on CH x tab (... if empty)
    if(cCustomName[channelno].isEmpty())
        cNameLabel[channelno]->setText("...");
    else
        cNameLabel[channelno]->setText(cCustomName[channelno]);

}

void TPGWidget::setChannelsNames()
{
    int i;
    for(i=0; i!=2; ++i)
    {
        setChannelName(i);
    }
}

void TPGWidget::loadConfig()
{
    int i;
    QString configkey;
    for(i=0; i!=2; ++i)
    {
        configkey = tr("TPG362CH%1/CustomName").arg(i);
        cCustomName[i] = QSettings().value(configkey).toString();
    }
}

void TPGWidget::saveConfig()
{
    int i;
    QString configkey;
    for(i=0; i!=2; ++i)
    {
        configkey = tr("TPG362CH%1/CustomName").arg(i);
        QSettings().setValue(configkey,cCustomName[i]);
    }
}
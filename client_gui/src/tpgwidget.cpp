#include "include/tpgwidget.h"
#include <string>
#include <QSettings>

TPGWidget::TPGWidget(QWidget *parent) : QWidget(parent)
{
    createLayout();
    tpg_controller *controller = new tpg_controller("");
    tpg_controllerPtr.reset(controller);
    connectSignals();

}

TPGWidget::TPGWidget(const char *name) : QWidget()
{   createLayout();
    instanceName = name;
    tpg_controller *controller = new tpg_controller(instanceName);
    tpg_controllerPtr.reset(controller);
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
}

void TPGWidget::deviceConnect()
{

}

void TPGWidget::onConnect()
{

}

void TPGWidget::onDisconnect()
{

}

void TPGWidget::updateStatus(QString info)
{

}

void TPGWidget::closeEvent(QCloseEvent* e)
{
    //tpg_controllerPtr->deviceDisconnect();
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
    connect = new QPushButton("Connect");
    disconnect = new QPushButton("Disconnect");
    disconnect->setEnabled(false);
    vbConnect->addWidget(connect);
    vbConnect->addWidget(disconnect);

    QHBoxLayout *hb2 = new QHBoxLayout;
    hb2->addLayout(vbIP);
    hb2->addLayout(vbPort);
    hb2->addLayout(vbConnect);

    mainLayout->addLayout(hb2);
}

void TPGWidget::connectSignals()
{

}

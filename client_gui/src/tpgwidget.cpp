#include "TPG362widget.h"
#include <string>
#include <QSettings>

TPG362widget::TPG362widget(QWidget *parent) : QWidget(parent)
{
    createLayout();

    TPG362Controller *controller = new TPG362Controller();
    TPG362ControllerPtr.reset(controller);

    connectSignals();
}

TPG362widget::TPG362widget(const char *name) : TPG362widget()
{
    instanceName = name;

    string IP(instanceName);
    IP.append("/IP");
    string Port(instanceName);
    Port.append("/Port");
    connectionIP->setText(QSettings().value(IP.c_str()).toString());
    connectionPort->setText(QSettings().value(Port.c_str()).toString());
}

TPG362widget::~TPG362widget()
{
    //delete ui;
}

void TPG362widget::deviceConnect()
{

}

void TPG362widget::onConnect()
{

}

void TPG362widget::onDisconnect()
{

}

void TPG362widget::updateStatus(QString info)
{

}

void TPG362widget::closeEvent(QCloseEvent* e)
{
    TPG362ControllerPtr->deviceDisconnect();
    string IP(instanceName);
    IP.append("/IP");
    string Port(instanceName);
    Port.append("/Port");
    //save settings
    QSettings().setValue(IP.c_str(),connectionIP->text());
    QSettings().setValue(Port.c_str(),connectionPort->text());

    QWidget::closeEvent(e);
}

void TPG362widget::createLayout()
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

void TPG362widget::drawLine()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
}

void TPG362widget::createConnectionSection()
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

void TPG362widget::connectSignals()
{

}

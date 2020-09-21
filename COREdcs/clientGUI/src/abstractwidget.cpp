#include "abstractwidget.h"

AbstractWidget::AbstractWidget(std::string name, bool horizontalTcpPanel, QWidget *parent)
    : BaseWidget(name, parent) {
    tcp = new TCPWidget(horizontalTcpPanel);
    loadConfig();
}

void AbstractWidget::updateStatus(void *data) {
    BaseWidget::updateStatus(data);
    tcp->setStatus(*static_cast<bool *>(data));
}

void AbstractWidget::saveConfig() {
    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    QSettings().setValue(IP.c_str(), tcp->getIPText());
    QSettings().setValue(Port.c_str(), tcp->getPortText());
}
void AbstractWidget::loadConfig() {
    std::string IP(instanceName);
    IP.append("/IP");
    std::string Port(instanceName);
    Port.append("/Port");
    tcp->setIP(QSettings().value(IP.c_str()).toString());
    tcp->setPort(QSettings().value(Port.c_str()).toString());
}
void AbstractWidget::connectSignals() {
    connect(tcp->connectButton, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
    connect(tcp->disconnectButton, SIGNAL(clicked(bool)), this, SLOT(deviceDisconnect()));
}

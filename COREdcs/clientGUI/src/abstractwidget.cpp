#include "abstractwidget.h"
#include <iostream>
AbstractWidget::AbstractWidget(opc_controller *controller, std::string name,
                               bool horizontalTcpPanel, QWidget *parent)
    : BaseWidget(name, parent), controller(controller) {
    tcp = new TCPWidget(horizontalTcpPanel);
    loadConfig();
}

void AbstractWidget::updateStatus(void *data) {
    BaseWidget::updateStatus(data);
    tcp->setStatus(*static_cast<bool *>(data));
}

void AbstractWidget::updateConnectionParameters(void *data) {
    auto *param = static_cast<UA_ParametersTCP *>(data);

    std::string address;
    if(param->address.length == 0) {
        address = "";
    } else {
        address = std::string(reinterpret_cast<char *>(param->address.data),
                              param->address.length);
    }
    tcp->setIP(address);
    tcp->setPort(std::to_string(param->port));
}

void AbstractWidget::connectSignals() {
    connect(tcp->connectButton, SIGNAL(clicked(bool)), controller, SLOT(callConnect()));
    connect(tcp->disconnectButton, SIGNAL(clicked(bool)), controller,
            SLOT(callDisconnect()));
    connect(controller, SIGNAL(statusChanged(void *)), this, SLOT(updateStatus(void *)));
    connect(controller, SIGNAL(measurementsChanged(void *)), this,
            SLOT(updateMeasurements(void *)));
    connect(controller, SIGNAL(configurationChanged(void *)), this,
            SLOT(updateConfiguration(void *)));
    connect(controller, SIGNAL(connectionParametersChanged(void *)), this,
            SLOT(updateConnectionParameters(void *)));
}

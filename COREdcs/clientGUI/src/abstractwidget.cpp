#include "abstractwidget.h"
AbstractWidget::AbstractWidget(opc_controller *controller, std::string name,
                               bool horizontalTcpPanel, QWidget *parent)
    : BaseWidget(name, parent), controller(controller) {
    tcp = new TCPWidget(horizontalTcpPanel, this);
}

void AbstractWidget::updateStatus(void *data) {
    BaseWidget::updateStatus(data);
    auto status = *static_cast<bool *>(data);
    tcp->setStatus(status);
    deviceInfoLabel.setEnabled(status);
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
    connect(tcp, SIGNAL(changeTCPParameters(std::string, int)), controller,
            SLOT(callSetConnectionParameters(std::string, int)));
    connect(controller, SIGNAL(statusChanged(void *)), this, SLOT(updateStatus(void *)));
    connect(controller, SIGNAL(measurementsChanged(void *)), this,
            SLOT(updateMeasurements(void *)));
    connect(controller, SIGNAL(configurationChanged(void *)), this,
            SLOT(updateConfiguration(void *)));
    connect(controller, SIGNAL(connectionParametersChanged(void *)), this,
            SLOT(updateConnectionParameters(void *)));
    connect(controller, &opc_controller::deviceInfoChanged, this,
            &AbstractWidget::updateDeviceInfo);
}

void AbstractWidget::updateDeviceInfo(void *data) {
    auto f = [](const UA_String &ua) {
        std::string s = "";
        if(ua.length != 0) {
            s = std::string(reinterpret_cast<char *>(ua.data), ua.length);
        }
        return s;
    };
    auto *info = static_cast<UA_DeviceInfo *>(data);
    std::string deviceInfo = f(info->vendor) + "/" + f(info->model) + "/" +
                             f(info->firmwareVersion) + "/" + f(info->serialNumber);
    deviceInfoLabel.setText(deviceInfo.c_str());
}
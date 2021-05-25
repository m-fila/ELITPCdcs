#include "abstractwidget.h"
AbstractWidget::AbstractWidget(opc_controller *controller, std::string name,
                               bool horizontalTcpPanel, QWidget *parent)
    : BaseWidget(name, parent), controller(controller) {
    tcp = new TCPWidget(horizontalTcpPanel, this);
}

void AbstractWidget::updateStatus(UA_DataValue *data) {
    BaseWidget::updateStatus(data);
    auto status = *static_cast<bool *>(data->value.data);
    tcp->setStatus(status);
    deviceInfoLabel.setEnabled(status);
}

void AbstractWidget::updateConnectionParameters(UA_DataValue *data) {
    auto *param = static_cast<UA_ParametersTCP *>(data->value.data);

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
    auto *moniteredItem = controller->addMonitoredItem("status");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &AbstractWidget::updateStatus);
    moniteredItem = controller->addMonitoredItem("measurements");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &AbstractWidget::updateMeasurements);
    moniteredItem = controller->addMonitoredItem("configuration");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &AbstractWidget::updateConfiguration);
    moniteredItem = controller->addMonitoredItem("connectionParameters");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &AbstractWidget::updateConnectionParameters);
    moniteredItem = controller->addMonitoredItem("deviceInfo");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &AbstractWidget::updateDeviceInfo);
}

void AbstractWidget::updateDeviceInfo(UA_DataValue *data) {
    auto f = [](const UA_String &ua) {
        std::string s = "";
        if(ua.length != 0) {
            s = std::string(reinterpret_cast<char *>(ua.data), ua.length);
        }
        return s;
    };
    auto *info = static_cast<UA_DeviceInfo *>(data->value.data);
    std::string deviceInfo = f(info->vendor) + "/" + f(info->model) + "/" +
                             f(info->firmwareVersion) + "/" + f(info->serialNumber);
    deviceInfoLabel.setText(deviceInfo.c_str());
}
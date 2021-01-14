#include "basewidget.h"

BaseWidget::BaseWidget(std::string name, QWidget *parent)
    : QWidget(parent), instanceName(name) {}

void BaseWidget::startup() {
    this->show();
    this->activateWindow();
}

void BaseWidget::updateStatus(UA_Variant data) {
    if(externalLed != nullptr) {
        auto status = static_cast<bool *>(data.data);
        if(*status) {
            externalLed->setColor(Qt::darkGreen);
            externalLed->setState(KLed::State::On);
        } else {
            externalLed->setColor(Qt::red);
            externalLed->setState(KLed::State::Off);
        }
    }
}
void BaseWidget::saveConfig() {}
void BaseWidget::loadConfig() {}
void BaseWidget::closeEvent(QCloseEvent *e) {
    saveConfig();
    QWidget::closeEvent(e);
}

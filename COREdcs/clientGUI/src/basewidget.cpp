#include "basewidget.h"

BaseWidget::BaseWidget(std::string name,QWidget *parent): QWidget(parent), instanceName(name){
    //loadConfig();
}

void BaseWidget::startup(){
    this->show();
}

void BaseWidget::updateStatus(void *data){
    if(externalLed!=nullptr){
    auto status=static_cast<bool*>(data);
    if (*status){
        externalLed->setColor(Qt::darkGreen);
        externalLed->setState(KLed::State::On);
    }
    else{
        externalLed->setColor(Qt::red);
        externalLed->setState(KLed::State::Off);
    }
    }
}
void BaseWidget::saveConfig(){

}
void BaseWidget::loadConfig(){

}
void BaseWidget::closeEvent(QCloseEvent* e){
    saveConfig();
    QWidget::closeEvent(e);
}
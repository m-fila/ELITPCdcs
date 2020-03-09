#include "abstractwidget.h"

AbstractWidget::AbstractWidget(std::string name,QWidget *parent): QWidget(parent), instanceName(name){
    //loadConfig();
}

void AbstractWidget::startup(){
    this->show();
}

void AbstractWidget::updateStatus(void *data){
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
void AbstractWidget::saveConfig(){

}
void AbstractWidget::loadConfig(){

}
void AbstractWidget::closeEvent(QCloseEvent* e){
    saveConfig();
    QWidget::closeEvent(e);
}
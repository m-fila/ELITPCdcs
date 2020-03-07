#include "abstractwidget.h"

AbstractWidget::AbstractWidget(QWidget *parent) : QWidget(parent)
{
}
AbstractWidget::AbstractWidget(std::string name,QWidget *parent): QWidget(parent), instanceName(name){
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
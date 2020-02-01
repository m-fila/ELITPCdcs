#include "abstractwidget.h"

AbstractWidget::AbstractWidget(QWidget *parent) : QWidget(parent)
{
}
AbstractWidget::AbstractWidget(std::string name,QWidget *parent): QWidget(parent), instanceName(name){
}

void AbstractWidget::startup(){
    this->show();
}

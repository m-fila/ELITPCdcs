#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>
#include <string>
#include "opc_controller.h"
class AbstractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractWidget(QWidget *parent = 0);
    explicit AbstractWidget(std::string,QWidget *parent=0);
protected:
    const std::string instanceName;
signals:

public slots:
    virtual void startup();
    virtual void controllerInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)=0;
};

#endif // ABSTRACTWIDGET_H

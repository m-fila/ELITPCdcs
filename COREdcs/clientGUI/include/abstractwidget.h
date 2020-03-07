#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>
#include <string>
#include "kled.h"
#include "opc_controller.h"

class KLed;
class AbstractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractWidget(QWidget *parent = 0);
    explicit AbstractWidget(std::string,QWidget *parent=0);
    inline void setExternalLed(KLed* led){externalLed=led;}
protected:
    const std::string instanceName;
    KLed *externalLed;
signals:

public slots:
    virtual void startup();
    virtual void controllerInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)=0;
    virtual void updateStatus(void *data);
};

#endif // ABSTRACTWIDGET_H

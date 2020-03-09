#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>
#include <QSettings>
#include <string>
#include "basewidget.h"
#include "TCPWidget.h"

class KLed;
class AbstractWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit AbstractWidget(std::string,bool horizontalTpcPanel =false, QWidget *parent=0);
protected:
    TCPWidget *tcp;
    virtual void saveConfig();
    virtual void loadConfig();
    virtual void connectSignals();
public slots:
    virtual void updateStatus(void *data);

};

#endif // ABSTRACTWIDGET_H

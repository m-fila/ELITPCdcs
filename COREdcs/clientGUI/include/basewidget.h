#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include "kled.h"
#include "opc_controller.h"
#include <QWidget>
#include <string>

class KLed;
class BaseWidget : public QWidget {
    Q_OBJECT
  public:
    explicit BaseWidget(std::string, QWidget *parent = 0);
    inline void setExternalLed(KLed *led) { externalLed = led; }

  protected:
    const std::string instanceName;
    virtual std::string getDeviceType() const { return ""; }
    KLed *externalLed;
    virtual void saveConfig();
    virtual void loadConfig();
    virtual void closeEvent(QCloseEvent *e);
  signals:

  public slots:
    virtual void startup();
    virtual void controllerInit(UA_Client *, UA_ClientConfig *,
                                UA_CreateSubscriptionResponse) = 0;
    virtual void updateStatus(UA_Variant data);
};

#endif  // BASEWIDGET_H

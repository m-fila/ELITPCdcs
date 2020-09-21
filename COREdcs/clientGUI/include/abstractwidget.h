#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include "TCPWidget.h"
#include "basewidget.h"
#include <QSettings>
#include <QWidget>
#include <string>

class KLed;
class AbstractWidget : public BaseWidget {
    Q_OBJECT
  public:
    explicit AbstractWidget(std::string, bool horizontalTpcPanel = false,
                            QWidget *parent = 0);

  protected:
    TCPWidget *tcp;
    virtual void saveConfig();
    virtual void loadConfig();
    virtual void connectSignals();
  public slots:
    virtual void updateStatus(void *data);
};

#endif  // ABSTRACTWIDGET_H

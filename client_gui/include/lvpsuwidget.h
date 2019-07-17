#ifndef LVPSUWIDGET_H
#define LVPSUWIDGET_H

#include <QWidget>
//#include <open62541/client_subscriptions.h>
//#include <open62541/client_highlevel.h>
//#include <open62541/plugin/log_stdout.h>
//#include <string>
#include "lvcontroller.h"
namespace Ui {
class LVpsuWidget;
}

class LVpsuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LVpsuWidget(QWidget *parent = 0);
    ~LVpsuWidget();
    lv_controller *LVController;
private:
    Ui::LVpsuWidget *ui;
    void connectSignals();

public slots:
    void deviceConnect();
/*
public slots:
    void deviceConnect();
      void onConnect();
    void onDisconnect();
    void updateStatus(QString info);
    void updateSettings(HMPseriesSettings hmpSettings); // update set values usually at start end not so frequently as measured values
    void updateMeasurements(HMPseriesMeasurements hmpMeasurements); // measured voltage and current measured values

protected:
    void closeEvent(QCloseEvent* e);

    std::unique_ptr<LVpsuController> LVpsuControllerPtr;
*/


};

#endif // LVPSUWIDGET_H

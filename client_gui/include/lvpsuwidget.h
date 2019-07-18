#ifndef LVPSUWIDGET_H
#define LVPSUWIDGET_H

#include <QWidget>
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
    HMPMeasurements deviceSettings;
    bool connectionState;
protected:
    void closeEvent(QCloseEvent* e);
public slots:
    void deviceConnect();
    void deviceDisconnect();
    void setCH1ON();
    void setCH1OFF();
    void setCH2ON();
    void setCH2OFF();
    void setOutputON();
    void setOutputOFF();
    void updateStatus(void* data);
    void updateMeasuremnts(void* data);
    void updateConfiguration(void* data);
};

#endif // LVPSUWIDGET_H

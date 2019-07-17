#include "include/lvpsuwidget.h"
#include "ui_lvpsuwidget.h"
//#include "src/ConnectionParameters.h"
#include <iostream>
#include <QSettings>

LVpsuWidget::LVpsuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LVpsuWidget)
{
    ui->setupUi(this);

    ui->connectionIP->setText(QSettings().value("LVpsuIP").toString());
    ui->connectionPort->setText(QSettings().value("LVpsuPort").toString());
    LVController=new lv_controller("HMP2");
    connectSignals();
//    ObjectName="HMP2";
//    MeasurementsVariableName=ObjectName+".Measurements";
//    ConfigurationVariableName=ObjectName+".Configuration";
//    StatusVariableName=ObjectName+".Status";
/*
    LVpsuController *controller = new LVpsuController();
    LVpsuControllerPtr.reset(controller);
    connect(ui->connect, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
    connect(ui->disconnect, SIGNAL(clicked(bool)), controller, SLOT(deviceDisconnect()));
    connect(controller, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(controller, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(controller, SIGNAL(statusChanged(QString)), this, SLOT(updateStatus(QString)));

    qRegisterMetaType<HMPseriesSettings>(); // this is needed because slot and signal uses custom clases
                                            // see also Q_DECLARE_METATYPE(HMPseriesSettings) in "HMPseriesSettings.h"
                                            // which need to be called first
    connect(controller, SIGNAL(settingsReadFromDevice(HMPseriesSettings)), this, SLOT(updateSettings(HMPseriesSettings)));
    qRegisterMetaType<HMPseriesMeasurements>();
    connect(controller, SIGNAL(measurementsReadFromDevice(HMPseriesMeasurements)), this, SLOT(updateMeasurements(HMPseriesMeasurements)));

    connect(ui->CH1on, SIGNAL(clicked(bool)), controller, SLOT(setCH1outputON()));
    connect(ui->CH1off, SIGNAL(clicked(bool)), controller, SLOT(setCH1outputOFF()));
    connect(ui->CH2on, SIGNAL(clicked(bool)), controller, SLOT(setCH2outputON()));
    connect(ui->CH2off, SIGNAL(clicked(bool)), controller, SLOT(setCH2outputOFF()));
    connect(ui->outputON, SIGNAL(clicked(bool)), controller, SLOT(setGENoutputON()));
    connect(ui->outputOFF, SIGNAL(clicked(bool)), controller, SLOT(setGENoutputOFF()));
    */
}

LVpsuWidget::~LVpsuWidget()
{
    delete ui;
}

void LVpsuWidget::connectSignals(){
        connect(ui->disconnect, SIGNAL(clicked(bool)), LVController, SLOT(callDisconnect()));
        connect(ui->connect, SIGNAL(clicked(bool)), this, SLOT(deviceConnect()));
        connect(LVController,SIGNAL(statusChanged(bool)),this,SLOT(updateStatus(bool)));
}

/*
void LVpsuWidget::onConnect()
{
    ui->connect->setEnabled(false);
    ui->disconnect->setEnabled(true);
    ui->connectionStatus->setText("CONNECTED");
    QPalette palette = ui->connectionStatus->palette();
    palette.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->connectionStatus->setPalette(palette);

    ui->connectionIP->setEnabled(false);
    ui->connectionPort->setEnabled(false);

    LVpsuControllerPtr->start();
}

void LVpsuWidget::onDisconnect()
{
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->connectionStatus->setText("DISCONNECTED");
    ui->statusLabel->setText("...");
    QPalette palette = ui->connectionStatus->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    ui->connectionStatus->setPalette(palette);

    ui->connectionIP->setEnabled(true);
    ui->connectionPort->setEnabled(true);

    ui->CH1on->setEnabled(false);
    ui->CH1off->setEnabled(false);
    ui->CH2on->setEnabled(false);
    ui->CH2off->setEnabled(false);
    ui->outputON->setEnabled(false);
    ui->outputOFF->setEnabled(false);
}



void LVpsuWidget::updateStatus(QString info)
{
    ui->statusLabel->setText(info);
}

void LVpsuWidget::updateSettings(HMPseriesSettings hmpSettings)
{
    ui->CH1volatgeSet->display(QString::fromStdString(hmpSettings.CH1_voltage));
    ui->CH1currentSet->display(QString::fromStdString(hmpSettings.CH1_current));
    ui->CH1on->setEnabled(!hmpSettings.CH1);
    ui->CH1off->setEnabled(hmpSettings.CH1);
    ui->CH2voltageSet->display(QString::fromStdString(hmpSettings.CH2_voltage));
    ui->CH2currentSet->display(QString::fromStdString(hmpSettings.CH2_current));
    ui->CH2on->setEnabled(!hmpSettings.CH2);
    ui->CH2off->setEnabled(hmpSettings.CH2);

    ui->outputON->setEnabled(!hmpSettings.Output);
    ui->outputOFF->setEnabled(hmpSettings.Output);
}

void LVpsuWidget::updateMeasurements(HMPseriesMeasurements hmpMeasurements)
{
    ui->CH1voltage->display(QString::fromStdString(hmpMeasurements.CH1_voltage));
    ui->CH1current->display(QString::fromStdString(hmpMeasurements.CH1_current));
    ui->CH1on->setEnabled(!hmpMeasurements.CH1);
    ui->CH1off->setEnabled(hmpMeasurements.CH1);

    ui->CH2voltage->display(QString::fromStdString(hmpMeasurements.CH2_voltage));
    ui->CH2current->display(QString::fromStdString(hmpMeasurements.CH2_current));
    ui->CH2on->setEnabled(!hmpMeasurements.CH2);
    ui->CH2off->setEnabled(hmpMeasurements.CH2);

    ui->outputON->setEnabled(!hmpMeasurements.Output);
    ui->outputOFF->setEnabled(hmpMeasurements.Output);
}
*/
void LVpsuWidget::updateStatus(bool isConnected){
    if(isConnected){
        ui->connect->setEnabled(false);
        ui->disconnect->setEnabled(true);
        ui->connectionStatus->setText("CONNECTED");
        QPalette palette = ui->connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        ui->connectionStatus->setPalette(palette);
        ui->connectionIP->setEnabled(false);
        ui->connectionPort->setEnabled(false);
    }
    else{
        ui->connect->setEnabled(true);
        ui->disconnect->setEnabled(false);
        ui->connectionStatus->setText("DISCONNECTED");
        ui->statusLabel->setText("...");
        QPalette palette = ui->connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->connectionStatus->setPalette(palette);

        ui->connectionIP->setEnabled(true);
        ui->connectionPort->setEnabled(true);

        ui->CH1on->setEnabled(false);
        ui->CH1off->setEnabled(false);
        ui->CH2on->setEnabled(false);
        ui->CH2off->setEnabled(false);
        ui->outputON->setEnabled(false);
        ui->outputOFF->setEnabled(false);
    }

}

void LVpsuWidget::deviceConnect()
{
//    TCPConnectionParameters cp;
    std::string IPaddress = ui->connectionIP->text().toStdString();
    int port = ui->connectionPort->text().toInt();
    LVController->callConnect(IPaddress,port);
}

void LVpsuWidget::closeEvent(QCloseEvent* e)
{
    //LVpsuControllerPtr->deviceDisconnect();
    //save settings
    QSettings().setValue("LVpsuIP",ui->connectionIP->text());
    QSettings().setValue("LVpsuPort",ui->connectionPort->text());

    QWidget::closeEvent(e);
}

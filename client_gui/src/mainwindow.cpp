#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client=new opc_client();
    statemachine=new stateMachine("MachineState");

    lvPSU = new LVpsuWidget();
    lvPSU->setWindowFlags(Qt::Window);
    lvPSU->setWindowTitle("HMP2020 PSU Controller");

    hvPSU = new HVpsuWidget();
    hvPSU->setWindowFlags(Qt::Window);
    hvPSU->setWindowTitle("DT1415ET PSU Controller");

    pm1 = new TPGWidget("PM1");
    pm1->setWindowFlags(Qt::Window);
    pm1->setWindowTitle("TPG362 Controller (1)");

    pm2 = new TPGWidget("PM2");
    pm2->setWindowFlags(Qt::Window);
    pm2->setWindowTitle("TPG362 Controller (2)");

    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
    delete statemachine;
    delete lvPSU;
    delete hvPSU;
    delete pm1;
    delete pm2;
}
void MainWindow::startLVPSU(){
    lvPSU->move((this->pos().x())+(this->size().width()), this->pos().y());
    lvPSU->show();
}

void MainWindow::startHVPSU()
{
    hvPSU->move(this->pos().x(), (this->pos().y())+(this->size().height()));
    hvPSU->show();
}

void MainWindow::startPM1()
{
    pm1->show();
}

void MainWindow::startPM2()
{
    pm2->show();
}

void MainWindow::connectSignals(){
    //start LV
    connect(ui->StartLVpsu, SIGNAL(pressed()), this, SLOT(startLVPSU()));
    connect(client,SIGNAL(subCreated(UA_Client*,UA_ClientConfig*, UA_CreateSubscriptionResponse))
            ,lvPSU->LVController,SLOT(opcInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)));
    //StateMachine
    connect(ui->stateBox,SIGNAL(currentIndexChanged(int)),statemachine,SLOT(changeState(int)));
    connect(client,SIGNAL(subCreated(UA_Client*,UA_ClientConfig*, UA_CreateSubscriptionResponse))
            ,statemachine,SLOT(opcInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)));
    connect(statemachine,SIGNAL(stateChanged(int)),ui->stateBox,SLOT(setCurrentIndex(int)));
    //start PM
    connect(ui->StartPM1, SIGNAL(pressed()), this, SLOT(startPM1()));
    connect(ui->StartPM2, SIGNAL(pressed()), this, SLOT(startPM2()));
    //start HV
    connect(ui->StartHVpsu, SIGNAL(pressed()), this, SLOT(startHVPSU()));
    connect(client,SIGNAL(subCreated(UA_Client*,UA_ClientConfig*, UA_CreateSubscriptionResponse))
            ,hvPSU->HVController,SLOT(opcInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)));

}

void MainWindow::closeEvent(QCloseEvent* e)
{
    lvPSU->close();
    hvPSU->close();
    pm1->close();
    pm2->close();
    QWidget::closeEvent(e);
}

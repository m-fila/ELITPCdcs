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

    //statemachine->client=client->client;
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
    delete statemachine;
    delete lvPSU;
}
void MainWindow::startLVPSU(){
    lvPSU->move((this->pos().x())+(this->size().width()), this->pos().y());
    lvPSU->show();
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



}

void MainWindow::closeEvent(QCloseEvent* e)
{
    lvPSU->close();
    //hvPSU->close();
    //pm1->close();
    //pm2->close();
    QWidget::closeEvent(e);
}

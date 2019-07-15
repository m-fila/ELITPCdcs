#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client=new opc_client();
    statemachine=new stateMachine();
    statemachine->client=client->client;
   // client->start();
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals(){
    connect(ui->stateBox,SIGNAL(currentIndexChanged(int)),statemachine,SLOT(requestChange(int)));
    connect(client,SIGNAL(subCreated(UA_CreateSubscriptionResponse)),statemachine,SLOT(addMonitoredItem(UA_CreateSubscriptionResponse)));
    connect(statemachine,SIGNAL(stateChanged(int)),ui->stateBox,SLOT(setCurrentIndex(int)));
}

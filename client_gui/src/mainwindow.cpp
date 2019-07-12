#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client=new opc_client;
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals(){
    connect(ui->StartLVpsu,SIGNAL(pressed()),client,SLOT(run()));
}

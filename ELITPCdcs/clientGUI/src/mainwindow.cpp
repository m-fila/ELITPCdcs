#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFormLayout>
MainWindow::MainWindow(Json::Value &root,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client=new eli_client(root["address"].asString(),root["port"].asString());
    loadWidgets(root["devices"]);
    buildStateBox();
    statemachine=new stateMachine("MachineState");
    connectSignals();
}

MainWindow::~MainWindow(){
    for (auto i :startButtons)
        delete i;
    for (auto i :deviceWidgets)
        delete i;
    delete ui;
    delete stateBox;
    delete client;
    delete statemachine;
}

void MainWindow::buildStateBox(){
    stateBox=new QComboBox();
    stateBox->addItem("Empty");
    stateBox->addItem("Filing");
    stateBox->addItem("Ready");
    QFormLayout *stateLayout=new QFormLayout();
    stateLayout->addRow("state:",stateBox);
    QSpacerItem *spacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    ui->verticalLayout->addSpacerItem(spacer);
    ui->verticalLayout->addLayout(stateLayout);
}

void MainWindow::connectSignals(){
    //StateMachine
    connect(stateBox,SIGNAL(currentIndexChanged(int)),statemachine,SLOT(changeState(int)));
    connect(client,SIGNAL(subCreated(UA_Client*,UA_ClientConfig*, UA_CreateSubscriptionResponse))
            ,statemachine,SLOT(opcInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)));
    connect(statemachine,SIGNAL(stateChanged(int)),stateBox,SLOT(setCurrentIndex(int)));
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    for (auto i :deviceWidgets)
        i->close();
    QWidget::closeEvent(e);
}

void MainWindow::loadWidgets(Json::Value items){
    

    for (auto i : items) {

        AbstractWidget* new_widget;
        if(i["type"].asString()=="HMP2020"){
           new_widget=new LVpsuWidget(i["id"].asString(),i["address"].asString(),i["port"].asString());
        }
        else if(i["type"].asString()=="HMP4040"){
          new_widget= new LV4psuWidget(i["id"].asString(),i["address"].asString(),i["port"].asString());
       }
        else if(i["type"].asString()=="DT1415ET"){
          new_widget=new HVpsuWidget(i["id"].asString(),i["address"].asString(),i["port"].asString());
        }
        else if(i["type"].asString()=="TPG362"){
          new_widget=new TPGWidget(i["id"].asString(),i["address"].asString(),i["port"].asString());
        }
        else if(i["type"].asString()=="PiWeather"){
          new_widget=new PiWeatherWidget(i["id"].asString(),i["address"].asString(),i["port"].asString());
        }
        else{
           std::cout<<"Unknown device:"<<i["type"].asString()<<std::endl;
           continue;
        }
        QPushButton* new_button=new QPushButton(QString::fromStdString("Start "+i["id"].asString()));
        connect(new_button,SIGNAL(pressed()),new_widget,SLOT(startup()));
        connect(client,SIGNAL(subCreated(UA_Client*,UA_ClientConfig*, UA_CreateSubscriptionResponse))
                ,new_widget,SLOT(controllerInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)));
        ui->verticalLayout->addWidget(new_button);
        startButtons.push_back(new_button);
        deviceWidgets.push_back(new_widget);
        new_widget->setWindowFlags(Qt::Window);
        new_widget->setWindowTitle((i["type"].asString()+" ("+i["id"].asString()+")").c_str());
    }


}

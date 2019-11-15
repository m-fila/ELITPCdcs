#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFormLayout>
MainWindow::MainWindow(Loader &loader,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client=new opc_client(loader.getAddress(),loader.getPort());
    loadWidgets(loader.getItems());
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

void MainWindow::loadWidgets(std::vector<Loader_item> items){
    

    for (auto L : items) {

        AbstractWidget* new_widget;
        if(L.device=="HMP2020"){
           new_widget=new LVpsuWidget(L.Id,L.address,L.port);
        }
        else if(L.device=="HMP4040"){
          new_widget= new LV4psuWidget(L.Id,L.address,L.port);
       }
        else if(L.device=="DT1415ET"){
          new_widget=new HVpsuWidget(L.Id,L.address,L.port);
        }
        else if(L.device=="TPG362"){
          new_widget=new TPGWidget(L.Id,L.address,L.port);
        }
        else if(L.device=="PiWeather"){
          new_widget=new PiWeatherWidget(L.Id,L.address,L.port);
        }
        else{
           std::cout<<"Unknown device:"<<L.device<<std::endl;
           continue;
        }
        QPushButton* new_button=new QPushButton(QString::fromStdString("Start "+L.Id));
        connect(new_button,SIGNAL(pressed()),new_widget,SLOT(startup()));
        connect(client,SIGNAL(subCreated(UA_Client*,UA_ClientConfig*, UA_CreateSubscriptionResponse))
                ,new_widget,SLOT(controllerInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)));
        ui->verticalLayout->addWidget(new_button);
        startButtons.push_back(new_button);
        deviceWidgets.push_back(new_widget);
        new_widget->setWindowFlags(Qt::Window);
        new_widget->setWindowTitle((L.device+" ("+L.Id+")").c_str());
    }


}

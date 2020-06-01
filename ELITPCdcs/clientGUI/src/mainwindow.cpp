#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFormLayout>
MainWindow::MainWindow(json &config,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client=new eli_client(config.at("server").at("address").get<std::string>(),config.at("server").at("port").get<int>());
    loadWidgets(config.at("devices"));
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

void MainWindow::loadWidgets(json &items){
    

    for (auto &i : items) {
        AbstractWidget* new_widget;
        auto type=i.at("type").get<std::string>();
        auto id=i.at("id").get<std::string>();
        std::string address, port;
        try{
            address=i.at("address").get<std::string>();
        }
        catch(const nlohmann::detail::out_of_range &){
            address="";
        }
        try{
            port=i.at("port").get<std::string>();
        }
        catch(const nlohmann::detail::out_of_range &){
            port="";
        }
        if(type=="HMP2020"){
           new_widget=new LVpsuWidget(id,address,port);
        }
        else if(type=="HMP4040"){
          new_widget= new LV4psuWidget(id,address,port);
       }
        else if(type=="DT1415ET"){
            try{
                int enabledChannels=i.at("enabledChannels").get<int>();
                new_widget=new DT1415Widget(id,address,port,enabledChannels);
            }
            catch(const nlohmann::detail::out_of_range &){
                new_widget=new DT1415Widget(id,address,port);
            }    
        }
        else if(type=="N1471"){
            try{
                int enabledChannels=i.at("enabledChannels").get<int>();
                new_widget=new N1471Widget(id,address,port,enabledChannels);
            }
            catch(const nlohmann::detail::out_of_range &){
                new_widget=new N1471Widget(id,address,port);
            }    
        }
        else if(type=="TPG362"){
          new_widget=new TPGWidget(id,address,port);
        }
        else if(type=="PiWeather"){
          new_widget=new PiWeatherWidget(id,address,port);
        }
        else if(type=="MKS910"){
          new_widget=new MKS910Widget(id,address,port);
        }
        else{
           std::cout<<"Unknown device:"<<type<<std::endl;
           continue;
        }
        QHBoxLayout* deviceLayout=new QHBoxLayout();
        ui->verticalLayout->addLayout(deviceLayout);
        KLed* buttonLed= new KLed();
        buttonLed->setFixedSize(20,20);
        buttonLed->off();
        buttonLed->setColor(Qt::red);
        controlLeds.push_back(buttonLed);
        deviceLayout->addWidget(buttonLed);

        QPushButton* new_button=new QPushButton(QString::fromStdString("Start "+id));
        connect(new_button,SIGNAL(pressed()),new_widget,SLOT(startup()));
        connect(client,SIGNAL(subCreated(UA_Client*,UA_ClientConfig*, UA_CreateSubscriptionResponse))
                ,new_widget,SLOT(controllerInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse)));
        deviceLayout->addWidget(new_button);
        startButtons.push_back(new_button);
        deviceWidgets.push_back(new_widget);
        new_widget->setWindowFlags(Qt::Window);
        new_widget->setWindowTitle((type+" ("+id+")").c_str());
        new_widget->setExternalLed(buttonLed);
    }


}

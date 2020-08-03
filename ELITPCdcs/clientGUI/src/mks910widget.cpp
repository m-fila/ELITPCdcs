#include "mks910widget.h"
#include <QInputDialog>
#include <QSettings>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

MKS910Widget::MKS910Widget(std::string name, QWidget *parent)
    : AbstractWidget(name, parent) {
    createLayout();
    controller = new MKS910_controller(instanceName);
    connectSignals();
    loadConfig();
    setChannelName();
}

MKS910Widget::MKS910Widget(std::string name, std::string address, std::string port,
                           QWidget *parent)
    : MKS910Widget(name, parent) {
    if(address.size()) {
        tcp->setIP(address);
    }
    if(port.size()) {
        tcp->setPort(port);
    }
}

MKS910Widget::~MKS910Widget() {
    // delete ui;
    delete controller;
}

void MKS910Widget::connectSignals() {
    AbstractWidget::connectSignals();
    connect(controller, SIGNAL(statusChanged(void *)), this, SLOT(updateStatus(void *)));
    connect(controller, SIGNAL(measurementsChanged(void *)), this,
            SLOT(updateMeasurements(void *)));
    connect(controller, SIGNAL(configurationChanged(void *)), this,
            SLOT(updateConfiguration(void *)));
    connect(controller, SIGNAL(relayChanged(void *)), this, SLOT(updateRelay(void *)));
    connect(unitsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeUnits(int)));
}
void MKS910Widget::controllerInit(UA_Client *client, UA_ClientConfig *config,
                                  UA_CreateSubscriptionResponse resp) {
    controller->opcInit(client, config, resp);
}

void MKS910Widget::deviceConnect() {
    std::string IPaddress = tcp->getIP();
    int port = tcp->getPort();
    controller->callConnect(IPaddress, port);
}

void MKS910Widget::deviceDisconnect() { controller->callDisconnect(); }

void MKS910Widget::updateStatus(void *data) {
    AbstractWidget::updateStatus(data);
    bool isConnected = *static_cast<bool *>(data);
    connectionState = isConnected;
}
void MKS910Widget::updateMeasurements(void *data) {
    UA_MKS910m measurements = *static_cast<UA_MKS910m *>(data);
    QString val;
    std::string s;
    std::ostringstream os;
    os << std::scientific << std::setprecision(5) << std::uppercase
       << measurements.vacuum;
    s = os.str();
    s.insert(s.size() - 4, " ");
    s.erase(std::remove(s.begin(), s.end(), '+'), s.end());
    val = QString::fromStdString(s);
    mVacuum->display(val);

    MKS910codes::Units units = static_cast<MKS910codes::Units>(measurements.units);
    val = QString::fromStdString(MKS910codes::unitsToString.at(units));
    mUnitLabel->setText(val);
    MKS910codes::Status status = static_cast<MKS910codes::Status>(measurements.status);
    if(status == MKS910codes::Status::O) {
        val = QString::fromStdString("OK");
    } else {
        val = QString::fromStdString(MKS910codes::statusToString.at(status) + "fail");
    }
    mStatus->setText(val);
    unitsBox->setCurrentIndex(static_cast<int>(units));

    val.sprintf("%2.1f ± 3 °C", measurements.temperature);
    mTemp->setText(val);
}

void MKS910Widget::updateConfiguration(void *data) {}

void MKS910Widget::updateRelay(void *data) {
    auto r = *static_cast<UA_Relay *>(data);
    for(size_t i = 0; i < r.statusSize; i++) {
        RelayStruct rStruct;
        rStruct.status = r.status[i];
        rStruct.direction = r.direction[i];
        rStruct.enabled = r.enabled[i];
        rStruct.setpoint = r.setpoint[i];
        rStruct.hysteresis = r.hysteresis[i];
        relayWidgets.at(i)->setValues(rStruct);
    }
}

void MKS910Widget::changeRelay(int nr, RelayStruct values) {
    controller->callSetRelay(nr, values.enabled, values.setpoint, values.hysteresis);
}

void MKS910Widget::updateStatusLabel(QString info) { statusLabel->setText(info); }

void MKS910Widget::createLayout() {
    // create main layout with base size
    mainLayout = new QVBoxLayout();
    resize(400, 350);

    mainLayout->addWidget(tcp);
    tab = new QTabWidget();
    mainLayout->addWidget(tab);

    createMTab();
    createHTab();
    createCTab();
    createRTab();
    mainLayout->addStretch();

    statusLabel = new QLabel("...");
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
}
void MKS910Widget::createMTab() {
    QWidget *mWidget = new QWidget();
    tab->addTab(mWidget, "Pressure");
    QVBoxLayout *mLayout = new QVBoxLayout();
    mWidget->setLayout(mLayout);
    // mainLayout->addStretch();
    mBox = new QGroupBox("Pressure");
    QVBoxLayout *mvLayout = new QVBoxLayout();
    mBox->setLayout(mvLayout);
    QHBoxLayout *mhStatusLayout = new QHBoxLayout();
    mvLayout->addLayout(mhStatusLayout);
    mStatusLabel = new QLabel("Status: ");
    mStatusLabel->setAlignment(Qt::AlignLeft);  //|Qt::AlignCenter);
    mStatus = new QLabel(".");
    mStatus->setAlignment(Qt::AlignLeft);
    QFont statusFont = mStatus->font();
    statusFont.setBold(true);
    mStatus->setFont(statusFont);
    mhStatusLayout->addWidget(mStatusLabel);
    mhStatusLayout->addWidget(mStatus);

    QHBoxLayout *mhTempLayout = new QHBoxLayout();
    mvLayout->addLayout(mhTempLayout);
    auto mTempLabel = new QLabel("Temperature: ");
    mTempLabel->setAlignment(Qt::AlignLeft);
    mTemp = new QLabel("°C");
    mTemp->setAlignment(Qt::AlignLeft);
    mhTempLayout->addWidget(mTempLabel);
    mhTempLayout->addWidget(mTemp);

    QHBoxLayout *mhUnitLayout = new QHBoxLayout();
    mvLayout->addLayout(mhUnitLayout);
    mUnitLabel = new QLabel("MBAR");
    mUnitLabel->setAlignment(Qt::AlignRight);
    mhUnitLayout->addWidget(mUnitLabel);

    QHBoxLayout *mhVacuumLayout = new QHBoxLayout();
    mvLayout->addLayout(mhVacuumLayout);
    mVacuum = new QLCDNumber();
    mVacuum->setDigitCount(12);
    mVacuum->setSegmentStyle(QLCDNumber::Flat);
    mVacuum->setMinimumSize(QSize(200, 50));
    QPalette palette = mVacuum->palette();
    palette.setColor(QPalette::WindowText, Qt::darkGreen);
    mVacuum->setPalette(palette);
    mVacuum->display(0.00);
    mhVacuumLayout->addWidget(mVacuum);

    mLayout->addWidget(mBox);
    mLayout->addStretch();
}
void MKS910Widget::createCTab() {
    QWidget *cWidget = new QWidget();
    tab->addTab(cWidget, "Config");
    QVBoxLayout *cLayout = new QVBoxLayout();
    cWidget->setLayout(cLayout);

    QGroupBox *cBox = new QGroupBox("");

    cLayout->addWidget(cBox);
    QVBoxLayout *cvLayout = new QVBoxLayout();
    cBox->setLayout(cvLayout);
    QHBoxLayout *chLayout = new QHBoxLayout();
    cvLayout->addLayout(chLayout);
    QLabel *clabel = new QLabel("Custom name:");
    cNameLabel = new QLabel("...");
    cNameButton = new QPushButton("Change name");
    connect(cNameButton, SIGNAL(pressed()), this, SLOT(changeNamePressed()));
    chLayout->addWidget(clabel);
    chLayout->addWidget(cNameLabel);
    chLayout->addWidget(cNameButton);

    QHBoxLayout *cuLayout = new QHBoxLayout();
    cvLayout->addLayout(cuLayout);
    QLabel *culabel = new QLabel("Units:");
    cuLayout->addWidget(culabel);
    unitsBox = new QComboBox();
    cuLayout->addWidget(unitsBox);
    for(auto &i : MKS910codes::unitsToString) {
        unitsBox->addItem(QString::fromStdString(i.second));
    }
    unitsBox->setCurrentIndex(static_cast<int>(MKS910codes::Units::MBAR));
}
void MKS910Widget::createHTab() {
    QWidget *hWidget = new QWidget();
    tab->addTab(hWidget, "Plots");
    QVBoxLayout *hLayout = new QVBoxLayout();
    hWidget->setLayout(hLayout);
}

void MKS910Widget::createRTab() {
    QWidget *rWidget = new QWidget();
    tab->addTab(rWidget, "Interlock");
    QVBoxLayout *rLayout = new QVBoxLayout();
    rWidget->setLayout(rLayout);
    std::map<int, std::string> enabledLabels;
    for(const auto &i : MKS910codes::relayEnabledToString) {
        enabledLabels.insert(std::make_pair(static_cast<int>(i.first), i.second));
    }
    for(int i = 1; i <= 3; ++i) {
        auto relayPanel = new DCSRelayWidget(i);
        relayPanel->setEnabledLabels(enabledLabels);
        rLayout->addWidget(relayPanel);
        connect(relayPanel, SIGNAL(changeValues(int, RelayStruct)), this,
                SLOT(changeRelay(int, RelayStruct)));
        relayWidgets.push_back(relayPanel);
    }
}

void MKS910Widget::drawLine() {
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
}

void MKS910Widget::changeUnits(int u) {
    if(connectionState) {
        controller->callSetUnits(u);
    }
}

void MKS910Widget::changeNamePressed() {
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Set name"), tr("Set name:"),
                                            QLineEdit::Normal, cCustomName, &ok);
    if(ok) {
        cCustomName = newName;
        setChannelName();
    }
}

void MKS910Widget::setChannelName() {
    QString title;
    title = tr("");
    title.append(cCustomName);
    mBox->setTitle(title);
    if(cCustomName.isEmpty())
        cNameLabel->setText("...");
    else
        cNameLabel->setText(cCustomName);
}

void MKS910Widget::loadConfig() {
    AbstractWidget::loadConfig();

    QString configkey;
    configkey.sprintf("%s/CustomName", instanceName.c_str());
    cCustomName = QSettings().value(configkey).toString();
}

void MKS910Widget::saveConfig() {
    AbstractWidget::saveConfig();
    QString configkey;
    configkey.sprintf("%s/CustomName", instanceName.c_str());
    QSettings().setValue(configkey, cCustomName);
}

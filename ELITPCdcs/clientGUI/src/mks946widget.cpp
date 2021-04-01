#include "mks946widget.h"
#include "utils.h"
#include "utilsQt.h"
#include <QGridLayout>
#include <QInputDialog>
#include <QSettings>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
MKS946Widget::MKS946Widget(std::string name, QWidget *parent)
    : AbstractWidget(new MKS946_controller(name), name, parent) {
    createLayout();
    connectSignals();
    loadConfig();
}

MKS946Widget::~MKS946Widget() {}

void MKS946Widget::connectSignals() {
    AbstractWidget::connectSignals();
    auto *moniteredItem = controller->addMonitoredItem("relay");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &MKS946Widget::updateRelay);
    moniteredItem = controller->addMonitoredItem("PID");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &MKS946Widget::updatePID);
    moniteredItem = controller->addMonitoredItem("PIDState");
    connect(moniteredItem, &DCSMonitoredItem::valueChanged, this,
            &MKS946Widget::updatePIDState);
}

void MKS946Widget::updateStatus(UA_Variant data) {
    AbstractWidget::updateStatus(data);
    connectionState = *static_cast<bool *>(data.data);
    if(!connectionState) {
        mVacuum->display(0);
        mFlow->display(0);
    }
    flowMode.setEnabled(connectionState);
    flowSetPoint.setEnabled(connectionState);
    flowNominalRange.setEnabled(connectionState);
    flowScaleFactor.setEnabled(connectionState);
    manometerType.setEnabled(connectionState);
    manometerNominalRange.setEnabled(connectionState);
    manometerVoltageRange.setEnabled(connectionState);

    PIDUnits.setEnabled(connectionState);
    PIDRecipe.setEnabled(connectionState);
    PIDFlowChannel.setEnabled(connectionState);
    PIDPressureChannel.setEnabled(connectionState);
    PIDPresssureSetPoint.setEnabled(connectionState);
    PIDKp.setEnabled(connectionState);
    PIDTimeConstant.setEnabled(connectionState);
    PIDDerivativeTimeConstant.setEnabled(connectionState);
    PIDCeiling.setEnabled(connectionState);
    PIDBase.setEnabled(connectionState);
    PIDPreset.setEnabled(connectionState);
    PIDStart.setEnabled(connectionState);
    PIDEnd.setEnabled(connectionState);
    PIDCtrlStart.setEnabled(connectionState);
    PIDDirection.setEnabled(connectionState);
    PIDBand.setEnabled(connectionState);
    PIDGain.setEnabled(connectionState);
    flowButton->setEnabled(connectionState);
    manometerButton->setEnabled(connectionState);
    PIDButton->setEnabled(connectionState);

    zeroMFCButton->setEnabled(connectionState);
    PIDStateButtonOFF->setEnabled(connectionState && PIDStateLabel->text() == "ON");
    PIDStateButtonON->setEnabled(connectionState && PIDStateLabel->text() == "OFF");
}

void MKS946Widget::updateMeasurements(UA_Variant data) {
    UA_MKS946m measurements = *static_cast<UA_MKS946m *>(data.data);
    auto f = [](double value) {
        std::string s;
        std::ostringstream os;
        os << std::scientific << std::setprecision(2) << std::uppercase << value;
        s = os.str();
        s.insert(s.size() - 4, " ");
        return QString::fromStdString(s);
    };

    mVacuum->display(f(measurements.pressure));
    mFlow->display(f(measurements.flow));
}

void MKS946Widget::updatePIDState(UA_Variant data) {
    auto state = *static_cast<UA_Boolean *>(data.data);
    PIDStateLabel->setText(state ? "ON" : "OFF");
    PIDStateButtonOFF->setEnabled(state);
    PIDStateButtonON->setEnabled(!state);
}

void MKS946Widget::updateConfiguration(UA_Variant data) {
    UA_MKS946c conf = *static_cast<UA_MKS946c *>(data.data);
    flowMode.setText(QString::fromStdString(DCSUtils::UaToStd(conf.flowMode)));
    flowSetPoint.setText(QString("%1").arg(conf.flowSetPoint));
    flowNominalRange.setText(QString("%1").arg(conf.flowNominalRange));
    flowScaleFactor.setText(QString("%1").arg(conf.flowScaleFactor));
    manometerType.setText(QString::fromStdString(DCSUtils::UaToStd(conf.manometerType)));
    manometerNominalRange.setText(QString("%1").arg(conf.manometerNominalRange));
    manometerVoltageRange.setText(
        QString::fromStdString(DCSUtils::UaToStd(conf.manometerVoltageRange)));
}

void MKS946Widget::updateRelay(UA_Variant data) {
    auto r = *static_cast<UA_Relay *>(data.data);
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

void MKS946Widget::updatePID(UA_Variant data) {

    auto pid = *static_cast<UA_PID *>(data.data);
    PIDUnits.setText(QString::fromStdString(DCSUtils::UaToStd(pid.units)));
    PIDRecipe.setText(QString("%1").arg(pid.recipeNr));
    PIDFlowChannel.setText(QString::fromStdString(DCSUtils::UaToStd(pid.flowChannel)));
    PIDPressureChannel.setText(
        QString::fromStdString(DCSUtils::UaToStd(pid.pressureChannel)));
    PIDPresssureSetPoint.setText(QString("%1").arg(pid.pressureSetPoint));
    PIDKp.setText(QString("%1").arg(pid.kp));
    PIDTimeConstant.setText(QString("%1").arg(pid.timeConstant));
    PIDDerivativeTimeConstant.setText(QString("%1").arg(pid.derivativeTimeConstant));
    PIDCeiling.setText(QString("%1").arg(pid.ceiling));
    PIDBase.setText(QString("%1").arg(pid.base));
    PIDPreset.setText(QString("%1").arg(pid.preset));
    PIDStart.setText(QString("%1").arg(pid.start));
    PIDEnd.setText(QString("%1").arg(pid.end));
    PIDCtrlStart.setText(QString("%1").arg(pid.ctrlStart));
    PIDDirection.setText(QString::fromStdString(DCSUtils::UaToStd(pid.direction)));
    PIDBand.setText(QString("%1").arg(pid.band));
    PIDGain.setText(QString("%1").arg(pid.gain));
}

void MKS946Widget::createLayout() {
    // create main layout with base size
    mainLayout = new QVBoxLayout();
    resize(400, 350);

    mainLayout->addWidget(tcp);
    tab = new QTabWidget();
    mainLayout->addWidget(tab);

    createMTab();
    createCTab();
    createPIDTab();
    createRTab();
    mainLayout->addStretch();
    mainLayout->addWidget(&deviceInfoLabel);

    setLayout(mainLayout);
}
void MKS946Widget::createMTab() {
    QWidget *mWidget = new QWidget();
    tab->addTab(mWidget, "Measurements");
    QVBoxLayout *mLayout = new QVBoxLayout();
    mWidget->setLayout(mLayout);
    auto createMeasurementsPanel = [](std::string name, QLabel *unitLabel,
                                      QLCDNumber *display) {
        auto *box = new QGroupBox(name.c_str());
        QVBoxLayout *layout = new QVBoxLayout();
        box->setLayout(layout);

        unitLabel->setAlignment(Qt::AlignRight);
        layout->addWidget(unitLabel);

        display->setDigitCount(12);
        display->setSegmentStyle(QLCDNumber::Flat);
        display->setMinimumSize(QSize(200, 50));
        auto palette = display->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        display->setPalette(palette);
        display->display(0.00);
        layout->addWidget(display);
        return box;
    };
    mVacuum = new QLCDNumber();
    mFlow = new QLCDNumber();
    mUnitLabel = new QLabel("mbar");
    mLayout->addWidget(createMeasurementsPanel("Flow", new QLabel("sccm"), mFlow));
    mLayout->addWidget(createMeasurementsPanel("Pressure", mUnitLabel, mVacuum));
}

void MKS946Widget::createCTab() {
    QWidget *cWidget = new QWidget();
    tab->addTab(cWidget, "Config channels");
    QVBoxLayout *cLayout = new QVBoxLayout();
    cWidget->setLayout(cLayout);

    auto *flowBox = new QGroupBox("Flow");
    cLayout->addWidget(flowBox);
    auto *flowVLayout = new QVBoxLayout();
    flowBox->setLayout(flowVLayout);
    auto *flowGrid = new QGridLayout();
    flowVLayout->addLayout(flowGrid);
    fillGrid({{"Mode:", &flowMode},
              {"Setpoint:", &flowSetPoint},
              {"Nominal range:", &flowNominalRange},
              {"Scale factor:", &flowScaleFactor}},
             flowGrid);
    flowButton = new QPushButton("Configure MFC");
    connect(flowButton, &QPushButton::pressed, this, &MKS946Widget::showFlowDialog);
    flowVLayout->addWidget(flowButton);

    zeroMFCButton = new QPushButton("Zero MFC");
    connect(zeroMFCButton, &QPushButton::clicked, this, &MKS946Widget::zeroMFC);
    cLayout->addWidget(zeroMFCButton);

    auto *pressureBox = new QGroupBox("Pressure");
    cLayout->addWidget(pressureBox);
    auto *pressureVLayout = new QVBoxLayout();
    pressureBox->setLayout(pressureVLayout);
    auto *pressureGrid = new QGridLayout();
    pressureVLayout->addLayout(pressureGrid);
    fillGrid({{"Type:", &manometerType},
              {"Nominal range:", &manometerNominalRange},
              {"Voltage range:", &manometerVoltageRange}},
             pressureGrid);
    manometerButton = new QPushButton("Configure CM");
    connect(manometerButton, &QPushButton::pressed, this,
            &MKS946Widget::showPressureDialog);
    pressureVLayout->addWidget(manometerButton);
}

void MKS946Widget::createPIDTab() {
    QWidget *pidWidget = new QWidget();
    tab->addTab(pidWidget, "PID");
    QVBoxLayout *pidLayout = new QVBoxLayout();
    pidWidget->setLayout(pidLayout);

    auto *stateLayout = new QHBoxLayout();
    pidLayout->addLayout(stateLayout);
    auto *statelabel = new QLabel("PID control: ");
    stateLayout->addWidget(statelabel);
    PIDStateLabel = new QLabel();
    stateLayout->addWidget(PIDStateLabel);
    PIDStateButtonOFF = new QPushButton("OFF");
    connect(PIDStateButtonOFF, &QPushButton::clicked, this, &MKS946Widget::setPIDState);
    stateLayout->addWidget(PIDStateButtonOFF);
    PIDStateButtonON = new QPushButton("ON");
    connect(PIDStateButtonON, &QPushButton::clicked, this, &MKS946Widget::setPIDState);
    stateLayout->addWidget(PIDStateButtonON);

    auto *pidBox = new QGroupBox("PID");
    pidLayout->addWidget(pidBox);
    auto *pidVLayout = new QVBoxLayout();
    pidBox->setLayout(pidVLayout);
    auto *pidGrid = new QGridLayout();
    pidVLayout->addLayout(pidGrid);
    fillGrid(
        {
            {"Units:", &PIDUnits},
            {"Recipe nr:", &PIDRecipe},
            {"MFC channel:", &PIDFlowChannel},
            {"Pressure channel:", &PIDPressureChannel},
            {"Pressure setpoint:", &PIDPresssureSetPoint},
            {"Kp:", &PIDKp},
            {"Time constant:", &PIDTimeConstant},
            {"Derivative time constant:", &PIDDerivativeTimeConstant},
            {"Ceiling:", &PIDCeiling},
            {"Base:", &PIDBase},
            {"Preset:", &PIDPreset},
            {"Start:", &PIDStart},
            {"End:", &PIDEnd},
            {"CtrlStart:", &PIDCtrlStart},
            {"Direction:", &PIDDirection},
            {"Band:", &PIDBand},
            {"Gain:", &PIDGain},
        },
        pidGrid, 9);
    PIDButton = new QPushButton("Configure PID");
    connect(PIDButton, &QPushButton::pressed, this, &MKS946Widget::showPIDDialog);
    pidVLayout->addWidget(PIDButton);
}

void MKS946Widget::createRTab() {
    QWidget *rWidget = new QWidget();
    tab->addTab(rWidget, "Interlock");
    QVBoxLayout *rLayout = new QVBoxLayout();
    rWidget->setLayout(rLayout);
    std::map<int, std::string> enabledLabels;
    for(const auto &i : MKS946codes::relayEnabledToString) {
        enabledLabels.insert(std::make_pair(static_cast<int>(i.first), i.second));
    }
    for(int i = 5; i <= 6; ++i) {
        auto relayPanel = new DCSRelayWidget(i);
        relayPanel->setEnabledLabels(enabledLabels);
        rLayout->addWidget(relayPanel);
        connect(relayPanel, SIGNAL(changeValues(int, RelayStruct)), this,
                SLOT(changeRelay(int, RelayStruct)));
        relayWidgets.push_back(relayPanel);
    }
}

void MKS946Widget::drawLine() {
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
}

void MKS946Widget::changeNamePressed() {
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Set name"), tr("Set name:"),
                                            QLineEdit::Normal, cCustomName, &ok);
    if(ok) {
        cCustomName = newName;
        setChannelName();
    }
}

void MKS946Widget::setChannelName() {
    QString title;
    title = tr("");
    title.append(cCustomName);
    if(cCustomName.isEmpty())
        cNameLabel->setText("...");
    else
        cNameLabel->setText(cCustomName);
}

void MKS946Widget::loadConfig() {
    QString configkey;
    configkey.sprintf("%s/CustomName", instanceName.c_str());
    cCustomName = QSettings().value(configkey).toString();
}

void MKS946Widget::saveConfig() {
    QString configkey;
    configkey.sprintf("%s/CustomName", instanceName.c_str());
    QSettings().setValue(configkey, cCustomName);
}

void MKS946Widget::changeRelay(int nr, RelayStruct values) {
    dynamic_cast<MKS946_controller *>(controller)
        ->callSetRelay(nr, values.enabled, values.setpoint, values.hysteresis);
}

void MKS946Widget::fillGrid(std::vector<std::pair<std::string, QWidget *>> names,
                            QGridLayout *grid, size_t row_max) {
    for(size_t i = 0; i < names.size(); ++i) {
        grid->addWidget(new QLabel(names.at(i).first.c_str()), i % row_max,
                        2 * (i / row_max), Qt::AlignmentFlag::AlignRight);
        grid->addWidget(names.at(i).second, i % row_max, 2 * (i / row_max) + 1);
    }
}

void MKS946Widget::showPIDDialog() {
    DCSInputDialog data("Change PID", "Enter PID parameters:", 9);
    data.addField("Units", PIDUnits.text(), true);
    data.addField("Recipe nr", PIDRecipe.text(), true);
    data.addField("MFC channel", DCSUtils::getKeys(MKS946codes::PIDFlowChannelFromString))
        .setInitial(PIDFlowChannel.text());
    data.addField("Pressure channel",
                  DCSUtils::getKeys(MKS946codes::PIDPressureChannelFromString))
        .setInitial(PIDPressureChannel.text());
    data.addField("Pressure setpoint", PIDPresssureSetPoint.text().toDouble())
        .setMin(0)
        .setMax(1e6);
    data.addField("Kp", PIDKp.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("Time constant", PIDTimeConstant.text().toDouble())
        .setMin(0)
        .setMax(1e6);
    data.addField("Derivative time constant", PIDDerivativeTimeConstant.text().toDouble())
        .setMin(0)
        .setMax(1e6);
    data.addField("Ceiling", PIDCeiling.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("Base", PIDBase.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("Preset", PIDPreset.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("Start", PIDStart.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("End", PIDEnd.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("CtrlStart", PIDCtrlStart.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("Direction", DCSUtils::getKeys(MKS946codes::PIDDirectionFromString))
        .setInitial(PIDDirection.text());
    data.addField("Band", PIDBand.text().toInt()).setMin(0).setMax(200);
    data.addField("Gain", PIDGain.text().toInt()).setMin(1).setMax(30);

    if(data.exec()) {
        dynamic_cast<MKS946_controller *>(controller)
            ->callConfigurePID(data.get<QString>("MFC channel").toStdString(),
                               data.get<QString>("Pressure channel").toStdString(),
                               data.get<double>("Pressure setpoint"),
                               data.get<double>("Kp"), data.get<double>("Time constant"),
                               data.get<double>("Derivative time constant"),
                               data.get<double>("Ceiling"), data.get<double>("Base"),
                               data.get<double>("Preset"), data.get<double>("Start"),
                               data.get<double>("End"), data.get<double>("CtrlStart"),
                               data.get<QString>("Direction").toStdString(),
                               data.get<int>("Gain"), data.get<int>("Band"));
    }
}

void MKS946Widget::showFlowDialog() {
    DCSInputDialog data("Change flow", "Enter flow parameters:", 10);
    data.addField("Mode", DCSUtils::getKeys(MKS946codes::flowModeFromString))
        .setInitial(flowMode.text());
    data.addField("Setpoint", flowSetPoint.text().toDouble()).setMin(0).setMax(1e6);
    data.addField("Nominal range", flowNominalRange.text().toDouble())
        .setMin(0)
        .setMax(1e6);
    data.addField("Scale factor", flowScaleFactor.text().toDouble())
        .setMin(0)
        .setMax(1e6);
    if(data.exec()) {
        dynamic_cast<MKS946_controller *>(controller)
            ->callConfigureFlow(
                data.get<QString>("Mode").toStdString(), data.get<double>("Setpoint"),
                data.get<double>("Nominal range"), data.get<double>("Scale factor"));
    }
}

void MKS946Widget::showPressureDialog() {
    DCSInputDialog data("Change pressure", "Enter pressure parameters:", 10);
    data.addField("Type", DCSUtils::getKeys(MKS946codes::CMTypeFromString))
        .setInitial(manometerType.text());
    data.addField("Nominal range", manometerNominalRange.text().toDouble())
        .setMin(0)
        .setMax(1e6);
    data.addField("Voltage range",
                  DCSUtils::getKeys(MKS946codes::CMVoltageRangeFromString))
        .setInitial(manometerVoltageRange.text())
        .setMin(0)
        .setMax(1e6);
    if(data.exec()) {
        dynamic_cast<MKS946_controller *>(controller)
            ->callConfigurePressure(data.get<QString>("Type").toStdString(),
                                    data.get<double>("Nominal range"),
                                    data.get<QString>("Voltage range").toStdString());
    }
}

void MKS946Widget::zeroMFC() {
    QMessageBox msgBox;
    msgBox.setText(QString::asprintf("Zero MFC channel %s.\nExecute only when flow "
                                     "reading is less than 5%% of full scale. ",
                                     "A1"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if(msgBox.exec() == QMessageBox::Ok) {
        dynamic_cast<MKS946_controller *>(controller)->callZeroMFC();
    }
}

void MKS946Widget::setPIDState() {
    QObject *obj = sender();
    if(obj == PIDStateButtonON) {
        dynamic_cast<MKS946_controller *>(controller)->callSetPIDState(true);
    } else if(obj == PIDStateButtonOFF)
        dynamic_cast<MKS946_controller *>(controller)->callSetPIDState(false);
}

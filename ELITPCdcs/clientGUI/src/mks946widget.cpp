#include "mks946widget.h"
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
}

void MKS946Widget::updateStatus(UA_Variant data) {
    AbstractWidget::updateStatus(data);
    connectionState = *static_cast<bool *>(data.data);
    if(!connectionState) {
        mVacuum->display(0);
        mFlow->display(0);

        flowMode.setText("");
        flowSetPoint.setText("");
        flowNominalRange.setText("");
        flowScaleFactor.setText("");
        manometerType.setText("");
        manometerNominalRange.setText("");
        manometerVoltageRange.setText("");

        PIDUnits.setText("");
        PIDRecipe.setText("");
        PIDFlowChannel.setText("");
        PIDPressureChannel.setText("");
        PIDPresssureSetPoint.setText("");
        PIDKp.setText("");
        PIDTimeConstant.setText("");
        PIDDerivativeTimeConstant.setText("");
        PIDCeiling.setText("");
        PIDBase.setText("");
        PIDPreset.setText("");
        PIDStart.setText("");
        PIDEnd.setText("");
        PIDCtrlStart.setText("");
        PIDDirection.setText("");
        PIDBand.setText("");
        PIDGain.setText("");
    }
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

void MKS946Widget::updateConfiguration(UA_Variant data) {
    UA_MKS946c conf = *static_cast<UA_MKS946c *>(data.data);
    flowMode.setText(QString::fromStdString(MKS946codes::flowModeToString.at(
        static_cast<MKS946codes::FlowMode>(conf.flowMode))));
    flowSetPoint.setText(QString("%1").arg(conf.flowSetPoint));
    flowNominalRange.setText(QString("%1").arg(conf.flowNominalRange));
    flowScaleFactor.setText(QString("%1").arg(conf.flowScaleFactor));
    manometerType.setText(QString::fromStdString(MKS946codes::CMTypeToString.at(
        static_cast<MKS946codes::CMType>(conf.manometerType))));
    manometerNominalRange.setText(QString("%1").arg(conf.manometerNominalRange));
    manometerVoltageRange.setText(QString("%1").arg(conf.manometerVoltageRange));
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

    auto uaString2std = [](UA_String ua) {
        std::string std;
        if(ua.length != 0) {
            std = std::string(reinterpret_cast<char *>(ua.data), ua.length);
        }
        return std;
    };
    auto pid = *static_cast<UA_PID *>(data.data);
    PIDUnits.setText(QString::fromStdString(uaString2std(pid.units)));
    PIDRecipe.setText(QString("%1").arg(pid.recipeNr));
    PIDFlowChannel.setText(QString::fromStdString(uaString2std(pid.flowChannel)));
    PIDPressureChannel.setText(QString::fromStdString(uaString2std(pid.pressureChannel)));
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
    PIDDirection.setText(QString("%1").arg(pid.direction));
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
    tab->addTab(cWidget, "Config");
    QVBoxLayout *cLayout = new QVBoxLayout();
    cWidget->setLayout(cLayout);

    auto *flowBox = new QGroupBox("Flow");
    cLayout->addWidget(flowBox);
    auto *flowGrid = new QGridLayout();
    flowBox->setLayout(flowGrid);
    fillGrid({{"Type:", &manometerType},
              {"Nominal range:", &manometerNominalRange},
              {"Voltage range:", &manometerVoltageRange}},
             flowGrid);

    auto *pressureBox = new QGroupBox("Pressure");
    cLayout->addWidget(pressureBox);
    auto *pressureGrid = new QGridLayout();
    pressureBox->setLayout(pressureGrid);
    fillGrid({{"Mode:", &flowMode},
              {"Setpoint:", &flowSetPoint},
              {"Nominal range:", &flowNominalRange},
              {"Scale factor:", &flowScaleFactor}},
             pressureGrid);
}

void MKS946Widget::createPIDTab() {
    QWidget *pidWidget = new QWidget();
    tab->addTab(pidWidget, "PID");
    QVBoxLayout *pidLayout = new QVBoxLayout();
    pidWidget->setLayout(pidLayout);

    auto *pidBox = new QGroupBox("PID");
    pidLayout->addWidget(pidBox);
    auto *pidGrid = new QGridLayout();
    pidBox->setLayout(pidGrid);

    fillGrid(
        {
            {"Units:", &PIDUnits},
            {"Recipe nr:", &PIDRecipe},
            {"MKF channel:", &PIDFlowChannel},
            {"Pressure channel:", &PIDPressureChannel},
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
        pidGrid, 8);
    PIDButton = new QPushButton("Set PID");
    PIDButton->setDisabled(true);
    pidLayout->addWidget(PIDButton);
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
        grid->addWidget(new QLabel(names.at(i).first.c_str()), i % row_max, i / row_max,
                        Qt::AlignmentFlag::AlignRight);
        grid->addWidget(names.at(i).second, i % row_max, i / row_max + 1);
    }
}
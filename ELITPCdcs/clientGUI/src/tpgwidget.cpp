#include "tpgwidget.h"
#include "TCPWidget.h"
#include <QInputDialog>
#include <QSettings>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
TPGWidget::TPGWidget(std::string name, QWidget *parent)
    : AbstractWidget(new tpg_controller(name), name, parent) {
    createLayout();
    connectSignals();
    loadConfig();
    setChannelsNames();
}

TPGWidget::~TPGWidget() {}

void TPGWidget::connectSignals() {
    AbstractWidget::connectSignals();
    connect(dynamic_cast<tpg_controller *>(controller), SIGNAL(relayChanged(void *)),
            this, SLOT(updateRelay(void *)));
}

// void TPGWidget::controllerInit(UA_Client *client, UA_ClientConfig *config,
//                               UA_CreateSubscriptionResponse resp) {
//    dynamic_cast<tpg_controller *>(controller)->opcInit(client, config, resp);
//}

void TPGWidget::updateStatus(void *data) {
    AbstractWidget::updateStatus(data);
    bool isConnected = *static_cast<bool *>(data);
    connectionState = isConnected;
    if(isConnected) {
    } else {

        for(int i = 0; i != 2; ++i) {
            mVacuum[i]->display(0);
            mStatus[i]->setText("");
        }
    }
}
void TPGWidget::updateMeasurements(void *data) {
    UA_TPG362m measurements = *static_cast<UA_TPG362m *>(data);
    if(measurements.statusSize) {
        QString val;
        std::string s;
        for(int i = 0; i != 2; ++i) {
            TPG362codes::Status stat =
                static_cast<TPG362codes::Status>(measurements.status[i]);
            val = QString::fromStdString(TPG362codes::statusToString.at(stat));
            mStatus[i]->setText(val);
            if(stat == TPG362codes::Status::NoSensor ||
               stat == TPG362codes::Status::SensorOff) {
                mVacuum[i]->display("OFF");
            } else {
                std::ostringstream os;
                os << std::scientific << std::setprecision(5) << std::uppercase
                   << measurements.vacuum[i];
                s = os.str();
                s.insert(s.size() - 4, " ");
                s.erase(std::remove(s.begin(), s.end(), '+'), s.end());
                val = QString::fromStdString(s);
                mVacuum[i]->display(val);
            }
        }
    }
}

void TPGWidget::updateConfiguration(void *data) {}

void TPGWidget::updateRelay(void *data) {
    auto r = *static_cast<UA_Relay *>(data);
    for(size_t i = 0; i < r.statusSize; i++) {
        RelayStruct rStruct;
        rStruct.status = r.status[i];
        rStruct.direction = r.direction[i];
        rStruct.enabled = r.enabled[i];
        rStruct.setpoint = r.setpoint[i];
        rStruct.hysteresis = r.hysteresis[i];
        rStruct.unit = "mbar";
        relayWidgets.at(i)->setValues(rStruct);
    }
}

void TPGWidget::changeRelay(int nr, RelayStruct values) {
    dynamic_cast<tpg_controller *>(controller)
        ->callSetRelay(nr, values.enabled, values.setpoint, values.hysteresis);
}

void TPGWidget::updateStatusLabel(QString info) { statusLabel->setText(info); }

void TPGWidget::createLayout() {
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

void TPGWidget::createMTab() {
    QWidget *mWidget = new QWidget();
    tab->addTab(mWidget, "Pressure");
    QVBoxLayout *mLayout = new QVBoxLayout();
    mWidget->setLayout(mLayout);
    for(int i = 0; i != 2; ++i) {
        mBox[i] = new QGroupBox(("CH" + std::to_string(i + 2)).c_str());
        QHBoxLayout *mhLayout = new QHBoxLayout();
        QVBoxLayout *mvLayout = new QVBoxLayout();
        mBox[i]->setLayout(mvLayout);
        mvLayout->addLayout(mhLayout);
        mVacuum[i] = new QLCDNumber();
        mVacuum[i]->setDigitCount(12);
        mVacuum[i]->setSegmentStyle(QLCDNumber::Flat);
        mVacuum[i]->setMinimumSize(QSize(200, 50));
        QPalette palette = mVacuum[i]->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        mVacuum[i]->setPalette(palette);
        mVacuum[i]->display(0.00);

        mStatusLabel[i] = new QLabel("Status: ");
        mStatusLabel[i]->setAlignment(Qt::AlignLeft);
        mStatus[i] = new QLabel(".");
        mStatus[i]->setAlignment(Qt::AlignLeft);
        QFont statusFont = mStatus[i]->font();

        mStatus[i]->setFont(statusFont);
        QLabel *unit = new QLabel("mbar");
        unit->setAlignment(Qt::AlignRight);
        mhLayout->addWidget(mStatusLabel[i]);
        mhLayout->addWidget(mStatus[i]);
        mhLayout->addWidget(unit);
        QHBoxLayout *mhhLayout = new QHBoxLayout();
        mvLayout->addLayout(mhhLayout);

        mhhLayout->addWidget(mVacuum[i]);

        mLayout->addWidget(mBox[i]);
        mLayout->addStretch();
    }
}
void TPGWidget::createCTab() {
    QWidget *cWidget = new QWidget();
    tab->addTab(cWidget, "Config");
    QVBoxLayout *cLayout = new QVBoxLayout();
    cWidget->setLayout(cLayout);
    for(int i = 0; i != 2; ++i) {
        QGroupBox *cBox = new QGroupBox(("CH " + std::to_string(i + 1)).c_str());
        cLayout->addWidget(cBox);
        QHBoxLayout *chLayout = new QHBoxLayout();
        cBox->setLayout(chLayout);
        QLabel *clabel = new QLabel("Custom name:");
        cNameLabel[i] = new QLabel("...");
        cNameButton[i] = new QPushButton("Change name");
        connect(cNameButton[i], SIGNAL(pressed()), this, SLOT(changeNamePressed()));
        chLayout->addWidget(clabel);
        chLayout->addWidget(cNameLabel[i]);
        chLayout->addWidget(cNameButton[i]);
    }
}
void TPGWidget::createHTab() {
    QWidget *hWidget = new QWidget();
    tab->addTab(hWidget, "Plots");
    QVBoxLayout *hLayout = new QVBoxLayout();
    hWidget->setLayout(hLayout);
}

void TPGWidget::createRTab() {
    QWidget *rWidget = new QWidget();
    tab->addTab(rWidget, "Interlock");
    QVBoxLayout *rLayout = new QVBoxLayout();
    std::map<int, std::string> enabledLabels = {
        {0, "OFF"}, {1, "ON"}, {2, "CH 1"}, {3, "CH 2"}};
    rWidget->setLayout(rLayout);
    for(int i = 1; i <= 4; ++i) {
        auto relayPanel = new DCSRelayWidget(i, RelayDirectionPolicy::Below);
        relayPanel->setEnabledLabels(enabledLabels);
        rLayout->addWidget(relayPanel);
        connect(relayPanel, SIGNAL(changeValues(int, RelayStruct)), this,
                SLOT(changeRelay(int, RelayStruct)));
        relayWidgets.push_back(relayPanel);
    }
}

void TPGWidget::drawLine() {
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
}

void TPGWidget::changeNamePressed() {
    QObject *obj = sender();
    bool ok;
    int i;
    for(i = 0; i < 2; i++) {
        if(cNameButton[i] == obj) {
            QString newName = QInputDialog::getText(
                this, tr("Set CH %1 name").arg(i + 1), tr("CH %1 name:").arg(i + 1),
                QLineEdit::Normal, cCustomName[i], &ok);
            if(ok) {
                cCustomName[i] = newName;
                setChannelName(i);
            }
        }
    }
}

void TPGWidget::setChannelName(int channelno) {
    QString title;
    title = tr("CH %1        ").arg(channelno + 1);
    title.append(cCustomName[channelno]);
    mBox[channelno]->setTitle(title);
    // set name on CH x tab (... if empty)
    if(cCustomName[channelno].isEmpty())
        cNameLabel[channelno]->setText("...");
    else
        cNameLabel[channelno]->setText(cCustomName[channelno]);
}

void TPGWidget::setChannelsNames() {
    int i;
    for(i = 0; i != 2; ++i) {
        setChannelName(i);
    }
}

void TPGWidget::loadConfig() {
    int i;
    QString configkey;
    for(i = 0; i != 2; ++i) {
        configkey.asprintf("%s/CustomName%i", instanceName.c_str(), i);
        cCustomName[i] = QSettings().value(configkey).toString();
    }
}

void TPGWidget::saveConfig() {
    int i;
    QString configkey;
    for(i = 0; i != 2; ++i) {
        configkey.asprintf("%s/CustomName%i", instanceName.c_str(), i);
        QSettings().setValue(configkey, cCustomName[i]);
    }
}

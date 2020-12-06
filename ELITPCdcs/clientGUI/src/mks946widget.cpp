#include "mks946widget.h"
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

void MKS946Widget::connectSignals() { AbstractWidget::connectSignals(); }

// void MKS946Widget::controllerInit(UA_Client *client, UA_ClientConfig *config,
//                                  UA_CreateSubscriptionResponse resp) {
//    dynamic_cast<MKS946_controller *>(controller)->opcInit(client, config, resp);
//}

void MKS946Widget::updateStatus(void *data) {
    AbstractWidget::updateStatus(data);
    bool isConnected = *static_cast<bool *>(data);
    connectionState = isConnected;
}
void MKS946Widget::updateMeasurements(void *data) {
    UA_MKS946m measurements = *static_cast<UA_MKS946m *>(data);
    QString val;
    std::string s;
    std::ostringstream os;
    os << std::scientific << std::setprecision(5) << std::uppercase
       << measurements.pressure;
    s = os.str();
    s.insert(s.size() - 4, " ");
    s.erase(std::remove(s.begin(), s.end(), '+'), s.end());
    val = QString::fromStdString(s);
    mVacuum->display(val);
    mFlow->display(measurements.flow);
}

void MKS946Widget::updateConfiguration(void *data) {}

void MKS946Widget::updateStatusLabel(QString info) { statusLabel->setText(info); }

void MKS946Widget::createLayout() {
    // create main layout with base size
    mainLayout = new QVBoxLayout();
    resize(400, 350);

    mainLayout->addWidget(tcp);
    tab = new QTabWidget();
    mainLayout->addWidget(tab);

    createMTab();
    //  createHTab();
    createCTab();
    createRTab();
    mainLayout->addStretch();

    statusLabel = new QLabel("...");
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
}
void MKS946Widget::createMTab() {
    QWidget *mWidget = new QWidget();
    tab->addTab(mWidget, "Measurements");
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
    auto palette = mVacuum->palette();
    palette.setColor(QPalette::WindowText, Qt::darkGreen);
    mVacuum->setPalette(palette);
    mVacuum->display(0.00);
    mhVacuumLayout->addWidget(mVacuum);
    mFlow = new QLCDNumber();
    mFlow->setDigitCount(12);
    mFlow->setSegmentStyle(QLCDNumber::Flat);
    mFlow->setMinimumSize(QSize(200, 50));
    auto paletteFlow = mFlow->palette();
    paletteFlow.setColor(QPalette::WindowText, Qt::darkGreen);
    mFlow->setPalette(paletteFlow);
    mFlow->display(0.00);
    mvLayout->addWidget(mFlow);

    mLayout->addWidget(mBox);
    mLayout->addStretch();
}
void MKS946Widget::createCTab() {
    QWidget *cWidget = new QWidget();
    tab->addTab(cWidget, "Config");
    QVBoxLayout *cLayout = new QVBoxLayout();
    cWidget->setLayout(cLayout);
}
void MKS946Widget::createHTab() {
    QWidget *hWidget = new QWidget();
    tab->addTab(hWidget, "Plots");
    QVBoxLayout *hLayout = new QVBoxLayout();
    hWidget->setLayout(hLayout);
}

void MKS946Widget::createRTab() {
    QWidget *rWidget = new QWidget();
    tab->addTab(rWidget, "Interlock");
    QVBoxLayout *rLayout = new QVBoxLayout();
    rWidget->setLayout(rLayout);
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
    mBox->setTitle(title);
    if(cCustomName.isEmpty())
        cNameLabel->setText("...");
    else
        cNameLabel->setText(cCustomName);
}

void MKS946Widget::loadConfig() {
    QString configkey;
    configkey.asprintf("%s/CustomName", instanceName.c_str());
    cCustomName = QSettings().value(configkey).toString();
}

void MKS946Widget::saveConfig() {
    QString configkey;
    configkey.asprintf("%s/CustomName", instanceName.c_str());
    QSettings().setValue(configkey, cCustomName);
}

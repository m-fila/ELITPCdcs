#include "DCSRelayWidget.h"

DCSRelayWidget::DCSRelayWidget(size_t nr, RelayDirectionPolicy directionPolicy,
                               QWidget *parent)
    : QWidget(parent), number(nr), directionPolicy(directionPolicy) {
    auto mainLayout = new QVBoxLayout(this);
    auto box = new QGroupBox(QString().asprintf("Relay %lu", number), this);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    auto innerLayout = new QVBoxLayout(this);
    auto firstRow = new QHBoxLayout(this);
    auto secondRow = new QHBoxLayout(this);
    box->setLayout(innerLayout);
    innerLayout->addLayout(firstRow);
    innerLayout->addLayout(secondRow);
    auto statusLabel = new QLabel("Status:", this);
    status = new QLabel("", this);
    status->setFixedWidth(50);
    firstRow->addWidget(statusLabel);
    firstRow->addWidget(status);
    auto enabledLabel = new QLabel("Enabled:", this);
    enabled = new QLabel("", this);
    enabled->setFixedWidth(50);
    firstRow->addWidget(enabledLabel);
    firstRow->addWidget(enabled);
    auto directionLabel = new QLabel("Direction:", this);
    direction = new QLabel("BELOW", this);
    direction->setFixedWidth(50);
    firstRow->addWidget(directionLabel);
    firstRow->addWidget(direction);
    auto setpointLabel = new QLabel("Setpoint:", this);
    setpoint = new QLabel("", this);
    setpoint->setFixedWidth(50);
    secondRow->addWidget(setpointLabel);
    secondRow->addWidget(setpoint);
    auto hysteresisLabel = new QLabel("Hysteresis:", this);
    hysteresis = new QLabel("", this);
    hysteresis->setFixedWidth(50);
    secondRow->addWidget(hysteresisLabel);
    secondRow->addWidget(hysteresis);
    units = new QLabel("");
    units->setFixedWidth(50);
    secondRow->addWidget(units);
    setButton = new QPushButton("Set relay", this);
    secondRow->addWidget(setButton);
    setValues({0, 0, 0, 0, 0, "mbar"});
    connectSignals();
}

void DCSRelayWidget::setValues(const RelayStruct &newVal) {
    value = newVal;
    try {
        enabled->setText(QString::fromStdString(enabledLabels.at(value.enabled)));
    } catch(const std::exception &e) {
        enabled->setText(QString::asprintf("%d", value.enabled));
    }
    if(value.direction) {
        status->setText("ABOVE");
    } else {
        status->setText("BELOW");
    }
    if(value.status) {
        status->setText("ON");
    } else {
        status->setText("OFF");
    }
    units->setText(QString::fromStdString(value.unit));
    setpoint->setText(QString::asprintf("%.2f", value.setpoint));
    hysteresis->setText(QString::asprintf("%.2f", value.hysteresis));
}

void DCSRelayWidget::showDialog() {
    RelayDialog dialog(number, value, enabledLabels, directionPolicy);
    dialog.setWindowModality(Qt::WindowModal);
    auto retv = dialog.exec();
    if(retv) {
        auto r = dialog.getValue();
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(QString::asprintf("Changing Relay %lu to:", number));
        msgBox.setInformativeText(QString::asprintf(
            "Enabled:\t%s\nDirection:\t%s\nSetpoint:\t%f %s\nHysteresis:\t%f "
            "%s\n\nDo you confirm?",
            enabledLabels.at(r.enabled).c_str(), r.direction ? "BELOW" : "ABOVE",
            r.setpoint, r.unit.c_str(), r.hysteresis, r.unit.c_str()));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        if(msgBox.exec() == QMessageBox::Ok) {
            std::cout << r.enabled << " " << r.setpoint << " " << r.hysteresis
                      << std::endl;
            emit changeValues(number, r);
        }
    }
}

void DCSRelayWidget::connectSignals() {
    connect(setButton, SIGNAL(pressed()), this, SLOT(showDialog()));
}

RelayDialog::RelayDialog(size_t number, RelayStruct init,
                         const std::map<int, std::string> &labels,
                         RelayDirectionPolicy directionPolicy, QWidget *parent)
    : QDialog(parent), directionPolicy(directionPolicy), unit(init.unit) {
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    QFormLayout *mainLayout = new QFormLayout(this);
    setLayout(outerLayout);
    outerLayout->addLayout(mainLayout);
    text.setText(QString::asprintf("Set Relay %lu:", number));
    mainLayout->addWidget(&text);
    for(auto i : labels) {
        enabled.addItem(QString::fromStdString(i.second));
    }
    int precision = 6;
    double step = 0.1;
    double maximum = 9999;
    enabled.setCurrentIndex(init.enabled);
    setpoint.setDecimals(precision);
    hysteresis.setDecimals(precision);
    setpoint.setSingleStep(step);
    hysteresis.setSingleStep(step);
    setpoint.setMaximum(maximum);
    hysteresis.setMaximum(maximum);
    setpoint.setValue(init.setpoint);
    hysteresis.setValue(init.hysteresis);
    // setpoint.setButtonSymbols(QAbstractSpinBox::NoButtons);
    // hysteresis.setButtonSymbols(QAbstractSpinBox::NoButtons);
    direction.setReadOnly(true);
    auto setpointLayout = new QHBoxLayout(this);
    setpointLayout->addWidget(&setpoint);
    auto setpointUnit = new QLabel(init.unit.c_str(), this);
    setpointLayout->addWidget(setpointUnit);
    auto hysteresisLayout = new QHBoxLayout(this);
    auto hysteresisUnit = new QLabel(init.unit.c_str(), this);
    hysteresisLayout->addWidget(&hysteresis);
    hysteresisLayout->addWidget(hysteresisUnit);
    mainLayout->addRow("Enabled:", &enabled);
    mainLayout->addRow("Setpoint:", setpointLayout);
    mainLayout->addRow("Hysteresis:", hysteresisLayout);
    auto blank = new QWidget(this);
    blank->setMinimumHeight(20);
    mainLayout->addRow("", blank);

    mainLayout->addRow("Direction:", &direction);
    outerLayout->addWidget(&img);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    outerLayout->addWidget(buttonBox);
    if(directionPolicy == RelayDirectionPolicy::Auto) {
        connect(&setpoint, SIGNAL(valueChanged(double)), this,
                SLOT(updateDirection(double)));
        connect(&hysteresis, SIGNAL(valueChanged(double)), this,
                SLOT(updateDirection(double)));
    } else {
        direction.setDisabled(true);
        connect(&setpoint, SIGNAL(valueChanged(double)), this,
                SLOT(updateHysteresis(double)));
    }
    updateDirection();
    connect(buttonBox, &QDialogButtonBox::accepted, this, &RelayDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &RelayDialog::reject);
}

RelayStruct RelayDialog::getValue() {
    return RelayStruct{0,
                       setpoint.value() <= hysteresis.value(),
                       enabled.currentIndex(),
                       setpoint.value(),
                       hysteresis.value(),
                       unit};
}

void RelayDialog::updateDirection(double) {
    bool isBelow;

    if(directionPolicy == RelayDirectionPolicy::Below) {
        isBelow = true;
    } else if(directionPolicy == RelayDirectionPolicy::Above) {
        isBelow = false;
    } else {
        isBelow = setpoint.value() <= hysteresis.value();
    }
    direction.setText(isBelow ? "BELOW" : "ABOVE");
    img.setPixmap(QPixmap(QString::fromUtf8(isBelow ? ":/images/res/relay_below.png"
                                                    : ":/images/res/relay_above.png"))
                      .scaled(QSize(this->size().width(), 200), Qt::KeepAspectRatio));
}

void RelayDialog::updateHysteresis(double newVal) {
    if(directionPolicy == RelayDirectionPolicy::Below) {
        if(newVal <= setpoint.value()) {
            hysteresis.setValue(1.1 * newVal);
            hysteresis.setMinimum(1.01 * newVal);
        }
    } else {
        if(newVal >= setpoint.value()) {
            hysteresis.setValue(0.9 * newVal);
            hysteresis.setMaximum(0.99 * newVal);
        }
    }
}

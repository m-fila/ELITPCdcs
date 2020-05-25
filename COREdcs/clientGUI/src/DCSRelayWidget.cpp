#include "DCSRelayWidget.h"

DCSRelayWidget::DCSRelayWidget(uint nr, RelayDirectionPolicy directionPolicy,
                               QWidget *parent)
    : QWidget(parent), number(nr), directionPolicy(directionPolicy) {
  auto mainLayout = new QVBoxLayout(this);
  auto box = new QGroupBox(QString().asprintf("Relay %u", number), this);
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
  setpoint->setFixedWidth(70);
  secondRow->addWidget(setpointLabel);
  secondRow->addWidget(setpoint);
  auto hysteresisLabel = new QLabel("Hysteresis:", this);
  hysteresis = new QLabel("", this);
  hysteresis->setFixedWidth(70);
  secondRow->addWidget(hysteresisLabel);
  secondRow->addWidget(hysteresis);
  setButton = new QPushButton("Set relay", this);
  secondRow->addWidget(setButton);
  setValues({0, 0, 0, 0, 0});
  connectSignals();
}

void DCSRelayWidget::setValues(const RelayStruct &newVal) {
  value = newVal;
  try {
    enabled->setText(QString::fromStdString(enabledLabels.at(value.enabled)));
  } catch (const std::exception &e) {
    enabled->setText(QString::asprintf("%d", value.enabled));
  }
  if (value.direction) {
    status->setText("ABOVE");
  } else {
    status->setText("BELOW");
  }
  if (value.status) {
    status->setText("ON");
  } else {
    status->setText("OFF");
  }
  setpoint->setText(QString::number(value.setpoint, 'E', 3));
  hysteresis->setText(QString::number(value.hysteresis, 'E', 3));
}

void DCSRelayWidget::showDialog() {
  RelayDialog dialog(number, value, enabledLabels, directionPolicy);
  dialog.setWindowModality(Qt::WindowModal);
  auto retv = dialog.exec();
  if (retv) {
    auto r = dialog.getValue();
    QMessageBox msgBox;
    msgBox.setText(QString::asprintf("Changing Relay %u to:", number));
    msgBox.setInformativeText(QString::asprintf(
        "Enabled:\t%s\nDirection:\t%s\nSetpoint:\t%.3e\nHysteresis:\t%."
        "3e\n\nConfirm?",
        enabledLabels.at(r.enabled).c_str(), r.direction ? "BELOW" : "ABOVE",
        r.setpoint, r.hysteresis));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    if (msgBox.exec() == QMessageBox::Ok) {
      std::cout << r.enabled << " " << r.setpoint << " " << r.hysteresis
                << std::endl;
    }
  }
}

void DCSRelayWidget::connectSignals() {
  connect(setButton, SIGNAL(pressed()), this, SLOT(showDialog()));
}

RelayDialog::RelayDialog(uint number, RelayStruct init,
                         const std::map<int, std::string> &labels,
                         RelayDirectionPolicy directionPolicy, QWidget *parent)
    : QDialog(parent), directionPolicy(directionPolicy) {
  QVBoxLayout *outerLayout = new QVBoxLayout(this);
  QFormLayout *mainLayout = new QFormLayout(this);
  setLayout(outerLayout);
  outerLayout->addLayout(mainLayout);
  text.setText(QString::asprintf("Set Relay %u:", number));
  mainLayout->addWidget(&text);
  for (auto i : labels) {
    enabled.addItem(QString::fromStdString(i.second));
  }

  enabled.setCurrentIndex(init.enabled);
  setpoint.setValue(init.setpoint);
  hysteresis.setValue(init.hysteresis);
  setpoint.setDecimals(8);
  hysteresis.setDecimals(8);
  setpoint.setButtonSymbols(QAbstractSpinBox::NoButtons);
  hysteresis.setButtonSymbols(QAbstractSpinBox::NoButtons);
  direction.setReadOnly(true);
  mainLayout->addRow("Enabled", &enabled);
  mainLayout->addRow("Setpoint", &setpoint);
  mainLayout->addRow("Hysteresis", &hysteresis);
  auto blank = new QWidget(this);
  blank->setMinimumHeight(20);
  mainLayout->addRow("", blank);

  mainLayout->addRow("Direction", &direction);
  outerLayout->addWidget(&img);
  QDialogButtonBox *buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
  outerLayout->addWidget(buttonBox);
  if (directionPolicy == RelayDirectionPolicy::Auto) {
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
  return RelayStruct{0, setpoint.value() <= hysteresis.value(),
                     enabled.currentIndex(), setpoint.value(),
                     hysteresis.value()};
}

void RelayDialog::updateDirection(double) {
  bool isBelow;
  
  if(directionPolicy==RelayDirectionPolicy::Below){
      isBelow=true;
  }
  else if(directionPolicy==RelayDirectionPolicy::Above){
      isBelow=false;
  }
  else{
    isBelow = setpoint.value() <= hysteresis.value();   
  }
  direction.setText(isBelow ? "BELOW" : "ABOVE");
  img.setPixmap(
      QPixmap(QString::fromUtf8(isBelow ? ":/images/res/relay_below.png"
                                        : ":/images/res/relay_above.png"))
          .scaled(QSize(this->size().width(), 200), Qt::KeepAspectRatio));
}

void RelayDialog::updateHysteresis(double newVal) {
  if (directionPolicy == RelayDirectionPolicy::Below) {
    if (newVal <= setpoint.value()) {
      hysteresis.setValue(1.1 * newVal);
      hysteresis.setMinimum(1.01*newVal);
    }
  } else {
    if (newVal >= setpoint.value()) {
      hysteresis.setValue(0.9 * newVal);
      hysteresis.setMaximum(0.99*newVal);
    }
  }
}
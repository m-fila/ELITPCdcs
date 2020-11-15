#include "DCSProfileWidget.h"

DCSProfileWidget::DCSProfileWidget(QWidget *parent) : QWidget(parent) {
    auto *mainLayout = new QHBoxLayout(this);
    auto *box = new QGroupBox("Profile", this);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    auto *innerLayout = new QHBoxLayout(this);
    box->setLayout(innerLayout);
    text = new QLabel("Active:");
    innerLayout->addWidget(text);
    key = new QLabel("none");
    innerLayout->addWidget(key);
    selectButton = new QPushButton("Change", this);
    innerLayout->addWidget(selectButton);
    applyButton = new QPushButton("Apply", this);
    innerLayout->addWidget(applyButton);
    dumpButton = new QPushButton("Dump", this);
    innerLayout->addWidget(dumpButton);
    connectSignals();
}

void DCSProfileWidget::connectSignals() {
    connect(applyButton, SIGNAL(pressed()), this, SIGNAL(applyProfile()));
    connect(dumpButton, SIGNAL(pressed()), this, SLOT(showDumpDialog()));
    connect(selectButton, SIGNAL(pressed()), this, SLOT(showSelectDialog()));
}

void DCSProfileWidget::showDumpDialog() {

    bool ok;
    QString text = QInputDialog::getText(
        this, "Profile dump key", "Key for dumped configuration:", QLineEdit::Normal,
        "new", &ok);
    if(ok && !text.isEmpty()) {
        emit dumpProfile(text.toStdString());
    }
}

void DCSProfileWidget::showSelectDialog() {

    auto doc = QJsonDocument::fromJson(enabledProfiles.toUtf8());
    if(doc.isObject()) {
        auto obj = doc.object();
        DCSProfileDialog dial(obj, key->text(), this);
        auto retv = dial.exec();
        if(retv) {
            emit setProfile(dial.getKey());
        }
    }
}

void DCSProfileWidget::updateActiveProfile(void *data) {

    auto *keyUA = static_cast<UA_String *>(data);
    std::string keyStr(reinterpret_cast<char *>(keyUA->data), keyUA->length);
    key->setText(QString::fromStdString(keyStr));
}

void DCSProfileWidget::updateEnabledProfiles(void *data) {

    auto *profilesUA = static_cast<UA_String *>(data);
    std::string profilesStr(reinterpret_cast<char *>(profilesUA->data),
                            profilesUA->length);
    enabledProfiles = QString::fromStdString(profilesStr);
}

void DCSProfileWidget::updateStatus(bool status) {
    applyButton->setEnabled(status);
    dumpButton->setEnabled(status);
    connected = status;
}

DCSProfileDialog::DCSProfileDialog(const QJsonObject &json, QString activeKey,
                                   QWidget *parent)
    : json(json), QDialog(parent) {
    auto *outerLayout = new QVBoxLayout(this);
    auto *innerLayout = new QHBoxLayout(this);
    QLabel *label = new QLabel("Select profile:", this);
    setLayout(outerLayout);
    outerLayout->addWidget(label);
    outerLayout->addLayout(innerLayout);
    innerLayout->addWidget(&list);
    innerLayout->addWidget(&body);
    list.insertItems(0, json.keys());
    body.setReadOnly(true);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    outerLayout->addWidget(buttonBox);
    connect(&list, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this,
            SLOT(keyChanged(QListWidgetItem *, QListWidgetItem *)));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DCSProfileDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DCSProfileDialog::reject);
    auto found = list.findItems(activeKey, Qt::MatchExactly);
    if(!found.isEmpty()) {
        list.setCurrentItem(found.at(0));
    }
}

void DCSProfileDialog::keyChanged(QListWidgetItem *current, QListWidgetItem *prev) {
    QJsonDocument doc(json.value(current->text()).toObject());
    body.setText(doc.toJson(QJsonDocument::Indented));
}

std::string DCSProfileDialog::getKey() {
    return list.currentItem()->text().toStdString();
}

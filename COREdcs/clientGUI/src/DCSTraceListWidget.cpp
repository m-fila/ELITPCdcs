#include "DCSTraceListWidget.h"
#include "DCSInputDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <algorithm>

DCSTraceListWidget::DCSTraceListWidget(QWidget *parent) : QWidget(parent) {
    connectSignals();
    auto *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(&list);
    auto *buttonLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addStretch();
    buttonLayout->addWidget(&configButton);
    configButton.setText("Configure\ntrace");
    timer.setInterval(10000);
    timer.start();
}

void DCSTraceListWidget::connectSignals() {
    connect(&timer, &QTimer::timeout, this, &DCSTraceListWidget::clearItems);
    connect(&configButton, &QPushButton::pressed, this,
            &DCSTraceListWidget::configureDialog);
}

void DCSTraceListWidget::clearItems() {
    auto now = QDateTime::currentDateTime().addMSecs(-timer.interval());
    auto obsolete =
        std::remove_if(traces.begin(), traces.end(),
                       [now](const DCSTraceItem *i) { return i->timestamp < now; });
    std::for_each(obsolete, traces.end(), [](DCSTraceItem *i) { delete i; });
    traces.erase(obsolete, traces.end());
}

void DCSTraceListWidget::configureDialog() {
    DCSInputDialog data("Configure trace",
                        "Traces older than the interval will be cleared.", 2);
    data.addField("Interval [s]", timer.interval() / 1000);
    if(data.exec()) {
        timer.setInterval(1000 * data.get<int>("Interval [s]"));
    }
}

void DCSTraceListWidget::addItem(QString message, QDateTime time) {
    auto *trace = new DCSTraceItem(time, message);
    traces.push_front(trace);
    list.addItem(trace);
}
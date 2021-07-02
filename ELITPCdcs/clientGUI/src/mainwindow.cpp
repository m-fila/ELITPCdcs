#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ProjectVersion.h>
#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QString>
#include <random>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    createWidgets();
    createMenu();
    timer.setInterval(1500);
    connect(&timer, &QTimer::timeout, this, &MainWindow::updateLabels);
    timer.start();
}

void MainWindow::createWidgets() {
    auto *grid = new QGridLayout();
    ui->verticalLayout->addLayout(grid);
    std::vector<QString> possibleStates = {"stable", "perfect", "fine"};
    for(size_t i = 0; i < items.size(); ++i) {
        auto *led = new KLed;
        led->setColor(Qt::darkGreen);
        led->setState(KLed::State::On);
        led->setFixedSize(20, 20);
        grid->addWidget(led, i, 0);
        grid->addWidget(new QLabel(items.at(i).first), i, 1);
        grid->addWidget(new QLabel("status: "), i, 2);
        grid->addWidget(items.at(i).second, i, 3);
    };
    updateLabels();
}
void MainWindow::updateLabels() {
    std::uniform_int_distribution<> distribution(0, possibleStates.size() - 1);
    for(size_t i = 0; i < items.size(); ++i) {
        items.at(i).second->setText(
            possibleStates.at(distribution(*QRandomGenerator::global())));
    }
}

void MainWindow::createMenu() {
    auto *helpMenu = menuBar()->addMenu(tr("&Help"));
    auto *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::aboutAction);
    aboutAct->setStatusTip(tr("Show about box"));
}

void MainWindow::aboutAction() {
    QMessageBox::about(this, tr("About dcs"),
                       "<p align='center'><b>dcs</b><br/>"
                       "Detector Control System for ELITPC<br/>"
                       "Client v" DCS_VERSION "</p>");
}

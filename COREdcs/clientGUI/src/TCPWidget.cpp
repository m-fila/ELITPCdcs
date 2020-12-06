#include "TCPWidget.h"

TCPWidget::TCPWidget(bool horizontal, QWidget *parent) {
    mainLayout = new QVBoxLayout();
    QLabel *connectionStatusLabel = new QLabel("Status: ");
    connectionStatus = new QLabel("DISCONNECTED");
    QPalette palette = connectionStatus->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    connectionStatus->setPalette(palette);
    QVBoxLayout *vbIP = new QVBoxLayout();
    QLabel *connectionIPLabel = new QLabel("IP:");
    connectionIP = new QLineEdit();
    QVBoxLayout *vbPort = new QVBoxLayout();
    QLabel *connectionPortLabel = new QLabel("Port:");
    connectionPort = new QLineEdit();
    QVBoxLayout *vbConnect = new QVBoxLayout();
    changeButton = new QPushButton("Change");
    connectButton = new QPushButton("Connect");
    disconnectButton = new QPushButton("Disconnect");
    disconnectButton->setEnabled(false);
    connectionIP->setEnabled(false);
    connectionPort->setEnabled(false);
    if(!horizontal) {
        QHBoxLayout *hb1 = new QHBoxLayout;
        hb1->addWidget(connectionStatusLabel);
        hb1->addWidget(connectionStatus);
        hb1->addStretch();

        mainLayout->addLayout(hb1);
        vbIP->addWidget(connectionIPLabel);
        vbIP->addWidget(connectionIP);
        vbPort->addWidget(connectionPortLabel);
        vbPort->addWidget(connectionPort);

        vbConnect->addWidget(changeButton);
        vbConnect->addWidget(connectButton);
        vbConnect->addWidget(disconnectButton);

        QHBoxLayout *hb2 = new QHBoxLayout;
        hb2->addLayout(vbIP);
        hb2->addLayout(vbPort);
        hb2->addLayout(vbConnect);

        mainLayout->addLayout(hb2);
    } else {
        QHBoxLayout *hb = new QHBoxLayout;
        mainLayout->addLayout(hb);
        hb->addWidget(connectionStatusLabel);
        hb->addWidget(connectionStatus);
        hb->addWidget(connectionIPLabel);
        hb->addWidget(connectionIP);
        hb->addWidget(connectionPortLabel);
        hb->addWidget(connectionPort);
        auto *vb = new QVBoxLayout();
        hb->addLayout(vb);
        vb->addWidget(changeButton);
        vb->addWidget(connectButton);
        vb->addWidget(disconnectButton);
    }
    setLayout(mainLayout);
}

void TCPWidget::setStatus(bool status) {
    if(status) {
        connectButton->setEnabled(false);
        disconnectButton->setEnabled(true);
        connectionStatus->setText("CONNECTED");
        QPalette palette = connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::darkGreen);
        connectionStatus->setPalette(palette);
    } else {
        connectButton->setEnabled(true);
        disconnectButton->setEnabled(false);
        connectionStatus->setText("DISCONNECTED");
        QPalette palette = connectionStatus->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        connectionStatus->setPalette(palette);
    }
}

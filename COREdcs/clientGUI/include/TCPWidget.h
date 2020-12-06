#ifndef TCPWIDGET_H
#define TCPWIDGET_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <cmath>
#include <iostream>
#include <string>
class TCPWidget : public QWidget {
    Q_OBJECT
  public:
    struct Parameters {
        std::string address;
        int port;
    };
    explicit TCPWidget(bool horizontal, QWidget *parent = 0);
    QPushButton *connectButton;
    QPushButton *changeButton;
    QPushButton *disconnectButton;
    inline void setIP(QString address) { connectionIP->setText(address); }
    inline void setPort(QString address) { connectionPort->setText(address); }
    inline void setIP(std::string address) {
        connectionIP->setText(QString::fromStdString(address));
    }
    inline void setPort(std::string address) {
        connectionPort->setText(QString::fromStdString(address));
    }
    void setStatus(bool status);
    inline std::string getIP() { return connectionIP->text().toStdString(); }
    inline int getPort() { return connectionPort->text().toInt(); }
    inline QString getIPText() { return connectionIP->text(); }
    inline QString getPortText() { return connectionPort->text(); }

  private:
    QVBoxLayout *mainLayout;
    QLabel *connectionStatus;
    QLineEdit *connectionIP;
    QLineEdit *connectionPort;

  private slots:
    void showDialog();

  signals:
    void changeTCPParameters(std::string address, int port);
};

class TCPParametersDialog : public QDialog {
    Q_OBJECT
  public:
    explicit TCPParametersDialog(QString address, int port, QWidget *parent = nullptr);
    TCPWidget::Parameters getValue();

  private:
    QLabel text;
    QLineEdit addressInput;
    QSpinBox portInput;
};

#endif  // TCPWIDGET_H

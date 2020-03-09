#ifndef TCPWIDGET_H
#define TCPWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <string>
#include <iostream>
class TCPWidget : public QWidget {
  Q_OBJECT
public:
  explicit TCPWidget(bool horizontal, QWidget *parent = 0);
  QPushButton *connectButton;
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
};

#endif // TCPWIDGET_H
#ifndef TPG362WIDGET_H
#define TPG362WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <memory>
#include "TPG362Controller.h"

class TPG362widget : public QWidget
{
    Q_OBJECT

public:
    explicit TPG362widget(QWidget *parent = 0);
    TPG362widget(const char *name);
    ~TPG362widget();

public slots:
    void deviceConnect();
    void onConnect();
    void onDisconnect();
    void updateStatus(QString info);

protected:
    void closeEvent(QCloseEvent* e);

private:
    const char *instanceName;
    std::unique_ptr<TPG362Controller> TPG362ControllerPtr;

    //create layout procedures and variables
    QVBoxLayout *mainLayout;
    QLabel *statusLabel;
    QLabel *connectionStatus;
    QLineEdit *connectionIP;
    QLineEdit *connectionPort;
    QPushButton *connect;
    QPushButton *disconnect;

    void createLayout();
    void drawLine();
    void createConnectionSection();

    void connectSignals();
};

#endif // TPG362WIDGET_H

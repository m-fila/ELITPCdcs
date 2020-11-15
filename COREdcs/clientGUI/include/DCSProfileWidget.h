#ifndef DCS_PROFILE_WIDGET_H
#define DCS_PROFILE_WIDGET_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <open62541/types_generated.h>
#include <string>

class DCSProfileWidget : public QWidget {
    Q_OBJECT
  public:
    explicit DCSProfileWidget(QWidget *parent = nullptr);

  signals:
    void dumpProfile(std::string key);
    void activeProfileChanged(std::string key);
    void applyProfile();

  private:
    void connectSignals();
    QLabel *text;
    QLabel *key;
    QPushButton *selectButton;
    QPushButton *applyButton;
    QPushButton *dumpButton;
    bool connected = false;
    QString enabledProfiles;
  private slots:
    void showDumpDialog();
    void showSelectDialog();

  public slots:
    void updateActiveProfile(void *data);
    void updateEnabledProfiles(void *data);
    void updateStatus(bool status);
  signals:
    void setProfile(std::string);
};

class DCSProfileDialog : public QDialog {
    Q_OBJECT
  public:
    explicit DCSProfileDialog(const QJsonObject &json, QString activeKey,
                              QWidget *parent = nullptr);
    std::string getKey();

  private:
    QListWidget list;
    QTextEdit body;
    QJsonObject json;
  private slots:
    void keyChanged(QListWidgetItem *current, QListWidgetItem *prev);
};
#endif  // DCS_PROFILE_WIDGET_H

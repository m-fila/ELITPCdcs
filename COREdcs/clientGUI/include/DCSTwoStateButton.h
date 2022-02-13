#ifndef DCS_TWO_STATE_BUTTON_H
#define DCS_TWO_STATE_BUTTON_H
#include "kled.h"
#include <QLabel>
#include <QPushButton>
#include <QString>
class DCSTwoStateButton : public QWidget {
    Q_OBJECT
    Q_PROPERTY(bool state MEMBER state READ getState WRITE setState NOTIFY stateChanged)
    Q_ENUMS(Display)
  public:
    enum class Display { None, Label, Led };
    DCSTwoStateButton(Qt::Orientation = Qt::Horizontal, Display display = Display::None,
                      QWidget *parent = nullptr);
    DCSTwoStateButton(QString onLabel, QString offLabel, Qt::Orientation = Qt::Horizontal,
                      Display display = Display::None, QWidget *parent = nullptr);
    bool getState() const noexcept { return state; }

  private:
    QPushButton onButton;
    QPushButton offButton;
    bool state = false;
    void update();
    QLabel *label = nullptr;
    KLed *led = nullptr;
  signals:
    void onClicked();
    void offClicked();
    void clicked(bool);
    void stateChanged(bool);

  public slots:
    void click();
    void setEnabled(bool enabled);
    void setDisabled(bool disabled);
    void setState(bool state);
};

#endif  // DCS_TWO_STATE_BUTTON_H

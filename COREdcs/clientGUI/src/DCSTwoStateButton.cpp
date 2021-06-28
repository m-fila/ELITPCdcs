#include "DCSTwoStateButton.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFontMetrics>
DCSTwoStateButton::DCSTwoStateButton(QString onLabel, QString offLabel,
                                     Qt::Orientation orientation, Display display,
                                     QWidget *parent)
    : QWidget(parent), onButton(onLabel), offButton(offLabel) {
    QBoxLayout *layout = nullptr;
    if(orientation == Qt::Vertical) {
        layout = new QVBoxLayout();
    } else {
        layout = new QHBoxLayout();
    }
    onButton.setText(onLabel);
    offButton.setText(offLabel);
    setLayout(layout);
    if(display == Display::Led) {
        led = new KLed();
        led->setFixedSize(20, 20);
        led->off();
        led->setColor(Qt::red);
        layout->addWidget(led);
    }
    if(display == Display::Label) {
        label = new QLabel();
        QFontMetrics metrics(label->font());
        label->setFixedWidth( std::max(metrics.horizontalAdvance(onLabel), metrics.horizontalAdvance(offLabel)));
        label->setAlignment(Qt::AlignHCenter);
        layout->addWidget(label);
    }

    layout->addWidget(&onButton);
    layout->addWidget(&offButton);
    connect(&onButton, SIGNAL(clicked()), this, SIGNAL(onClicked()));
    connect(&offButton, SIGNAL(clicked()), this, SIGNAL(offClicked()));
    connect(&onButton, SIGNAL(clicked()), this, SLOT(click()));
    connect(&offButton, SIGNAL(clicked()), this, SLOT(click()));
    update();
}

DCSTwoStateButton::DCSTwoStateButton(Qt::Orientation orientation, Display display,
                                     QWidget *parent)
    : DCSTwoStateButton("ON", "OFF", orientation, display, parent) {}

void DCSTwoStateButton::update() {
    onButton.setEnabled(!state);
    offButton.setEnabled(state);
    if(label != nullptr) {
        label->setText(state ? onButton.text() : offButton.text());
    }
    if(led != nullptr) {
        led->setColor(state ? Qt::darkGreen : Qt::red);
        led->setState(state ? KLed::State::On : KLed::State::Off);
    }
}

void DCSTwoStateButton::click() {
    if(sender() == &onButton) {
        emit clicked(true);
    } else if(sender() == &offButton) {
        emit clicked(false);
    }
}

void DCSTwoStateButton::setEnabled(bool enabled) {
    QWidget::setEnabled(enabled);
    onButton.setEnabled(enabled);
    offButton.setEnabled(enabled);
    if(enabled) {
        update();
    }
}

void DCSTwoStateButton::setDisabled(bool disabled) { setEnabled(!disabled); }

void DCSTwoStateButton::setState(bool state) {
    if(this->state == state) {
        return;
    }
    this->state = state;
    update();
    emit stateChanged(state);
}

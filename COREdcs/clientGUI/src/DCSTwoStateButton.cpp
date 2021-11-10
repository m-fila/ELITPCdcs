#include "DCSTwoStateButton.h"
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QVBoxLayout>
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
        layout->addStretch();
        label = new QLabel();
        QFontMetrics metrics(label->font());
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        auto onLabelSize = metrics.horizontalAdvance(onLabel);
        auto offLabelSize = metrics.horizontalAdvance(offLabel);
#else
        auto onLabelSize = metrics.boundingRect(onLabel).size().width();
        auto offLabelSize = metrics.boundingRect(offLabel).size().width();
#endif
        label->setFixedWidth(std::max(onLabelSize, offLabelSize));
        layout->addWidget(label);
        layout->addStretch();
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

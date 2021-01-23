#include "DCSInputDialog.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
DCSInputDialog::DCSInputDialog(const QString &title, const QString &text, size_t max_row,
                               QWidget *parent)
    : QDialog(parent), max_row(max_row) {
    setWindowTitle(title);
    auto *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    auto *textLabel = new QLabel(text);
    textLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(textLabel, Qt::AlignmentFlag::AlignCenter);
    mainLayout->addSpacing(10);
    layout = new QGridLayout();
    mainLayout->addLayout(layout);

    auto *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DCSInputDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DCSInputDialog::reject);
    mainLayout->addWidget(buttonBox);
}

DCSInputDialog::FieldOptions &DCSInputDialog::addField(const QString &key, QVariant value,
                                                       bool disabled) {

    QWidget *genericWidget = nullptr;
    switch(value.type()) {
    case QVariant::Bool: {
        auto *widget = new QCheckBox();
        widget->setChecked(value.toBool());
        genericWidget = widget;
        break;
    }
    case QVariant::Double: {
        auto *widget = new QDoubleSpinBox();
        widget->setButtonSymbols(QAbstractSpinBox::NoButtons);
        widget->setValue(value.toDouble());
        genericWidget = widget;
        break;
    }
    case QVariant::Int: {
        auto widget = new QSpinBox();
        widget->setButtonSymbols(QAbstractSpinBox::NoButtons);
        widget->setValue(value.toInt());
        genericWidget = widget;
        break;
    }
    case QVariant::String: {
        auto widget = new QLineEdit(value.toString());
        genericWidget = widget;
        break;
    }
    case QVariant::StringList: {
        auto widget = new QComboBox();
        widget->addItems(value.toStringList());
        genericWidget = widget;
        break;
    }
    default:
        throw std::logic_error("Incomatybile type: " + value.type());
        break;
    }
    genericWidget->setDisabled(disabled);

    layout->addWidget(new QLabel(key + ":"), combo.size() % max_row,
                      2 * (combo.size() / max_row));
    layout->addWidget(genericWidget, combo.size() % max_row,
                      2 * (combo.size() / max_row) + 1);
    FieldOptions option;
    auto r = combo.insert({key, std::tie(value, option, genericWidget)});
    return std::get<1>(r.first->second);
}

int DCSInputDialog::exec() {
    for(auto &el : combo) {
        QVariant &variant = std::get<0>(el.second);
        FieldOptions &options = std::get<1>(el.second);
        QWidget *&genericWidget = std::get<2>(el.second);
        switch(variant.type()) {
        case QVariant::Double: {
            auto *widget = qobject_cast<QDoubleSpinBox *>(genericWidget);
            widget->setMinimum(options.min);
            widget->setMaximum(options.max);
            widget->setValue(variant.toDouble());
            break;
        }
        case QVariant::Int: {
            auto *widget = qobject_cast<QSpinBox *>(genericWidget);
            widget->setMinimum(options.min);
            widget->setMaximum(options.max);
            widget->setValue(variant.toInt());
            break;
        }
        default:
            break;
        }
    }
    return QDialog::exec();
}
QVariant DCSInputDialog::getValue(const QString &key) const {
    auto tup = combo.at(key);
    QVariant &variant = std::get<0>(tup);
    FieldOptions &options = std::get<1>(tup);
    QWidget *&genericWidget = std::get<2>(tup);
    switch(variant.type()) {
    case QVariant::Bool: {
        auto *widget = qobject_cast<QCheckBox *>(genericWidget);
        variant = widget->isChecked();
        break;
    }
    case QVariant::Double: {
        auto *widget = qobject_cast<QDoubleSpinBox *>(genericWidget);
        variant = widget->value();
        break;
    }
    case QVariant::Int: {
        auto *widget = qobject_cast<QSpinBox *>(genericWidget);
        variant = widget->value();
        break;
    }
    case QVariant::String: {
        auto *widget = qobject_cast<QLineEdit *>(genericWidget);
        variant = widget->text();
        break;
    }
    case QVariant::StringList: {
        auto *widget = qobject_cast<QComboBox *>(genericWidget);
        if(options.returnString) {
            variant = widget->currentText();
        } else {
            variant = widget->currentIndex();
        }
        break;
    }
    default:
        break;
    }
    return variant;
}

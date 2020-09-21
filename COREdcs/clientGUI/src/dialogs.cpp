#include "dialogs.h"

ConfirmDialog::ConfirmDialog(QString text, QWidget *parent) : QDialog(parent) {
    setWindowTitle("Confirm");
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    layout = new QVBoxLayout();
    this->setLayout(layout);
    //   resize(300, 100);
    //   QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //   setSizePolicy(sizePolicy);
    //  setMinimumSize(QSize(300, 100));
    //  setMaximumSize(QSize(300, 100));

    label = new QLabel(this);
    label->setText(text);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(box);
}
ConfirmDialog::~ConfirmDialog() {
    delete(layout);
    delete label;
    delete box;
}

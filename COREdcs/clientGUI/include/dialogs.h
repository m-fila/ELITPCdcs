#ifndef DIALOGS_H
#define DIALOGS_H
#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>
//#include <QString>
class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    ConfirmDialog(QString text, QWidget *parent = 0);
    ~ConfirmDialog();
private:    
    QVBoxLayout *layout;
    QLabel *label;
    QDialogButtonBox *box;
};


#endif // DIALOGS_H
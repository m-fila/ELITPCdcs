#ifndef DCS_INPUT_DIALOG_H
#define DCS_INPUT_DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QString>
#include <QVariant>
#include <map>
class DCSInputDialog : public QDialog {
    Q_OBJECT
  public:
    class FieldOptions {
        friend DCSInputDialog;

      public:
        FieldOptions() = default;
        FieldOptions &setMin(double min) {
            this->min = min;
            return *this;
        }
        FieldOptions &setMax(double max) {
            this->max = max;
            return *this;
        }
        FieldOptions &setReturnIndex() {
            returnString = false;
            return *this;
        }
        FieldOptions &setReturnString() {
            returnString = true;
            return *this;
        }
        FieldOptions &setInitial(QString current) {
            currentString = current;
            return *this;
        }

      private:
        double min = 0;
        double max = 1e6;
        bool returnString = true;
        QString currentString;
    };

    DCSInputDialog(const QString &title, const QString &text, size_t max_row = 10,
                   QWidget *parent = nullptr);
    FieldOptions &addField(const QString &key, QVariant value, bool disbled = false);
    void addConstant(const QString &key, const QString &value);
    template <typename T> T get(const QString &key) const {
        return getValue(key).value<T>();
    }
    int exec() override;

  private:
    QVariant getValue(const QString &key) const;
    QGridLayout *layout;
    std::map<QString, std::tuple<QVariant, FieldOptions, QWidget *>> combo;
    size_t max_row;
};

#endif  // DCS_INPUT_DIALOG_H

#ifndef DCS_COREDCS_CLIENTGUI_TRACELISTWIDGET_H_
#define DCS_COREDCS_CLIENTGUI_TRACELISTWIDGET_H_

#include <QDateTime>
#include <QListWidget>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <deque>
class DCSTraceItem : public QListWidgetItem {
  public:
    DCSTraceItem(QDateTime timestamp, const QString &text, QListWidget *parent = nullptr)
        : QListWidgetItem(QString("%1\t").arg(timestamp.toString("dd.MM.yyyy hh:mm:ss")) +
                              text,
                          parent),
          timestamp(timestamp) {}
    QDateTime timestamp;
};

class DCSTraceListWidget : public QWidget {
    Q_OBJECT
  public:
    explicit DCSTraceListWidget(QWidget *parent = nullptr);
    void addItem(QString message, QDateTime time);
  public slots:
    void setnterval(size_t interval_s) { timer.setInterval(interval_s * 1000); }

  private:
    std::deque<DCSTraceItem *> traces;
    QTimer timer;
    QListWidget list;
    void connectSignals();
    QPushButton configButton;
  private slots:
    void clearItems();
    void configureDialog();
};

#endif  // DCS_COREDCS_CLIENTGUI_TRACELISTWIDGET_H_

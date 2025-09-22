#ifndef INVESTIGATIONTYPEBAR_H
#define INVESTIGATIONTYPEBAR_H

#include <QFrame>

namespace Ui {
class InvestigationTypeBar;
}

class InvestigationTypeBar : public QFrame {
    Q_OBJECT

  public:
    explicit InvestigationTypeBar(QWidget* parent = nullptr);
    ~InvestigationTypeBar();

  signals:
    void sendTestType(QString);
    void sendTestSelected(bool);
    void sendExitTestType();

  public slots:
    void receiveConnectionOpen(bool open);

  private slots:
    void on_selectTestButton_clicked();

  private:
    Ui::InvestigationTypeBar* ui;
    void setTestOptions();
    void setTestSelected();
    void exitTestSelected();
    void setConnectionOpen(bool open);

    QString getSelectedTest();
    QString current_test_type;
};

#endif // INVESTIGATIONTYPEBAR_H

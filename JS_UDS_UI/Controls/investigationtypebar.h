#ifndef INVESTIGATIONTYPEBAR_H
#define INVESTIGATIONTYPEBAR_H

#include <QFrame>

namespace Ui {
class InvestigationTypeBar;
}

class InvestigationTypeBar : public QFrame
{
    Q_OBJECT

public:
    explicit InvestigationTypeBar(QWidget *parent = nullptr);
    ~InvestigationTypeBar();

signals:
    void sendTestName(QString);

private slots:
    void on_selectTestButton_clicked();
    void on_endTestButton_clicked();

private:
    Ui::InvestigationTypeBar *ui;
    void setTestOptions();
    void setTestSelected(bool selected);

    QString getSelectedTest();

};

#endif // INVESTIGATIONTYPEBAR_H

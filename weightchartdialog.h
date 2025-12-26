#ifndef WEIGHTCHARTDIALOG_H
#define WEIGHTCHARTDIALOG_H

#include <QDialog>
#include <QChart>
#include <QChartView>
namespace Ui {
class WeightChartDialog;
}

class WeightChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeightChartDialog(QWidget *parent = nullptr);
    ~WeightChartDialog();

    void setChart(QChart *chart);

private slots:
    void on_closeBtn_clicked();

private:
    Ui::WeightChartDialog *ui;
    QChartView *m_chartView = nullptr;
};

#endif // WEIGHTCHARTDIALOG_H

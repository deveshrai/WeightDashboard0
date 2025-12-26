#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WeightRecord.h"
#include "weightchartdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_Database_2_triggered();

    void on_actionSave_Changes_triggered();

    void on_tableWidget_cellChanged(int row, int column);

    void on_actionCharts_Trends_triggered();

    void on_actionCharts_Trends_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QString filename;
    QList<WeightRecord> records;
    void populateRecordsOnUI(void);
    bool updateRecordsBackToFile(QString filename);
    bool isUpdatingTable = false;
    bool updateRecordsForIOS(const QString &path);
    WeightChartDialog * wcd100;
    WeightChartDialog * wcd7;
    WeightChartDialog * wcdTrend;

};

#endif // MAINWINDOW_H

#include "weightchartdialog.h"
#include "ui_weightchartdialog.h"
#include <QVBoxLayout>

WeightChartDialog::WeightChartDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::WeightChartDialog)
{
    ui->setupUi(this);

    // Create the chart view once
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Ensure chartContainer has a layout
    if (!ui->chartContainer->layout()) {
        auto *layout = new QVBoxLayout(ui->chartContainer);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    ui->chartContainer->layout()->addWidget(m_chartView);
#ifndef Q_OS_IOS
    ui->closeBtn->hide();
#endif

    // Set the chartContainer as the central widget of the QMainWindow
    //setCentralWidget(ui->chartContainer);
}

WeightChartDialog::~WeightChartDialog()
{
    delete ui;
}

void WeightChartDialog::setChart(QChart *chart)
{
    m_chartView->setChart(chart);
}

void WeightChartDialog::on_closeBtn_clicked()
{
    this->hide();
}


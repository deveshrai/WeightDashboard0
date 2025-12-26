/********************************************************************************
** Form generated from reading UI file 'weightchartdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEIGHTCHARTDIALOG_H
#define UI_WEIGHTCHARTDIALOG_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WeightChartDialog
{
public:
    QVBoxLayout *verticalLayout;
    QChartView *chartContainer;
    QPushButton *closeBtn;

    void setupUi(QDialog *WeightChartDialog)
    {
        if (WeightChartDialog->objectName().isEmpty())
            WeightChartDialog->setObjectName("WeightChartDialog");
        WeightChartDialog->resize(812, 300);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WeightChartDialog->sizePolicy().hasHeightForWidth());
        WeightChartDialog->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(WeightChartDialog);
        verticalLayout->setObjectName("verticalLayout");
        chartContainer = new QChartView(WeightChartDialog);
        chartContainer->setObjectName("chartContainer");

        verticalLayout->addWidget(chartContainer);

        closeBtn = new QPushButton(WeightChartDialog);
        closeBtn->setObjectName("closeBtn");

        verticalLayout->addWidget(closeBtn);


        retranslateUi(WeightChartDialog);

        QMetaObject::connectSlotsByName(WeightChartDialog);
    } // setupUi

    void retranslateUi(QDialog *WeightChartDialog)
    {
        WeightChartDialog->setWindowTitle(QCoreApplication::translate("WeightChartDialog", "Chart", nullptr));
        closeBtn->setText(QCoreApplication::translate("WeightChartDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeightChartDialog: public Ui_WeightChartDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEIGHTCHARTDIALOG_H

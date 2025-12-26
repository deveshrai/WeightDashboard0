/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_Database;
    QAction *actionExit;
    QAction *actionOpen_Database_2;
    QAction *actionSave_Changes;
    QAction *actionClose_Database;
    QAction *actionCharts_Trends;
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionOpen_Database = new QAction(MainWindow);
        actionOpen_Database->setObjectName("actionOpen_Database");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionOpen_Database_2 = new QAction(MainWindow);
        actionOpen_Database_2->setObjectName("actionOpen_Database_2");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icons/database-svgrepo-com-3.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionOpen_Database_2->setIcon(icon);
        actionOpen_Database_2->setMenuRole(QAction::MenuRole::NoRole);
        actionSave_Changes = new QAction(MainWindow);
        actionSave_Changes->setObjectName("actionSave_Changes");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/icons/database-svgrepo-com.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionSave_Changes->setIcon(icon1);
        actionSave_Changes->setMenuRole(QAction::MenuRole::NoRole);
        actionClose_Database = new QAction(MainWindow);
        actionClose_Database->setObjectName("actionClose_Database");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/icons/database-svgrepo-com-2.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionClose_Database->setIcon(icon2);
        actionClose_Database->setMenuRole(QAction::MenuRole::NoRole);
        actionCharts_Trends = new QAction(MainWindow);
        actionCharts_Trends->setObjectName("actionCharts_Trends");
        actionCharts_Trends->setCheckable(true);
        actionCharts_Trends->setEnabled(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/icons/chart-svgrepo-com.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionCharts_Trends->setIcon(icon3);
        actionCharts_Trends->setMenuRole(QAction::MenuRole::NoRole);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/icons/info-svgrepo-com.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionAbout->setIcon(icon4);
        actionAbout->setMenuRole(QAction::MenuRole::NoRole);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget->rowCount() < 3)
            tableWidget->setRowCount(3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setRowCount(3);

        verticalLayout->addWidget(tableWidget);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 30));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Database);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        toolBar->addAction(actionOpen_Database_2);
        toolBar->addSeparator();
        toolBar->addAction(actionSave_Changes);
        toolBar->addAction(actionClose_Database);
        toolBar->addSeparator();
        toolBar->addAction(actionCharts_Trends);
        toolBar->addSeparator();
        toolBar->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_Database->setText(QCoreApplication::translate("MainWindow", "Open Database", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionOpen_Database_2->setText(QCoreApplication::translate("MainWindow", "Open Database", nullptr));
        actionSave_Changes->setText(QCoreApplication::translate("MainWindow", "Save Changes", nullptr));
        actionClose_Database->setText(QCoreApplication::translate("MainWindow", "Close Database", nullptr));
        actionCharts_Trends->setText(QCoreApplication::translate("MainWindow", "Charts and Trends", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "UUID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Weight", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Date and Time", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

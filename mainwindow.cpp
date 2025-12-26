#include "mainwindow.h"
#include "WeightDatabase.h"
#include "ui_mainwindow.h"
#include "SaveFileSpecialDialog.h"
#include "weightcharts.h"
#include <QStandardPaths>

#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->wcd100 = new WeightChartDialog(this);
    this->wcd7 = new WeightChartDialog(this);
    this->wcdTrend = new WeightChartDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Database_2_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Database",
        QString(),
        "All Files (*.*)"
        );

    if (!fileName.isEmpty()) {
        // Use the file path here
        qDebug() << "Selected file:" << fileName<<"\nAttempting to Open DB";
        this->filename = fileName;
        try {
            WeightDatabase wd(fileName);

            if (!QFile::exists(fileName)) {
                qWarning() << "Database file does not exist:" << fileName;
                return;
            }

            // Try a simple query to verify DB integrity
            this->records = wd.fetchAll();
            qDebug() << "Loaded" << records.size() << "records from database.";

            // Continue with your dashboard logic here...
            this->populateRecordsOnUI();

        } catch (const std::exception& ex) {
            qWarning() << "Exception while opening WeightDatabase:" << ex.what();
        } catch (...) {
            qWarning() << "Unknown exception while opening WeightDatabase.";
        }


    }
}

void MainWindow::populateRecordsOnUI()
{
    // Clear existing rows
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(records.size());

    // Set column headers (only once)
    ui->tableWidget->setColumnCount(3);
    QStringList headers;
    headers << "UUID" << "Weight" << "Timestamp";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Populate rows
    for (int row = 0; row < records.size(); ++row) {
        const auto& rec = records[row];

        // ID
        ui->tableWidget->setItem(
            row, 0,
            new QTableWidgetItem(rec.id.toString(QUuid::WithoutBraces))
            );

        // Weight
        ui->tableWidget->setItem(
            row, 1,
            new QTableWidgetItem(QString::number(rec.weight, 'f', 2))
            );

        // Timestamp (formatted)
        QString ts = rec.timestamp.toString("yyyy-MM-dd hh:mm:ss");
        ui->tableWidget->setItem(
            row, 2,
            new QTableWidgetItem(ts)
            );
    }

    // Optional: resize columns to fit content
    ui->tableWidget->resizeColumnsToContents();
}

bool MainWindow::updateRecordsBackToFile(QString filename)
{
    // Open the database
    WeightDatabase db(filename);

    // Clear existing rows
    if (!db.deleteAll()) {
        qWarning() << "Failed to clear existing records in DB";
        return false;
    }

    // Re-insert all records from the UI list
    for (const auto& rec : records) {
        if (!db.insertRecord(rec)) {
            qWarning() << "Failed to insert record back into DB:" << rec.id.toString();
            return false;
        }
    }

    return true;
}
bool MainWindow::updateRecordsForIOS(const QString &path)
{
    // Remove any existing file
    QFile::remove(path);

    // 1. Create a fresh SQLite DB
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "ios_export_connection");
    db.setDatabaseName(path);

    if (!db.open()) {
        qWarning() << "iOS export: failed to open DB at" << path;
        QSqlDatabase::removeDatabase("ios_export_connection");
        return false;
    }

    QSqlQuery q(db);

    // 2. Create schema
    if (!q.exec("CREATE TABLE IF NOT EXISTS weights ("
                "id TEXT PRIMARY KEY,"
                "weight REAL,"
                "timestamp TEXT"
                ")")) {
        qWarning() << "iOS export: failed to create table:" << q.lastError();
        db.close();
        QSqlDatabase::removeDatabase("ios_export_connection");
        return false;
    }

    // 3. Insert all records
    q.prepare("INSERT INTO weights (id, weight, timestamp) VALUES (?, ?, ?)");

    for (const auto &rec : records) {
        q.addBindValue(rec.id.toString());
        q.addBindValue(rec.weight);
        q.addBindValue(rec.timestamp.toString("yyyy-MM-dd hh:mm:ss"));

        if (!q.exec()) {
            qWarning() << "iOS export: insert failed:" << q.lastError();
        }
    }

    db.close();
    QSqlDatabase::removeDatabase("ios_export_connection");

    // 4. Verify file size
    QFileInfo info(path);
    qDebug() << "iOS export DB size:" << info.size();

    return info.size() > 0;
}

void MainWindow::on_actionSave_Changes_triggered()
{
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)

    // Desktop platforms: normal save-to-existing-file behavior
    if (!this->filename.isEmpty()) {
        if (updateRecordsBackToFile(this->filename)) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle("Success");
            msgBox.setText("Save Successful!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        } else {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("Error");
            msgBox.setText("Save failed!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    } else {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("No File Open");
        msgBox.setText("There is no open file to update.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

#else
    //
    // iOS / iPadOS branch — uses native UIDocumentPicker export
    //

    // 1. Build timestamped filename
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString suggestedName = QString("weights%1.sqlite").arg(timestamp);

    // QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
    //                    + "/temp_export.sqlite";
    // {
    //     // 1.
    //     WeightDatabase db(tempPath);
    //     //db.createTables();   // IMPORTANT: ensure schema exists
    //     // 2. Write your records into it
    //     updateRecordsBackToFile(tempPath);

    // }
    // // 2. Generate the SQLite bytes in memory
    // QByteArray dbBytes;
    // {
    //     // Write your in-memory records to a temporary SQLite file
    //     // then read it back into dbBytes


    //     QFile f(tempPath);
    //     if (f.open(QIODevice::ReadOnly))
    //         dbBytes = f.readAll();
    //     qDebug() << "Temp DB size:" << dbBytes.size();  // Should NOT be zero
    // }

    // // 3. Use the native iOS save dialog
    // QString savedPath = SaveFileSpecialDialog::saveFile(dbBytes, suggestedName);
    
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                       + "/temp_export.sqlite";

    if (!updateRecordsForIOS(tempPath)) {
        QMessageBox::critical(this, "Error", "Failed to prepare database for export.");
        return;
    }

    // Read bytes
    QByteArray dbBytes;
    {
        QFile f(tempPath);
        if (f.open(QIODevice::ReadOnly))
            dbBytes = f.readAll();
    }

    QString savedPath = SaveFileSpecialDialog::saveFile(dbBytes, suggestedName);

    // 4. If user selected a location, update filename
    if (!savedPath.isEmpty()) {
        this->filename = savedPath;

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Success");
        msgBox.setText("Save Successful!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Save Cancelled");
        msgBox.setText("No file was saved.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

#endif
}


void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if (isUpdatingTable)
        return; // Prevent recursion

    if (row < 0 || row >= records.size())
        return;

    auto& rec = records[row];
    auto* item = ui->tableWidget->item(row, column);
    if (!item)
        return;

    QString value = item->text().trimmed();

    switch (column) {

    case 0: // ID
    {
        QUuid newId(value);
        if (!newId.isNull()) {
            rec.id = newId;
        } else {
            qWarning() << "Invalid UUID entered:" << value;

            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle("Invalid ID");
            msg.setText("The ID you entered is not a valid UUID.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
        break;
    }

    case 1: // Weight
    {
        bool ok = false;
        double w = value.toDouble(&ok);
        if (ok) {
            rec.weight = w;
        } else {
            qWarning() << "Invalid weight entered:" << value;

            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle("Invalid Weight");
            msg.setText("Please enter a valid numeric weight.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
        break;
    }

    case 2: // Timestamp
    {
        QDateTime ts = QDateTime::fromString(value, "yyyy-MM-dd hh:mm:ss");
        if (ts.isValid()) {
            rec.timestamp = ts;
        } else {
            qWarning() << "Invalid timestamp entered:" << value;

            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle("Invalid Timestamp");
            msg.setText("Timestamp must be in the format:\n\nYYYY-MM-DD HH:MM:SS");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
        break;
    }

    } // switch
}


void MainWindow::on_actionCharts_Trends_triggered()
{

}


void MainWindow::on_actionCharts_Trends_triggered(bool checked)
{
#ifdef Q_OS_IOS
    if(this->wcd100){
        WeightCharts theCharts(this->records);
        
        this->wcd100->setChart(theCharts.buildLast100Chart());
        this->wcd7->setChart(theCharts.buildRolling7Chart());
        this->wcdTrend->setChart(theCharts.buildTrendlineChart());
        wcd100->show();
        wcd7->show();
        wcdTrend->show();
    }

#else
    if(checked){

        if(!this->records.isEmpty()){
            WeightCharts theCharts(this->records);
            if(this->wcd100){
                this->wcd100->setChart(theCharts.buildLast100Chart());
                this->wcd7->setChart(theCharts.buildRolling7Chart());
                this->wcdTrend->setChart(theCharts.buildTrendlineChart());
                // Get the outer frame of the main window
                QRect mainFrame = this->frameGeometry();

                // Get the size of the chart window (including its frame)
                QSize chartSize = this->wcd100->frameGeometry().size();

                // Compute top-right corner
                int x = mainFrame.x() + mainFrame.width();// - chartSize.width();
                int y = mainFrame.y();

                //this->wcd->adjustSize();
                this->wcd100->show();
                this->wcd100->move(x,y);
                this->wcd7->show();
                this->wcd7->move(x,y+wcd100->height());
                this->wcdTrend->show();
                this->wcdTrend->move(x,y+wcd100->height()+wcd7->height());


                //chartWin->show();

            }
            else{
                qWarning() << "Error! No pointer to WCD.";

                QMessageBox msg;
                msg.setIcon(QMessageBox::Critical);
                msg.setWindowTitle("Error");
                msg.setText("No Pointer to WCD!!");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
        }
        else{
            qWarning() << "No Records! \nRecord Count:" << records.count();

            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle("No Records");
            msg.setText("Database not loaded or No Records.\nRecord Count:"+QString::number(records.count()));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }

    }
    else
    {
         if(this->wcd100){
            wcd100->hide();
            wcd7->hide();
            wcdTrend->hide();

         }
    }
#endif
}


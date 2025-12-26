#include "WeightDatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>

// ------------------------------------------------------------
// Constructor / Destructor
// ------------------------------------------------------------

WeightDatabase::WeightDatabase(const QString& dbPath, QObject* parent)
    : QObject(parent), dbPath(dbPath)
{
    openDatabase();
    createTable();
}

WeightDatabase::~WeightDatabase() {
    closeDatabase();
}

// ------------------------------------------------------------
// Internal DB Management
// ------------------------------------------------------------

void WeightDatabase::openDatabase() {
    if (QSqlDatabase::contains("weightdb")) {
        db = QSqlDatabase::database("weightdb");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "weightdb");
        db.setDatabaseName(dbPath);
    }

    if (!db.open()) {
        qWarning() << "Error opening database:" << db.lastError();
    }
}

void WeightDatabase::closeDatabase() {
    if (db.isOpen())
        db.close();
}

void WeightDatabase::createTable() {
    QSqlQuery query(db);
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS weights (
            id TEXT PRIMARY KEY NOT NULL,
            weight REAL NOT NULL,
            timestamp REAL NOT NULL
        );
    )";

    if (!query.exec(sql)) {
        qWarning() << "Table creation failed:" << query.lastError();
    }
}

// ------------------------------------------------------------
// CRUD Operations
// ------------------------------------------------------------

bool WeightDatabase::insertRecord(const WeightRecord& record) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO weights (id, weight, timestamp) VALUES (?, ?, ?);");
    query.addBindValue(record.id.toString(QUuid::WithoutBraces));
    query.addBindValue(record.weight);
    query.addBindValue(record.timestamp.toSecsSinceEpoch());

    if (!query.exec()) {
        qWarning() << "Insert failed:" << query.lastError();
        return false;
    }
    return true;
}

QList<WeightRecord> WeightDatabase::fetchAll() {
    QList<WeightRecord> records;
    QSqlQuery query(db);

    if (!query.exec("SELECT id, weight, timestamp FROM weights ORDER BY timestamp DESC;")) {
        qWarning() << "Query failed:" << query.lastError();
        return records;
    }

    while (query.next()) {
        QUuid id(query.value(0).toString());
        double weight = query.value(1).toDouble();
        qint64 ts = query.value(2).toLongLong();

        records.append(WeightRecord(id, weight, QDateTime::fromSecsSinceEpoch(ts)));
    }

    return records;
}

bool WeightDatabase::updateRecord(const QUuid& id, double newWeight, const QDateTime& newTimestamp) {
    QString sql = "UPDATE weights SET weight = ?";
    bool hasTimestamp = newTimestamp.isValid();

    if (hasTimestamp)
        sql += ", timestamp = ?";

    sql += " WHERE id = ?;";

    QSqlQuery query(db);
    query.prepare(sql);
    query.addBindValue(newWeight);

    if (hasTimestamp)
        query.addBindValue(newTimestamp.toSecsSinceEpoch());

    query.addBindValue(id.toString(QUuid::WithoutBraces));

    if (!query.exec()) {
        qWarning() << "Update failed:" << query.lastError();
        return false;
    }
    return true;
}

bool WeightDatabase::deleteRecord(const QUuid& id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM weights WHERE id = ?;");
    query.addBindValue(id.toString(QUuid::WithoutBraces));

    if (!query.exec()) {
        qWarning() << "Delete failed:" << query.lastError();
        return false;
    }
    return true;
}

bool WeightDatabase::deleteAll() {
    QSqlQuery query(db);
    if (!query.exec("DELETE FROM weights;")) {
        qWarning() << "Delete all failed:" << query.lastError();
        return false;
    }
    return true;
}

// ------------------------------------------------------------
// Import / Export
// ------------------------------------------------------------

bool WeightDatabase::exportDatabase(const QString& destPath) {
    closeDatabase();
    bool ok = QFile::copy(dbPath, destPath);
    openDatabase();
    return ok;
}

bool WeightDatabase::importDatabase(const QString& srcPath) {
    closeDatabase();

    if (QFile::exists(dbPath))
        QFile::remove(dbPath);

    if (!QFile::copy(srcPath, dbPath)) {
        qWarning() << "Import failed";
        openDatabase();
        return false;
    }

    openDatabase();
    createTable();
    return true;
}

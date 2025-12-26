/**
 * @file WeightDatabase.h
 * @brief SQLite-backed database manager for weight tracking records.
 *
 * This file declares the WeightDatabase class, which provides full CRUD
 * (Create, Read, Update, Delete) operations for the `weights` table stored
 * in a SQLite database. It mirrors the Swift WeightDatabase used in the
 * mobile app and ensures schema compatibility for cross‑platform syncing.
 *
 * The schema managed by this class is:
 *   - id TEXT PRIMARY KEY
 *   - weight REAL
 *   - timestamp REAL (Unix epoch seconds)
 *
 * The class supports:
 *   - Inserting new WeightRecord entries
 *   - Fetching all records ordered by timestamp
 *   - Updating existing records
 *   - Deleting individual or all records
 *   - Importing and exporting the entire SQLite file
 *
 * @author
 *   Devesh Rai
 *
 * @see WeightRecord
 */

#ifndef WEIGHTDATABASE_H
#define WEIGHTDATABASE_H
#pragma once


#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include "WeightRecord.h"

/**
 * @class WeightDatabase
 * @brief Provides full CRUD and import/export operations for the weights SQLite database.
 *
 * This class encapsulates all database interactions for the weight‑tracking
 * application. It automatically opens the SQLite file, creates the schema
 * if missing, and exposes a clean API for reading and modifying data.
 *
 * The database connection uses Qt's SQL module and requires:
 *   `QT += core sql`
 */
class WeightDatabase : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs the WeightDatabase and opens/initializes the SQLite file.
     * @param dbPath Full filesystem path to the SQLite database file.
     */
    explicit WeightDatabase(const QString& dbPath, QObject* parent = nullptr);

    /**
     * @brief Closes the database connection and removes the Qt SQL connection.
     */
    ~WeightDatabase();

    // ---------------------------------------------------------------------
    // CRUD OPERATIONS
    // ---------------------------------------------------------------------

    /**
     * @brief Inserts a new WeightRecord into the database.
     * @param record The WeightRecord to insert.
     * @return true on success, false on failure.
     */
    bool insertRecord(const WeightRecord& record);

    /**
     * @brief Fetches all weight records ordered by timestamp (descending).
     * @return A QList of WeightRecord objects.
     */
    QList<WeightRecord> fetchAll();

    /**
     * @brief Updates an existing record's weight and optionally its timestamp.
     *
     * If `newTimestamp` is invalid, the timestamp remains unchanged.
     *
     * @param id The UUID of the record to update.
     * @param newWeight The new weight value.
     * @param newTimestamp Optional new timestamp.
     * @return true on success, false on failure.
     */
    bool updateRecord(const QUuid& id,
                      double newWeight,
                      const QDateTime& newTimestamp = QDateTime());

    /**
     * @brief Deletes a record by UUID.
     * @param id The UUID of the record to delete.
     * @return true on success, false on failure.
     */
    bool deleteRecord(const QUuid& id);

    /**
     * @brief Deletes all records from the weights table.
     * @return true on success, false on failure.
     */
    bool deleteAll();

    // ---------------------------------------------------------------------
    // IMPORT / EXPORT
    // ---------------------------------------------------------------------

    /**
     * @brief Exports the SQLite database file to another location.
     * @param destPath Destination file path.
     * @return true on success, false on failure.
     */
    bool exportDatabase(const QString& destPath);

    /**
     * @brief Imports a SQLite database file, replacing the current one.
     * @param srcPath Source file path.
     * @return true on success, false on failure.
     */
    bool importDatabase(const QString& srcPath);

private:
    QString dbPath;      ///< Filesystem path to the SQLite database file.
    QSqlDatabase db;     ///< Qt SQL database connection.

    /**
     * @brief Opens the SQLite database connection.
     */
    void openDatabase();

    /**
     * @brief Closes the SQLite database connection.
     */
    void closeDatabase();

    /**
     * @brief Creates the weights table if it does not already exist.
     */
    void createTable();
};

#endif // WEIGHTDATABASE_H

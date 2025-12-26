/**
 * @file WeightRecord.h
 * @brief Data structure representing a single weight entry.
 *
 * This struct mirrors the Swift WeightRecord used in the mobile app.
 * It stores:
 *   - A UUID identifier
 *   - A weight value
 *   - A timestamp (QDateTime)
 *
 * @author
 *   Devesh Rai
 */

#ifndef WEIGHTRECORD_H
#define WEIGHTRECORD_H

#pragma once


#include <QString>
#include <QDateTime>
#include <QUuid>

/**
 * @struct WeightRecord
 * @brief Represents a single weight measurement entry.
 */
struct WeightRecord {
    QUuid id;            ///< Unique identifier for the record.
    double weight;       ///< Weight value.
    QDateTime timestamp; ///< Timestamp of the measurement.

    WeightRecord() = default;

    /**
     * @brief Constructs a WeightRecord with explicit values.
     */
    WeightRecord(const QUuid& id_, double weight_, const QDateTime& ts_)
        : id(id_), weight(weight_), timestamp(ts_) {}

    /**
     * @brief Creates a new WeightRecord with a fresh UUID and current timestamp.
     * @param weight The weight value.
     * @return A new WeightRecord instance.
     */
    static WeightRecord create(double weight) {
        return WeightRecord(
            QUuid::createUuid(),
            weight,
            QDateTime::currentDateTime()
            );
    }
};

#endif // WEIGHTRECORD_H

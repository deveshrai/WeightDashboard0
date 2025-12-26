#ifndef WEIGHTCHARTS_H
#define WEIGHTCHARTS_H

#pragma once

#include <QList>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QVector>
#include "WeightDatabase.h"

//QT_CHARTS_USE_NAMESPACE


//     struct WeightRecord {
//     QDateTime timestamp;
//     double weight;
// };

class WeightCharts
{
public:
    explicit WeightCharts(const QList<WeightRecord> &records);

    QChart* buildRolling7Chart();
    QChart* buildLast100Chart();
    QChart* buildTrendlineChart();

private:
    QList<WeightRecord> m_records;

    QVector<QPointF> computeRolling7() const;
    QVector<QPointF> computeLast100() const;
    QVector<QPointF> computeTrendline() const;
    QVector<QPointF> computeProjection(const QVector<QPointF> &trend) const;

    void styleChart(QChart *chart, const QString &title) const;
};

#endif // WEIGHTCHARTS_H

#include "WeightCharts.h"

#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QChart>
#include <QChartView>

#include <QtMath>
#include <QDebug>


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

WeightCharts::WeightCharts(const QList<WeightRecord> &records)
    : m_records(records)
{
}

// ------------------------------------------------------------
// INTERNAL COMPUTATION
// ------------------------------------------------------------

QVector<QPointF> WeightCharts::computeRolling7() const
{
    QVector<QPointF> out;
    const int window = 7;

    for (int i = 0; i < m_records.size(); ++i) {
        int start = qMax(0, i - window + 1);
        double sum = 0;
        int count = 0;

        for (int j = start; j <= i; ++j) {
            sum += m_records[j].weight;
            count++;
        }

        double avg = sum / count;
        out.append(QPointF(m_records[i].timestamp.toMSecsSinceEpoch(), avg));
    }

    return out;
}

QVector<QPointF> WeightCharts::computeLast100() const
{
    QVector<QPointF> out;
    int start = qMax(0, m_records.size() - 100);

    for (int i = start; i < m_records.size(); ++i) {
        out.append(QPointF(
            m_records[i].timestamp.toMSecsSinceEpoch(),
            m_records[i].weight
            ));
    }

    return out;
}

QVector<QPointF> WeightCharts::computeTrendline() const
{
    QVector<QPointF> out;
    if (m_records.isEmpty())
        return out;

    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
    int n = m_records.size();

    for (const auto &r : m_records) {
        double x = r.timestamp.toMSecsSinceEpoch() / 1000.0;
        double y = r.weight;

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumXX += x * x;
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumXX - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    for (const auto &r : m_records) {
        double x = r.timestamp.toMSecsSinceEpoch() / 1000.0;
        double y = slope * x + intercept;
        out.append(QPointF(r.timestamp.toMSecsSinceEpoch(), y));
    }

    return out;
}

QVector<QPointF> WeightCharts::computeProjection(const QVector<QPointF> &trend) const
{
    QVector<QPointF> out;
    if (trend.isEmpty() || m_records.isEmpty())
        return out;

    double x1 = trend.first().x() / 1000.0;
    double y1 = trend.first().y();
    double x2 = trend.last().x() / 1000.0;
    double y2 = trend.last().y();

    double slope = (y2 - y1) / (x2 - x1);
    double intercept = y1 - slope * x1;

    QDateTime last = m_records.last().timestamp;

    static constexpr int PROJECTION_DAYS = 7;

    for (int i = 1; i <= PROJECTION_DAYS; ++i) {
        QDateTime future = last.addDays(i);
        double x = future.toMSecsSinceEpoch() / 1000.0;
        double y = slope * x + intercept;
        out.append(QPointF(future.toMSecsSinceEpoch(), y));
    }

    return out;
}

// ------------------------------------------------------------
// AXIS + CHART STYLING
// ------------------------------------------------------------

void WeightCharts::styleChart(QChart *chart, const QString &title) const
{
    chart->setTitle(title);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Human-readable date axis
    auto *axisX = new QDateTimeAxis();
    axisX->setFormat("dd MMM yyyy");
    axisX->setTitleText("Date");
    axisX->setTickCount(8);

    // Weight axis
    auto *axisY = new QValueAxis();
    axisY->setLabelFormat("%.1f");
    axisY->setTitleText("Weight");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
}

// ------------------------------------------------------------
// PUBLIC CHART BUILDERS
// ------------------------------------------------------------

QChart* WeightCharts::buildRolling7Chart()
{
    auto data = computeRolling7();
    auto *series = new QLineSeries();
    series->setName("7-Day Rolling Average");

    for (auto &p : data)
        series->append(p);

    QPen pen(Qt::blue);
    pen.setWidth(3);
    series->setPen(pen);

    QChart *chart = new QChart();
    chart->addSeries(series);
    styleChart(chart, "7-Day Rolling Average");

    auto *axisX = chart->axes(Qt::Horizontal).first();
    auto *axisY = chart->axes(Qt::Vertical).first();
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    return chart;
}

QChart* WeightCharts::buildLast100Chart()
{
    auto data = computeLast100();
    auto *series = new QLineSeries();
    series->setName("Last 100 Entries");

    for (auto &p : data)
        series->append(p);

    QPen pen(Qt::darkGreen);
    pen.setWidth(2);
    series->setPen(pen);

    QChart *chart = new QChart();
    chart->addSeries(series);
    styleChart(chart, "Last 100 Entries");

    auto *axisX = chart->axes(Qt::Horizontal).first();
    auto *axisY = chart->axes(Qt::Vertical).first();
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    return chart;
}

QChart* WeightCharts::buildTrendlineChart()
{
    // Actual data should use ALL records, not last 100
    QVector<QPointF> actual;
    for (const auto &r : m_records) {
        actual.append(QPointF(r.timestamp.toMSecsSinceEpoch(), r.weight));
    }

    auto trend = computeTrendline();        // full trendline
    auto proj  = computeProjection(trend);  // 60-day projection

    // -------------------------------
    // Actual data series (thin gray)
    // -------------------------------
    auto *seriesActual = new QLineSeries();
    seriesActual->setName("Actual Data");

    for (auto &p : actual)
        seriesActual->append(p);

    QPen actualPen(Qt::darkGray);
    actualPen.setWidth(1);
    seriesActual->setPen(actualPen);

    // -------------------------------
    // Trendline (solid red)
    // -------------------------------
    auto *seriesTrend = new QLineSeries();
    seriesTrend->setName("Trendline");

    for (auto &p : trend)
        seriesTrend->append(p);

    QPen trendPen(Qt::red);
    trendPen.setWidth(3);
    seriesTrend->setPen(trendPen);

    // -------------------------------
    // Projection (dotted red)
    // -------------------------------
    auto *seriesProj = new QLineSeries();
    seriesProj->setName("Projection");

    for (auto &p : proj)
        seriesProj->append(p);

    QPen projPen(Qt::red);
    projPen.setStyle(Qt::DashLine);
    projPen.setWidth(2);
    seriesProj->setPen(projPen);

    // -------------------------------
    // Build chart
    // -------------------------------
    QChart *chart = new QChart();
    chart->addSeries(seriesActual);
    chart->addSeries(seriesTrend);
    chart->addSeries(seriesProj);

    styleChart(chart, "Trendline + 7-Day Projection");

    auto *axisX = chart->axes(Qt::Horizontal).first();
    auto *axisY = chart->axes(Qt::Vertical).first();

    for (auto *s : chart->series()) {
        s->attachAxis(axisX);
        s->attachAxis(axisY);
    }

    // -------------------------------
    // Ensure projection is visible
    // -------------------------------
    qint64 minX = m_records.first().timestamp.toMSecsSinceEpoch();
    qint64 maxX = m_records.last().timestamp.toMSecsSinceEpoch();

    if (!proj.isEmpty()) {
        maxX = qMax(maxX, (qint64)proj.last().x());
    }

    auto *dtAxis = qobject_cast<QDateTimeAxis*>(axisX);
    dtAxis->setRange(QDateTime::fromMSecsSinceEpoch(minX),
                     QDateTime::fromMSecsSinceEpoch(maxX));

    return chart;
}


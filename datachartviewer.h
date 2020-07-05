#ifndef DATACHARTVIEWER_H
#define DATACHARTVIEWER_H

#include <QChartView>

using namespace QtCharts;

class SignalInfo {
public:
    SignalInfo(const QString& name, const QColor& color, Qt::Alignment alignment = 0) : m_name(name), m_color(color), m_alignment(alignment) {}

    QString name() const { return m_name; }
    QColor color() const { return m_color; }
    Qt::Alignment alignment() const { return m_alignment; }

private:
    QString m_name;
    QColor m_color;
    Qt::Alignment m_alignment;

};

class DataChartViewer : public QtCharts::QChartView
{
    Q_OBJECT
public:
    DataChartViewer(const QList<SignalInfo>& signalInfo);
    ~DataChartViewer();

public slots:
    void receiveDataRow(QList<QPointF>);

signals:
    void sendDataRow(QList<QPointF>);

private:
    QChart* chart;
};

#endif // DATACHARTVIEWER_H

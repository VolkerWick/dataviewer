#ifndef DATACHARTVIEWER_H
#define DATACHARTVIEWER_H

#include <QChartView>

using namespace QtCharts;

class SignalInfo {
public:
    enum  Align { none = 0, left = 1, right = 2, };

    SignalInfo(const QString& name, const QColor& color, Align align = Align::none) : m_name(name), m_color(color), m_align(align) {}

    QString name() const { return m_name; }
    QColor color() const { return m_color; }
    Align align() const { return m_align; }
    bool isAligned() const { return align() != Align::none; }

    Qt::Alignment alignment() const;

private:
    QString m_name;
    QColor m_color;
    Align m_align;
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

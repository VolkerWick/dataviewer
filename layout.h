#ifndef LAYOUT_H
#define LAYOUT_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QColor>

#define ALIGN_INVALID (Qt::AlignTop | Qt::AlignBottom)

struct AxisInfo {
    AxisInfo(const QJsonObject& o);
    AxisInfo() : title(""), alignment(ALIGN_INVALID) {}

    QString getTitle() const;
    Qt::Alignment getAlignment() const;

private:
    QString title;
    Qt::Alignment alignment;
};

struct SignalInfoEx {
    SignalInfoEx(const QJsonObject& o);

    QString getSignalName() const;
    QColor getColor() const;
    AxisInfo getAxisInfo() const;

private:
    QString signalName;
    QColor color;
    AxisInfo axisInfo;
};

struct ChartInfo {
    ChartInfo(const QJsonObject& o);

    QString getTitle() const;
    QList<SignalInfoEx> getSignalInfo() const;

private:
    QString title;
    QList<SignalInfoEx> signalInfo;
};

class Layout
{
public:
    Layout(const QString& fileName);
    QList<ChartInfo> getChartInfo() const;

private:
    QJsonDocument doc;
    QList<ChartInfo> chartInfo;
};

#endif // LAYOUT_H

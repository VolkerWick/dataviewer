#include "layout.h"

#include <QJsonArray>

#include <QFile>
#include <QDebug>

QDebug operator<<(QDebug s, const AxisInfo& axisInfo) {
    s << "\n\t\tAxisInfo:" << axisInfo.getTitle() << axisInfo.getAlignment();
    return s;
}

QDebug operator<<(QDebug s, const SignalInfoEx& signl) {
    s << "\n\tSignalInfoEx:" << signl.getSignalName() << "Color" << signl.getColor() << signl.getAxisInfo() << "\n";
    return s;
}

QDebug operator<<(QDebug s, const ChartInfo& chartInfo) {
    s << "\nChartInfo:" << chartInfo.getTitle();
    for (auto info: chartInfo.getSignalInfo()) {
        s << info;
    }
    return s;
}

static QJsonDocument readJson(const QString& fileName) {
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly|QIODevice::Text)) {
        throw new std::runtime_error(QObject::tr("Unable to open %s (%s)").arg(fileName, f.errorString()).toStdString().c_str());
    }

    QJsonParseError error;
    QJsonDocument result = QJsonDocument::fromJson(f.readAll(),&error);
    if (error.error != QJsonParseError::NoError) {
        throw new std::runtime_error(QObject::tr("Unable to parse %s (%s)").arg(fileName, error.errorString()).toStdString().c_str());
    }

    return result;
}

Layout::Layout(const QString& fileName)
{
    QJsonArray chartArray = readJson(fileName).array();
    std::for_each(chartArray.cbegin(), chartArray.cend(),[=](const QJsonValue& chart) {
        chartInfo << ChartInfo(chart.toObject());
    });

    qDebug() << chartInfo;
}

QList<ChartInfo> Layout::getChartInfo() const
{
    return chartInfo;
}

ChartInfo::ChartInfo(const QJsonObject& o)
    : title(o.value("title").toString())
{
    QJsonArray signalList = o.value("signal").toArray();

    std::for_each(signalList.cbegin(), signalList.cend(),[=](const QJsonValue& signl){
        signalInfo << SignalInfoEx(signl.toObject());
    });
}

QString ChartInfo::getTitle() const
{
    return title;
}

QList<SignalInfoEx> ChartInfo::getSignalInfo() const
{
    return signalInfo;
}

SignalInfoEx::SignalInfoEx(const QJsonObject& o)
    : signalName(o.value("name").toString())
    , color(QColor(o.value("color").toString()))
{
    if (!o.value("axis").isUndefined()) {
        axisInfo = AxisInfo(o.value("axis").toObject());
    }
}

QString SignalInfoEx::getSignalName() const
{
    return signalName;
}

QColor SignalInfoEx::getColor() const
{
    return color;
}

AxisInfo SignalInfoEx::getAxisInfo() const
{
    return axisInfo;
}

AxisInfo::AxisInfo(const QJsonObject& o)
    : title(o.value("title").toString())
    , alignment(ALIGN_INVALID)
{
    QString s = o.value("align").toString();
    if (s == "left") {
        alignment = Qt::AlignLeft;
    } else if (s == "right") {
        alignment = Qt::AlignRight;
    }
}

QString AxisInfo::getTitle() const
{
    return title;
}

Qt::Alignment AxisInfo::getAlignment() const
{
    return alignment;
}

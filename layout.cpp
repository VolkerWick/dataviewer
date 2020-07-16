#include "layout.h"

#include <QJsonArray>
#include <QFile>
#include <QDebug>

// json field names
const char* const TITLE = "title";
const char* const SIGNAL = "signal";
const char* const AXIS = "axis";
const char* const NAME = "name";
const char* const COLOR = "color";
const char* const ALIGN = "align";
const char* const LEFT = "left";
const char* const RIGHT = "right";

QDebug operator<<(QDebug s, const AxisInfo& axisInfo) {
    s << "\n\t\tAxisInfo:" << axisInfo.getTitle() << axisInfo.getAlignment();
    return s;
}

QDebug operator<<(QDebug s, const SignalInfo& signl) {
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
        throw new std::runtime_error(QObject::tr("Unable to open %1.\n%2")
                                     .arg(fileName, f.errorString()).toStdString().c_str());
    }

    QJsonParseError error;
    QJsonDocument result = QJsonDocument::fromJson(f.readAll(),&error);
    if (error.error != QJsonParseError::NoError) {
        throw new std::runtime_error(QObject::tr("Unable to parse %1.\n%2\nApproximate location: %3")
                                     .arg(fileName, error.errorString())
                                     .arg(error.offset).toStdString().c_str());
    }

    return result;
}

static QString getValueString(const QJsonObject& o, const QString& key) {
    QString result;
    if (o.contains(key)) {
        result = o.value(key).toString();
    } else {
        qWarning() << "Missing key" << key << " in " << o;
    }

    return result;
}

Layout::Layout(const QString& fileName)
{
    QJsonDocument doc = readJson(fileName);
    if (!doc.isArray()) {
        throw new std::runtime_error(QObject::tr("Json Document in %1 must be an array, i.e. start and end with square brackets.")
                                     .arg(fileName).toStdString().c_str());
    }

    QJsonArray chartArray = doc.array();
    std::for_each(chartArray.begin(), chartArray.end(),[=](const QJsonValue& chart) {
        chartInfo << ChartInfo(chart.toObject());
    });

    qDebug() << chartInfo;
}

QList<ChartInfo> Layout::getChartInfo() const
{
    return chartInfo;
}

ChartInfo::ChartInfo(const QJsonObject& o)
    : title(getValueString(o, TITLE))
{
    if (o.contains(SIGNAL)) {
        QJsonArray signalList = o.value(SIGNAL).toArray();

        std::for_each(signalList.begin(), signalList.end(),[=](const QJsonValue& signl){
            signalInfo << SignalInfo(signl.toObject());
        });
    } else {
        qWarning() << "Missing key" << SIGNAL << " in " << o;
    }
}

QString ChartInfo::getTitle() const
{
    return title;
}

QList<SignalInfo> ChartInfo::getSignalInfo() const
{
    return signalInfo;
}

SignalInfo::SignalInfo(const QJsonObject& o)
    : signalName(getValueString(o, NAME))
    , color(QColor(getValueString(o, COLOR)))
{
    if (!o.value(AXIS).isUndefined()) {
        axisInfo = AxisInfo(o.value(AXIS).toObject());
    }
}

QString SignalInfo::getSignalName() const
{
    return signalName;
}

QColor SignalInfo::getColor() const
{
    return color;
}

AxisInfo SignalInfo::getAxisInfo() const
{
    return axisInfo;
}

AxisInfo::AxisInfo(const QJsonObject& o)
    : title(o.value("title").toString())
    , alignment(ALIGN_INVALID)
{
    QString s = getValueString(o, ALIGN);
    if (s == LEFT) {
        alignment = Qt::AlignLeft;
    } else if (s == RIGHT) {
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

#ifndef SIGNALINFO_H
#define SIGNALINFO_H

#include <QColor>
#include <QJsonObject>
#include <QJsonValue>

// Class SignalInfo holds the layout information for a signal containing name, color on the chart and axis alignment.
// Axis alignment can be left, right or none in which case the alignment is inherited from a previous signal
// on the same chart. The first signal on a chart must have an alignment.
class SignalInfo {
public:
    // alignment of the signal on the chart
    // for value axes only, time axis is always hcenter aligned
    enum class Align { none, left, right, };

    SignalInfo(const QJsonObject& o);

    QString name() const { return m_name; }
    QColor color() const { return m_color; }
    Align align() const { return m_align; }
    bool isAligned() const { return align() != Align::none; }

    // get an alignment value that can be used to set the axis
    Qt::Alignment alignment() const;

private:
    // get alignment value from Json Value
    static Align fromValue(const QJsonValue& v);

    QString m_name;
    QColor m_color;
    Align m_align;
};


#endif // SIGNALINFO_H

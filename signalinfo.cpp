#include "signalinfo.h"

SignalInfo::SignalInfo(const QJsonObject &o)
    : m_name(o.value("name").toString())
    , m_color(o.value("color").toString())
    , m_align(fromValue(o.value("align")))
{}

SignalInfo::Align SignalInfo::fromValue(const QJsonValue &v) {
    Align align = Align::none;
    if (v.toString() == "left") {
        align = Align::left;
    } else if (v.toString() == "right") {
        align = Align::right;
    }
    return align;
}

Qt::Alignment SignalInfo::alignment() const
{
    if (!isAligned()) {
        throw new std::runtime_error("Unable to return alignment when align==none");
    }

    return align() == Align::left ? Qt::AlignLeft : Qt::AlignRight;
}

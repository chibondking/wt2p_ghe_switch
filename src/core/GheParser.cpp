#include "GheParser.h"

GheParser::GheParser(QObject *parent)
    : QObject(parent)
{
}

GheParser::~GheParser() = default;

void GheParser::feed(const QString &line)
{
    // Discard heartbeat noise.
    if (line == u"." || line.isEmpty())
        return;

    // TODO: split on '.', dispatch on line.section('.', 0, 0):
    //   "SWITCHADD"    -> parse radio/antenna list, emit switchAdded(...)
    //   "SWITCHUPDATE" -> parse active antenna + dBm, emit switchUpdated(...)
    //   "SWITCHLOCKS"  -> parse lock state, emit switchLocked(...)
    Q_UNUSED(line)
}

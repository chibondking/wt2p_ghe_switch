#include "GheParser.h"

GheParser::GheParser(QObject *parent)
    : QObject(parent)
{
    // TODO: initialise internal state machine / ring buffer.
}

GheParser::~GheParser() = default;

void GheParser::feed(const QByteArray &data)
{
    Q_UNUSED(data)
    // TODO: accumulate bytes, extract complete frames, emit parsed messages.
}

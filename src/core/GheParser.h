#pragma once

#include <QObject>
#include <QByteArray>

// TODO: Parse raw bytes from the GHE Everywhere TCP stream into structured
// command/response objects. Handle framing, checksums, and protocol version
// negotiation per the GHE Everywhere documentation.
class GheParser : public QObject
{
    Q_OBJECT

public:
    explicit GheParser(QObject *parent = nullptr);
    ~GheParser() override;

    void feed(const QByteArray &data);
};

#pragma once

#include <QObject>
#include <QString>

// TODO: Parse newline-terminated, dot-delimited ASCII messages from the GHE
// Everywhere TCP stream. Input arrives as complete lines (split by GheClient).
// Responsibilities: identify message type (SWITCHADD, SWITCHUPDATE, SWITCHLOCKS),
// split on '.', validate field counts, emit typed signals to SwitchController.
// Pure parsing — no network I/O, no Qt dependencies beyond QString/QStringList.
class GheParser : public QObject
{
    Q_OBJECT

public:
    explicit GheParser(QObject *parent = nullptr);
    ~GheParser() override;

    // Feed one complete newline-terminated line (newline already stripped).
    // Silently discards lone "." heartbeat messages.
    void feed(const QString &line);
};

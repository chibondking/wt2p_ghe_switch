#pragma once

#include <QObject>

class QTcpSocket;

// TODO: Wrap QTcpSocket to manage the TCP connection to the GHE Everywhere
// server. Responsibilities: connect/disconnect, send raw command strings,
// emit signals on incoming data and connection state changes.
class GheClient : public QObject
{
    Q_OBJECT

public:
    explicit GheClient(QObject *parent = nullptr);
    ~GheClient() override;

private:
    QTcpSocket *m_socket{nullptr};
};

#pragma once

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;

// Owns the TCP connection to one GHE Everywhere server.
// Responsibilities: connect/disconnect, buffer incoming bytes, split on newline,
// emit lineReceived() for each complete message, emit connection state signals.
class GheClient : public QObject
{
    Q_OBJECT

public:
    explicit GheClient(QObject *parent = nullptr);
    ~GheClient() override;

    void connectToHost(const QString &host, quint16 port);
    void disconnectFromHost();

    bool isConnected() const;

signals:
    void connected();
    void disconnected();
    void lineReceived(const QString &line);
    void errorOccurred(const QString &message);

private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    QTcpSocket *m_socket{nullptr};
    QByteArray  m_buffer;
};

#include "GheClient.h"

#include <QTcpSocket>

GheClient::GheClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected,
            this,     &GheClient::connected);

    connect(m_socket, &QTcpSocket::disconnected,
            this,     &GheClient::disconnected);

    connect(m_socket, &QTcpSocket::readyRead,
            this,     &GheClient::onReadyRead);

    connect(m_socket, &QTcpSocket::errorOccurred,
            this,     &GheClient::onSocketError);
}

GheClient::~GheClient() = default;

void GheClient::connectToHost(const QString &host, quint16 port)
{
    m_buffer.clear();
    m_socket->connectToHost(host, port);
}

void GheClient::disconnectFromHost()
{
    m_socket->disconnectFromHost();
}

bool GheClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void GheClient::onReadyRead()
{
    m_buffer += m_socket->readAll();

    // Split on newline, feed complete lines to parser.
    int pos;
    while ((pos = m_buffer.indexOf('\n')) != -1) {
        const QString line = QString::fromUtf8(m_buffer.left(pos)).trimmed();
        m_buffer.remove(0, pos + 1);
        emit lineReceived(line);
    }
}

void GheClient::onSocketError(QAbstractSocket::SocketError /*error*/)
{
    emit errorOccurred(m_socket->errorString());
}

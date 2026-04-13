#include "GheClient.h"

#include <QTcpSocket>

GheClient::GheClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    // TODO: connect QTcpSocket signals (connected, disconnected,
    // readyRead, errorOccurred) to slots implemented here.
}

GheClient::~GheClient() = default;

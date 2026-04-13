#include "SwitchController.h"

#include "GheClient.h"
#include "GheParser.h"
#include "AntennaModel.h"

SwitchController::SwitchController(QObject *parent)
    : QObject(parent)
    , m_client(new GheClient(this))
    , m_parser(new GheParser(this))
    , m_model(new AntennaModel(this))
{
    // TODO: wire GheClient::dataReceived -> GheParser::feed,
    // GheParser::messageParsed -> update AntennaModel.
}

SwitchController::~SwitchController() = default;

void SwitchController::connectToHost(const QString &host, quint16 port)
{
    Q_UNUSED(host)
    Q_UNUSED(port)
    // TODO: delegate to m_client->connectToHost(host, port).
}

void SwitchController::disconnectFromHost()
{
    // TODO: delegate to m_client->disconnectFromHost().
}

void SwitchController::selectPort(int portIndex)
{
    Q_UNUSED(portIndex)
    // TODO: build GHE select-port command and send via m_client.
}

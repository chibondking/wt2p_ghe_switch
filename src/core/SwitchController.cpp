#include "SwitchController.h"

#include "GheClient.h"
#include "GheParser.h"
#include "AntennaModel.h"

SwitchController::SwitchController(const ServerConfig &cfg, QObject *parent)
    : QObject(parent)
    , m_config(cfg)
    , m_client(new GheClient(this))
    , m_parser(new GheParser(this))
    , m_model(new AntennaModel(this))
{
    connect(m_client, &GheClient::connected,
            this,     &SwitchController::connected);

    connect(m_client, &GheClient::disconnected,
            this,     &SwitchController::disconnected);

    connect(m_client, &GheClient::errorOccurred,
            this,     &SwitchController::connectionError);

    connect(m_client, &GheClient::lineReceived,
            m_parser, &GheParser::feed);

    // TODO: connect GheParser signals to AntennaModel updates once parsing is implemented.
}

SwitchController::~SwitchController() = default;

bool SwitchController::isConnected() const
{
    return m_client->isConnected();
}

void SwitchController::connectToServer()
{
    m_client->connectToHost(m_config.host, m_config.port);
}

void SwitchController::disconnectFromServer()
{
    m_client->disconnectFromHost();
}

void SwitchController::selectPort(int portIndex)
{
    Q_UNUSED(portIndex)
    // TODO: build SET_SWITCH.<radio>.<antenna> command and send via m_client.
}

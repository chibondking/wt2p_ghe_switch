#pragma once

#include <QObject>
#include "config/ServerConfig.h"

class GheClient;
class GheParser;
class AntennaModel;

// Owns one GHE Everywhere connection (GheClient + GheParser + AntennaModel).
// One SwitchController instance per configured server.
class SwitchController : public QObject
{
    Q_OBJECT

public:
    explicit SwitchController(const ServerConfig &cfg, QObject *parent = nullptr);
    ~SwitchController() override;

    const ServerConfig &serverConfig() const { return m_config; }
    AntennaModel       *model()        const { return m_model; }
    bool                isConnected()  const;

public slots:
    void connectToServer();
    void disconnectFromServer();
    void selectPort(int portIndex);

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &message);

private:
    ServerConfig  m_config;
    GheClient    *m_client{nullptr};
    GheParser    *m_parser{nullptr};
    AntennaModel *m_model{nullptr};
};

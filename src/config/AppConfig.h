#pragma once

#include "ServerConfig.h"
#include <QList>
#include <QString>

// Singleton. Loads/saves the application XML config file.
// Config path (platform-standard):
//   Linux:   ~/.config/GHEControl/config.xml
//   Windows: %APPDATA%\GHEControl\config.xml
class AppConfig
{
public:
    static AppConfig &instance();

    // Load from disk. Safe to call even if the file doesn't exist yet.
    void load();

    // Write current state to disk.
    void save() const;

    QList<ServerConfig> servers() const;
    void setServers(const QList<ServerConfig> &servers);

    // Update a single server by id (no-op if id not found).
    void updateServer(const ServerConfig &cfg);

    // Convenience: flip the autoConnect flag for one server and save.
    void setAutoConnect(const QString &id, bool autoConnect);

private:
    AppConfig() = default;

    QString             configFilePath() const;
    QList<ServerConfig> m_servers;
};

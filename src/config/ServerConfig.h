#pragma once

#include <QString>
#include <QUuid>

struct ServerConfig {
    QString  id;              // stable UUID — never changes after creation
    QString  host;
    quint16  port{10072};
    QString  username;
    QString  password;
    bool     autoConnect{false};

    // Factory so every new config gets a unique id automatically.
    static ServerConfig create() {
        ServerConfig cfg;
        cfg.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        return cfg;
    }
};

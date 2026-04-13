#include "AppConfig.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

AppConfig &AppConfig::instance()
{
    static AppConfig s_instance;
    return s_instance;
}

QString AppConfig::configFilePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
           + QStringLiteral("/config.xml");
}

void AppConfig::load()
{
    m_servers.clear();

    QFile file(configFilePath());
    if (!file.open(QIODevice::ReadOnly))
        return; // first run — no config yet, that's fine

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        if (xml.readNextStartElement()) {
            if (xml.name() == u"server") {
                ServerConfig cfg;
                cfg.id          = xml.attributes().value(QLatin1StringView("id")).toString();
                cfg.autoConnect = (xml.attributes().value(QLatin1StringView("autoConnect")) == u"true");

                // If the file has a server without an id (hand-edited, old version), mint one.
                if (cfg.id.isEmpty())
                    cfg.id = QUuid::createUuid().toString(QUuid::WithoutBraces);

                while (!(xml.isEndElement() && xml.name() == u"server")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue;

                    if (xml.name() == u"host")
                        cfg.host = xml.readElementText();
                    else if (xml.name() == u"port")
                        cfg.port = static_cast<quint16>(xml.readElementText().toUShort());
                    else if (xml.name() == u"username")
                        cfg.username = xml.readElementText();
                    else if (xml.name() == u"password")
                        cfg.password = xml.readElementText();
                }
                m_servers.append(cfg);
            }
        }
    }
}

void AppConfig::save() const
{
    const QString path = configFilePath();
    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement(QStringLiteral("GHEControl"));
    xml.writeAttribute(QStringLiteral("version"), QStringLiteral("1"));

    xml.writeStartElement(QStringLiteral("servers"));
    for (const ServerConfig &cfg : m_servers) {
        xml.writeStartElement(QStringLiteral("server"));
        xml.writeAttribute(QStringLiteral("id"),          cfg.id);
        xml.writeAttribute(QStringLiteral("autoConnect"), cfg.autoConnect ? QStringLiteral("true") : QStringLiteral("false"));
        xml.writeTextElement(QStringLiteral("host"),     cfg.host);
        xml.writeTextElement(QStringLiteral("port"),     QString::number(cfg.port));
        xml.writeTextElement(QStringLiteral("username"), cfg.username);
        xml.writeTextElement(QStringLiteral("password"), cfg.password);
        xml.writeEndElement(); // server
    }
    xml.writeEndElement(); // servers
    xml.writeEndElement(); // GHEControl
    xml.writeEndDocument();
}

QList<ServerConfig> AppConfig::servers() const
{
    return m_servers;
}

void AppConfig::setServers(const QList<ServerConfig> &servers)
{
    m_servers = servers;
}

void AppConfig::updateServer(const ServerConfig &cfg)
{
    for (ServerConfig &s : m_servers) {
        if (s.id == cfg.id) {
            s = cfg;
            return;
        }
    }
}

void AppConfig::setAutoConnect(const QString &id, bool autoConnect)
{
    for (ServerConfig &s : m_servers) {
        if (s.id == id) {
            s.autoConnect = autoConnect;
            save();
            return;
        }
    }
}

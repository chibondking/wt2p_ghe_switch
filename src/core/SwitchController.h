#pragma once

#include <QObject>

class GheClient;
class GheParser;
class AntennaModel;

// TODO: Orchestrate the relationship between GheClient, GheParser, and
// AntennaModel. Owns all three objects. Translates user actions (select port)
// into outgoing GHE commands and incoming parsed responses into model updates.
class SwitchController : public QObject
{
    Q_OBJECT

public:
    explicit SwitchController(QObject *parent = nullptr);
    ~SwitchController() override;

    AntennaModel *model() const { return m_model; }

public slots:
    void connectToHost(const QString &host, quint16 port);
    void disconnectFromHost();
    void selectPort(int portIndex);

private:
    GheClient    *m_client{nullptr};
    GheParser    *m_parser{nullptr};
    AntennaModel *m_model{nullptr};
};

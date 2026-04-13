#pragma once

#include <QMainWindow>
#include <QMap>
#include "config/ServerConfig.h"

class SwitchController;
class SetupDialog;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void openSetupDialog();
    void onConnectRequested(const ServerConfig &cfg);
    void onDisconnectRequested(const QString &serverId);
    void onServersChanged(const QList<ServerConfig> &servers);
    void updateStatusLabel();

private:
    void autoConnectFromConfig();
    void doConnect(const ServerConfig &cfg);
    void doDisconnect(const QString &serverId);

    // serverId → active SwitchController
    QMap<QString, SwitchController *> m_controllers;

    SetupDialog *m_setupDialog{nullptr};
    QLabel      *m_statusLabel{nullptr};
};

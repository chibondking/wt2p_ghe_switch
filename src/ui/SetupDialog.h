#pragma once

#include <QDialog>
#include <QList>
#include "config/ServerConfig.h"

class QTableWidget;
class QCheckBox;
class QPushButton;

// Non-modal "Servers" configuration dialog.
// Displays all configured servers, lets the user edit credentials,
// and connect/disconnect each one independently.
//
// Signal flow with MainWindow:
//   connectRequested(cfg)   → MainWindow creates a SwitchController
//   disconnectRequested(id) → MainWindow tears down the SwitchController
//   serversChanged(list)    → MainWindow persists updated server list
//
// MainWindow calls setRowState(id, connected) when connection state changes.
class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog() override;

    // Populate the table from a list of server configs.
    void setServers(const QList<ServerConfig> &servers);

    // Called by MainWindow when a connection succeeds or drops.
    void setRowState(const QString &serverId, bool connected);

signals:
    void connectRequested(const ServerConfig &cfg);
    void disconnectRequested(const QString &serverId);
    void serversChanged(const QList<ServerConfig> &servers);

private slots:
    void addRow(const ServerConfig &cfg = {});
    void removeSelectedRow();
    void onConnectButtonClicked();
    void onCellChanged();

private:
    struct RowWidgets {
        class QLineEdit   *hostEdit{nullptr};
        class QLineEdit   *portEdit{nullptr};
        class QLineEdit   *usernameEdit{nullptr};
        class QLineEdit   *passwordEdit{nullptr};
        class QPushButton *connectButton{nullptr};
        QString            serverId;
        bool               connected{false};
    };

    void         insertRow(int row, const ServerConfig &cfg, bool connected = false);
    ServerConfig rowToConfig(int row) const;
    void         updateConnectButton(int row, bool connected);
    void         emitServersChanged();
    int          rowForId(const QString &id) const;

    QTableWidget      *m_table{nullptr};
    QCheckBox         *m_showPasswords{nullptr};
    QList<RowWidgets>  m_rows;
    bool               m_suppressChanges{false};
};

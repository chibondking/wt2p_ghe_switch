#include "MainWindow.h"

#include "config/AppConfig.h"
#include "core/SwitchController.h"
#include "ui/SetupDialog.h"

#include <QLabel>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("GHEControl v0.1.0");
    setMinimumSize(200, 350);

    // ── Menu bar ─────────────────────────────────────────────────────────────
    auto *setupMenu = menuBar()->addMenu("&Setup");
    auto *serversAct = setupMenu->addAction("&Servers...");
    connect(serversAct, &QAction::triggered, this, &MainWindow::openSetupDialog);

    // ── Central placeholder ───────────────────────────────────────────────────
    m_statusLabel = new QLabel("GHEControl \u2014 no servers connected", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #606060; font-size: 13px;");
    setCentralWidget(m_statusLabel);

    // ── Load config and auto-connect ──────────────────────────────────────────
    AppConfig::instance().load();
    autoConnectFromConfig();
}

MainWindow::~MainWindow() = default;

// ── Private helpers ────────────────────────────────────────────────────────────

void MainWindow::autoConnectFromConfig()
{
    for (const ServerConfig &cfg : AppConfig::instance().servers()) {
        if (cfg.autoConnect)
            doConnect(cfg);
    }
}

void MainWindow::doConnect(const ServerConfig &cfg)
{
    if (m_controllers.contains(cfg.id))
        return; // already have a controller for this server

    auto *ctrl = new SwitchController(cfg, this);

    connect(ctrl, &SwitchController::connected, this, [this, id = cfg.id]() {
        AppConfig::instance().setAutoConnect(id, true);
        if (m_setupDialog)
            m_setupDialog->setRowState(id, true);
        updateStatusLabel();
    });

    connect(ctrl, &SwitchController::disconnected, this, [this, id = cfg.id]() {
        if (m_setupDialog)
            m_setupDialog->setRowState(id, false);
        updateStatusLabel();
    });

    connect(ctrl, &SwitchController::connectionError, this, [this, id = cfg.id](const QString &msg) {
        Q_UNUSED(msg)
        if (m_setupDialog)
            m_setupDialog->setRowState(id, false);
        updateStatusLabel();
    });

    m_controllers.insert(cfg.id, ctrl);
    ctrl->connectToServer();
}

void MainWindow::doDisconnect(const QString &serverId)
{
    if (auto *ctrl = m_controllers.take(serverId)) {
        AppConfig::instance().setAutoConnect(serverId, false);
        ctrl->disconnectFromServer();
        ctrl->deleteLater();
    }
    updateStatusLabel();
}

void MainWindow::updateStatusLabel()
{
    if (!m_statusLabel) return;

    const int n = m_controllers.size();
    if (n == 0) {
        m_statusLabel->setText("GHEControl \u2014 no servers connected");
        m_statusLabel->setStyleSheet("color: #606060; font-size: 13px;");
    } else {
        m_statusLabel->setText(
            QString("GHEControl \u2014 %1 server%2 connected")
                .arg(n).arg(n == 1 ? "" : "s"));
        m_statusLabel->setStyleSheet("color: #00a855; font-size: 13px;");
    }
}

// ── Private slots ──────────────────────────────────────────────────────────────

void MainWindow::openSetupDialog()
{
    if (!m_setupDialog) {
        m_setupDialog = new SetupDialog(this);

        connect(m_setupDialog, &SetupDialog::connectRequested,
                this,          &MainWindow::onConnectRequested);
        connect(m_setupDialog, &SetupDialog::disconnectRequested,
                this,          &MainWindow::onDisconnectRequested);
        connect(m_setupDialog, &SetupDialog::serversChanged,
                this,          &MainWindow::onServersChanged);
    }

    // Populate / refresh the table.
    m_setupDialog->setServers(AppConfig::instance().servers());

    // Reflect current connection states.
    for (auto it = m_controllers.cbegin(); it != m_controllers.cend(); ++it) {
        if (it.value()->isConnected())
            m_setupDialog->setRowState(it.key(), true);
    }

    m_setupDialog->show();
    m_setupDialog->raise();
    m_setupDialog->activateWindow();
}

void MainWindow::onConnectRequested(const ServerConfig &cfg)
{
    doConnect(cfg);
}

void MainWindow::onDisconnectRequested(const QString &serverId)
{
    doDisconnect(serverId);
}

void MainWindow::onServersChanged(const QList<ServerConfig> &servers)
{
    // Preserve autoConnect flags that are driven by live connection state,
    // not by the editor (the editor's rowToConfig uses rw.connected).
    AppConfig::instance().setServers(servers);
    AppConfig::instance().save();
}

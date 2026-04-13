#include "SetupDialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

// ── Column indices ─────────────────────────────────────────────────────────────
static constexpr int COL_HOST    = 0;
static constexpr int COL_PORT    = 1;
static constexpr int COL_USER    = 2;
static constexpr int COL_PASS    = 3;
static constexpr int COL_BUTTON  = 4;
static constexpr int COL_COUNT   = 5;

// ── Helpers ────────────────────────────────────────────────────────────────────
static QLineEdit *makeEdit(QWidget *parent, const QString &placeholder = {})
{
    auto *e = new QLineEdit(parent);
    e->setPlaceholderText(placeholder);
    e->setFrame(false);
    e->setStyleSheet("background: transparent; color: #c0c0c0;");
    return e;
}

// ── SetupDialog ────────────────────────────────────────────────────────────────
SetupDialog::SetupDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Servers");
    setMinimumSize(740, 360);

    auto *root = new QVBoxLayout(this);
    root->setSpacing(8);
    root->setContentsMargins(12, 12, 12, 12);

    // ── Top bar ──────────────────────────────────────────────────────────────
    auto *topBar = new QHBoxLayout;
    m_showPasswords = new QCheckBox("Show passwords", this);
    m_showPasswords->setStyleSheet("color: #909090;");
    connect(m_showPasswords, &QCheckBox::toggled, this, [this](bool show) {
        for (int r = 0; r < m_table->rowCount(); ++r) {
            if (auto *e = qobject_cast<QLineEdit *>(m_table->cellWidget(r, COL_PASS)))
                e->setEchoMode(show ? QLineEdit::Normal : QLineEdit::Password);
        }
    });
    topBar->addWidget(m_showPasswords);
    topBar->addStretch();
    root->addLayout(topBar);

    // ── Table ─────────────────────────────────────────────────────────────────
    m_table = new QTableWidget(0, COL_COUNT, this);
    m_table->setHorizontalHeaderLabels(
        { "IP Address / Name", "Port", "Username", "Password", "" });
    m_table->horizontalHeader()->setSectionResizeMode(COL_HOST,   QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(COL_PORT,   QHeaderView::Fixed);
    m_table->horizontalHeader()->setSectionResizeMode(COL_USER,   QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(COL_PASS,   QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(COL_BUTTON, QHeaderView::Fixed);
    m_table->setColumnWidth(COL_PORT,   70);
    m_table->setColumnWidth(COL_BUTTON, 130);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(true);
    m_table->setStyleSheet(R"(
        QTableWidget {
            background-color: #252525;
            alternate-background-color: #2c2c2c;
            gridline-color: #3a3a3a;
        }
        QHeaderView::section {
            background-color: #2a2a2a;
            color: #909090;
            border: none;
            border-bottom: 1px solid #3a3a3a;
            padding: 4px;
        }
    )");
    root->addWidget(m_table);

    // ── Bottom button bar ─────────────────────────────────────────────────────
    auto *bottomBar = new QHBoxLayout;

    auto *addBtn = new QPushButton("Add Server", this);
    connect(addBtn, &QPushButton::clicked, this, [this]() {
        ServerConfig cfg = ServerConfig::create();
        cfg.port = 10072;
        addRow(cfg);
        emitServersChanged();
    });

    auto *removeBtn = new QPushButton("Remove Selected", this);
    connect(removeBtn, &QPushButton::clicked, this, &SetupDialog::removeSelectedRow);

    auto *closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    bottomBar->addWidget(addBtn);
    bottomBar->addWidget(removeBtn);
    bottomBar->addStretch();
    bottomBar->addWidget(closeBtn);
    root->addLayout(bottomBar);
}

SetupDialog::~SetupDialog() = default;

// ── Public API ─────────────────────────────────────────────────────────────────

void SetupDialog::setServers(const QList<ServerConfig> &servers)
{
    m_suppressChanges = true;
    m_table->setRowCount(0);
    m_rows.clear();

    for (const ServerConfig &cfg : servers)
        insertRow(m_table->rowCount(), cfg, false);

    m_suppressChanges = false;
}

void SetupDialog::setRowState(const QString &serverId, bool connected)
{
    const int r = rowForId(serverId);
    if (r < 0) return;
    m_rows[r].connected = connected;
    updateConnectButton(r, connected);
}

// ── Private slots ──────────────────────────────────────────────────────────────

void SetupDialog::addRow(const ServerConfig &cfg)
{
    insertRow(m_table->rowCount(), cfg, false);
}

void SetupDialog::removeSelectedRow()
{
    const int r = m_table->currentRow();
    if (r < 0 || r >= m_rows.size()) return;

    const QString id = m_rows[r].serverId;
    if (m_rows[r].connected)
        emit disconnectRequested(id);

    m_table->removeRow(r);
    m_rows.removeAt(r);
    emitServersChanged();
}

void SetupDialog::onConnectButtonClicked()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) return;

    const QString id = btn->property("serverId").toString();
    const int r = rowForId(id);
    if (r < 0) return;

    if (m_rows[r].connected) {
        emit disconnectRequested(id);
    } else {
        // Save the row first so the caller gets up-to-date credentials.
        emitServersChanged();
        emit connectRequested(rowToConfig(r));
    }
}

void SetupDialog::onCellChanged()
{
    if (!m_suppressChanges)
        emitServersChanged();
}

// ── Private helpers ────────────────────────────────────────────────────────────

void SetupDialog::insertRow(int row, const ServerConfig &cfg, bool connected)
{
    const bool isNew = cfg.id.isEmpty();
    ServerConfig effectiveCfg = isNew ? ServerConfig::create() : cfg;
    if (isNew) effectiveCfg.port = 10072;

    m_table->insertRow(row);

    RowWidgets rw;
    rw.serverId  = effectiveCfg.id;
    rw.connected = connected;

    auto onChange = [this]() { onCellChanged(); };

    rw.hostEdit = makeEdit(this, "hostname or IP");
    rw.hostEdit->setText(effectiveCfg.host);
    connect(rw.hostEdit, &QLineEdit::textChanged, this, onChange);

    rw.portEdit = makeEdit(this, "10072");
    rw.portEdit->setText(QString::number(effectiveCfg.port));
    rw.portEdit->setMaximumWidth(70);
    connect(rw.portEdit, &QLineEdit::textChanged, this, onChange);

    rw.usernameEdit = makeEdit(this, "username");
    rw.usernameEdit->setText(effectiveCfg.username);
    connect(rw.usernameEdit, &QLineEdit::textChanged, this, onChange);

    rw.passwordEdit = makeEdit(this, "password");
    rw.passwordEdit->setText(effectiveCfg.password);
    rw.passwordEdit->setEchoMode(
        m_showPasswords && m_showPasswords->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
    connect(rw.passwordEdit, &QLineEdit::textChanged, this, onChange);

    rw.connectButton = new QPushButton(this);
    rw.connectButton->setProperty("serverId", rw.serverId);
    connect(rw.connectButton, &QPushButton::clicked, this, &SetupDialog::onConnectButtonClicked);

    m_table->setCellWidget(row, COL_HOST,   rw.hostEdit);
    m_table->setCellWidget(row, COL_PORT,   rw.portEdit);
    m_table->setCellWidget(row, COL_USER,   rw.usernameEdit);
    m_table->setCellWidget(row, COL_PASS,   rw.passwordEdit);
    m_table->setCellWidget(row, COL_BUTTON, rw.connectButton);

    m_table->setRowHeight(row, 28);

    m_rows.insert(row, rw);
    updateConnectButton(row, connected);
}

ServerConfig SetupDialog::rowToConfig(int row) const
{
    const RowWidgets &rw = m_rows.at(row);
    ServerConfig cfg;
    cfg.id       = rw.serverId;
    cfg.host     = rw.hostEdit->text().trimmed();
    cfg.port     = static_cast<quint16>(rw.portEdit->text().toUShort());
    if (cfg.port == 0) cfg.port = 10072;
    cfg.username = rw.usernameEdit->text();
    cfg.password = rw.passwordEdit->text();
    cfg.autoConnect = rw.connected;
    return cfg;
}

void SetupDialog::updateConnectButton(int row, bool connected)
{
    QPushButton *btn = m_rows[row].connectButton;
    if (connected) {
        btn->setText("\u25cf  Connected");
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #1a6b3c;
                border: 1px solid #00a855;
                border-radius: 3px;
                padding: 3px 8px;
                color: #c0c0c0;
            }
            QPushButton:hover { background-color: #1f7d47; }
        )");
    } else {
        btn->setText("\u25cb  Disconnected");
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #3a3a3a;
                border: 1px solid #555;
                border-radius: 3px;
                padding: 3px 8px;
                color: #808080;
            }
            QPushButton:hover { background-color: #454545; }
        )");
    }
}

void SetupDialog::emitServersChanged()
{
    QList<ServerConfig> list;
    list.reserve(m_rows.size());
    for (int r = 0; r < m_rows.size(); ++r)
        list.append(rowToConfig(r));
    emit serversChanged(list);
}

int SetupDialog::rowForId(const QString &id) const
{
    for (int r = 0; r < m_rows.size(); ++r)
        if (m_rows[r].serverId == id)
            return r;
    return -1;
}

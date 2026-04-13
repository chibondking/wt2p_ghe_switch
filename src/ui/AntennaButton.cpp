#include "AntennaButton.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>

AntennaButton::AntennaButton(int portIndex, const QString &label, QWidget *parent)
    : QWidget(parent)
    , m_portIndex(portIndex)
    , m_label(label)
{
    setMinimumHeight(32);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // TODO: set ForbiddenCursor when state == LockedOut.
}

AntennaButton::~AntennaButton() = default;

void AntennaButton::setState(AntennaState state)
{
    if (m_state == state)
        return;
    m_state = state;
    setCursor(state == AntennaState::LockedOut ? Qt::ForbiddenCursor : Qt::ArrowCursor);
    update();
}

void AntennaButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    // TODO: full paintEvent implementation — draw background, border, and
    // label text according to m_state colours defined in AntennaState enum.
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QColor bg, border;
    switch (m_state) {
    case AntennaState::Available:
        bg     = QColor("#3a3a3a");
        border = QColor("#555555");
        break;
    case AntennaState::SelectedSelf:
        bg     = QColor("#1a6b3c");
        border = QColor("#00a855");
        break;
    case AntennaState::LockedOut:
        bg     = QColor("#4a1515");
        border = QColor("#7a2020");
        break;
    }

    QRect r = rect().adjusted(1, 1, -1, -1);
    p.setBrush(bg);
    p.setPen(QPen(border, 1));
    p.drawRoundedRect(r, 3, 3);

    p.setPen(QColor("#c0c0c0"));
    p.drawText(r, Qt::AlignCenter, m_label);
}

void AntennaButton::mousePressEvent(QMouseEvent *event)
{
    if (m_state == AntennaState::LockedOut) {
        event->ignore();
        return;
    }
    if (event->button() == Qt::LeftButton)
        emit portClicked(m_portIndex);
}

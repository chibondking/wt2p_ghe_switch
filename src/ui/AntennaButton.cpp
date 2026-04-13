#include "AntennaButton.h"

AntennaButton::AntennaButton(int portIndex, const QString &label, QWidget *parent)
    : QPushButton(label, parent)
    , m_portIndex(portIndex)
{
    // TODO: connect QPushButton::clicked to emit portClicked(m_portIndex).
}

AntennaButton::~AntennaButton() = default;

void AntennaButton::setActive(bool active)
{
    Q_UNUSED(active)
    // TODO: toggle a highlighted stylesheet property to show active port.
}

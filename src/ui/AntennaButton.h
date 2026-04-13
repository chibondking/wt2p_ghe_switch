#pragma once

#include <QPushButton>

// TODO: Custom QPushButton representing a single antenna port. Shows the port
// label and highlights when that port is active. Emits a clicked(int portIndex)
// signal so SwitchController can respond to user selection.
class AntennaButton : public QPushButton
{
    Q_OBJECT

public:
    explicit AntennaButton(int portIndex, const QString &label, QWidget *parent = nullptr);
    ~AntennaButton() override;

    void setActive(bool active);

signals:
    void portClicked(int portIndex);

private:
    int m_portIndex;
};

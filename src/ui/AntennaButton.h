#pragma once

#include <QWidget>
#include <QString>

enum class AntennaState {
    Available,    // #3a3a3a bg, #555 border
    SelectedSelf, // #1a6b3c bg, #00a855 border (green)
    LockedOut     // #4a1515 bg, #7a2020 border, ForbiddenCursor, non-clickable
};

// Custom QWidget representing a single antenna port. Visual state is driven by
// paintEvent using AntennaState. Emits portClicked(int) on user interaction
// unless state is LockedOut.
class AntennaButton : public QWidget
{
    Q_OBJECT

public:
    explicit AntennaButton(int portIndex, const QString &label, QWidget *parent = nullptr);
    ~AntennaButton() override;

    void setState(AntennaState state);
    AntennaState state() const { return m_state; }

signals:
    void portClicked(int portIndex);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    int          m_portIndex;
    QString      m_label;
    AntennaState m_state{AntennaState::Available};
};

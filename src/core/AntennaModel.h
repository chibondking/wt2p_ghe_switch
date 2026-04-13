#pragma once

#include <QAbstractListModel>
#include <QString>
#include <vector>

// TODO: Represent the set of antenna ports reported by the switch hardware.
// Each port has an index, a user-editable label, and an active/inactive state.
// Expose the data via QAbstractListModel so views can bind directly.
class AntennaModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AntennaModel(QObject *parent = nullptr);
    ~AntennaModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

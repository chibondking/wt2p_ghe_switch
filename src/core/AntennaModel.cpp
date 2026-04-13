#include "AntennaModel.h"

AntennaModel::AntennaModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // TODO: populate initial port list; connect to GheClient signals to
    // update state when the hardware reports switch position changes.
}

AntennaModel::~AntennaModel() = default;

int AntennaModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 0; // TODO: return actual port count
}

QVariant AntennaModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    // TODO: return port label, active state, etc. by role.
    return {};
}

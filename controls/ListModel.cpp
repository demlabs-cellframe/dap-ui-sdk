#include "ListModel.h"


ListModel::ListModel(QObject *parent):QAbstractListModel(parent)
{

    DataModel tmpModel;
    for(int i = 0;i<6;i++)
    {
        tmpModel.text = "kelvin-testnet.Cellframe";
        tmpModel.iconPath ="img1";
        m_data.append(tmpModel);
    }

}
ListModel::ListModel(QList<DataModel> *list_model,QObject *parent)
{
    Q_UNUSED(parent)

    for(int i = 0;i<list_model->size();i++)
        m_data.append(list_model->at(i));
}


int ListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_data.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

        return QVariant();
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

    return roles;
}

QList<DataModel> ListModel::getData()
{
    return m_data;
}



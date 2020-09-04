#include "LocaleModel.h"

LocaleModel::LocaleModel(QObject *a_parrent)
    :QAbstractListModel(a_parrent)
{

}

void LocaleModel::appendLocale(const QLocale &a_locale)
{
    this->beginInsertRows(QModelIndex(), m_locales.size(), m_locales.size());
    m_locales.append(a_locale);
    this->endInsertRows();
}

int LocaleModel::rowCount(const QModelIndex &a_parent) const
{
    Q_UNUSED(a_parent)
    return m_locales.size();
}

QVariant LocaleModel::data(const QModelIndex &a_index, int a_role) const
{
    if (!a_index.isValid() || a_role != Qt::DisplayRole)
        return {};

    int index = a_index.row();

    if (index >= m_locales.size() || index < 0)
        return {};

    return m_locales.value(index).nativeLanguageName();
}

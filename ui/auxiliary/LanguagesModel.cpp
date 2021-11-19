#include "LanguagesModel.h"
#include "Utilz.h"

LanguagesModel::LanguagesModel(QObject *a_parrent)
    :QAbstractListModel(a_parrent)
{

}

void LanguagesModel::appendLanguage(QLocale::Language a_locale)
{
    this->beginInsertRows(QModelIndex(), m_languages.size(), m_languages.size());
    m_languages.append(a_locale);
    this->endInsertRows();
}

void LanguagesModel::appendLanguages(const QList<QLocale::Language> &a_languageList)
{
    this->beginInsertRows(QModelIndex(), m_languages.size(), m_languages.size() + a_languageList.count() - 1);
    for (QLocale::Language language: a_languageList)
        m_languages.append(language);
    this->endInsertRows();
}

QLocale::Language LanguagesModel::language(int a_index) const
{
    return m_languages[a_index];
}

bool LanguagesModel::isExist(QLocale::Language a_language) const
{
    if (qFind(m_languages.begin(),m_languages.end(), a_language) == m_languages.end()) return false;
    else return true;
}

int LanguagesModel::rowCount(const QModelIndex &a_parent) const
{
    Q_UNUSED(a_parent)
    return m_languages.size();
}

QVariant LanguagesModel::data(const QModelIndex &a_index, int a_role) const
{
    if (!a_index.isValid() || a_role != Qt::DisplayRole)
        return {};

    int index = a_index.row();

    if (index >= m_languages.size() || index < 0)
        return {};

    return Utils::toNativeLanguageName(this->language(index));
}

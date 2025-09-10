/* INCLUDES */
#include "LanguagesModel.h"
#include "Utilz.h"
#include <algorithm>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

LanguagesModel::LanguagesModel (QObject *a_parrent)
  : QAbstractListModel (a_parrent)
{

}

/********************************************
 * METHODS
 *******************************************/

void LanguagesModel::appendLanguage (QLocale::Language a_locale)
{
  beginInsertRows (QModelIndex(), m_languages.size(), m_languages.size());
  m_languages.append (a_locale);
  endInsertRows();
}

void LanguagesModel::appendLanguages (const QList<QLocale::Language> &a_languageList)
{
  beginInsertRows (QModelIndex(), m_languages.size(), m_languages.size() + a_languageList.count() - 1);

  for (QLocale::Language language : a_languageList)
    m_languages.append (language);

  endInsertRows();
}

QLocale::Language LanguagesModel::language (int a_index) const
{
  if (a_index < 0 || a_index >= m_languages.size())
    return QLocale::Language();

  return m_languages[a_index];
}

int LanguagesModel::indexOf (QLocale::Language a_language) const
{
  return m_languages.indexOf (a_language);
}

bool LanguagesModel::isExist (QLocale::Language a_language) const
{
  if (std::find (m_languages.begin(), m_languages.end(), a_language) == m_languages.end())
    return false;
  else
    return true;
}

/********************************************
 * OVERRIDE
 *******************************************/

int LanguagesModel::rowCount (const QModelIndex &a_parent) const
{
  Q_UNUSED (a_parent)
  return m_languages.size();
}

QVariant LanguagesModel::data (const QModelIndex &a_index, int a_role) const
{
  if (!a_index.isValid() || a_role != Qt::DisplayRole)
    return {};

  int index = a_index.row();

  if (index >= m_languages.size() || index < 0)
    return {};

  return Utils::toNativeLanguageName (this->language (index));
}

/*-----------------------------------------*/

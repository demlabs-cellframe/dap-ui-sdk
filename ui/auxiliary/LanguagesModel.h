#ifndef LANGUAGESMODEL_H
#define LANGUAGESMODEL_H

/* INCLUDES */
#include <QAbstractItemModel>
#include <QLocale>
#include <QtAlgorithms>

/****************************************//**
 * @brief language model
 * @date 05.07.2022
 * @author Mikhail Shilenko
 *******************************************/

class LanguagesModel: public QAbstractListModel
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QVector<QLocale::Language> m_languages;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  LanguagesModel (QObject *a_parrent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void appendLanguage (QLocale::Language a_locale);
  void appendLanguages (const QList<QLocale::Language> &a_languageList);

  QLocale::Language language (int a_index) const;
  int indexOf (QLocale::Language a_language) const;
  bool isExist (QLocale::Language a_language) const;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  virtual int rowCount (const QModelIndex &parent) const override;
  virtual QVariant data (const QModelIndex &index, int role) const override;
  /// @}
};

/*-----------------------------------------*/
#endif // LANGUAGESMODEL_H

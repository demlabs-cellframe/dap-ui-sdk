#ifndef LANGUAGESMODEL_H
#define LANGUAGESMODEL_H

#include <QAbstractItemModel>
#include <QLocale>

class LanguagesModel: public QAbstractListModel
{
public:
    LanguagesModel(QObject *a_parrent = nullptr);

    void appendLanguage(QLocale::Language a_locale);
    void appendLanguages(const QList<QLocale::Language>& a_languageList);
    QLocale::Language language(int a_index) const;
    bool isExist(QLocale::Language a_language) const;

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
private:
    QVector<QLocale::Language> m_languages;
};

#endif // LANGUAGESMODEL_H

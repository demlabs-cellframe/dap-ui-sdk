#ifndef LOCALEMODEL_H
#define LOCALEMODEL_H

#include <QAbstractItemModel>
#include <QLocale>

class LocaleModel: public QAbstractListModel
{
public:
    LocaleModel(QObject *a_parrent = nullptr);

    void appendLocale(const QLocale& a_locale);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
private:
    QVector<QLocale> m_locales;
};

#endif // LOCALEMODEL_H

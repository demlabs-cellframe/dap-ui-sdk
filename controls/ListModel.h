#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QString>
#include <QStringList>
//class ListModel
//{
//public:
//    ListModel();
//};

struct DataModel
{
    QString iconPath;
    QString text;
};


class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    ListModel(QObject *parent = nullptr);
    ListModel(QList<DataModel> *list_model,QObject *parent = nullptr);


    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    QList<DataModel> getData();

private:
    QList<DataModel> m_data;
};



#endif // LISTMODEL_H

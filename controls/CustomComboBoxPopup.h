#ifndef CUSTOMCOMBOBOXPOPUP_H
#define CUSTOCOMBOBOXPOPUP_H

#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QScrollArea>
#include "CustomPlacementButton_New.h"


struct DataModel
{
    QString iconPath;
    QString text;
};

class CustomComboBoxPopup: public QWidget
{
    Q_OBJECT
public:
    CustomComboBoxPopup( QWidget *parent = nullptr);
    /// Set model.
    /// @param model Model date for button.
    void setModel(QList<DataModel> *model);

    void setModel(QAbstractItemModel *a_model);
    /// Set caption.
    /// @param text Name.
    void setCaption(const QString &text);

signals:
    void itemSelected(int index);

private:

    QVBoxLayout *m_layout;
    QVBoxLayout *m_layoutScroll;
    QLabel *m_lblCaption;
    QScrollArea *m_scaList;
    QList<CustomPlacementButton_New*> m_listButton;
};

#endif // CUSTOMCOMBOBOXPOPUP_H

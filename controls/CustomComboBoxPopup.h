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
public:
    CustomComboBoxPopup( QWidget *parent = nullptr);
    /// Set model.
    /// @param model Model date for button.
    void setModel(QList<DataModel> *model);
    /// Set capture name.
    /// @param text Name.
    void setTextCapture(const QString &text);
private:


    QVBoxLayout *m_layout;
    QVBoxLayout *m_layoutScroll;
    QLabel *m_lblCaption;
    QScrollArea *m_scaList;
    QList<CustomPlacementButton_New*> *m_listButton;
};

#endif // CUSTOMCOMBOBOXPOPUP_H

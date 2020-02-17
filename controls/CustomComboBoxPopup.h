#ifndef CUSTOMCOMBOBOXPOPUP_H
#define CUSTOCOMBOBOXPOPUP_H

#include <QMdiArea>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include "ListModel.h"
#include "CustomWidget.h"

class CustomComboBoxPopup: public QWidget
{
public:
    CustomComboBoxPopup(const QString &a_Caption,ListModel *model = nullptr, QWidget *parent = nullptr);
private:
    QVBoxLayout *m_layout;
    QLabel *m_lblCaption;
    QListView *m_lvwList;
    ListModel *m_model;
};

#endif // CUSTOMCOMBOBOXPOPUP_H

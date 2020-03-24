#ifndef CUSTOMCOMBOBOXPOPUP_H
#define CUSTOCOMBOBOXPOPUP_H

#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QScrollArea>
#include "CustomComboBox.h"
#include "AdaptiveWidget.h"

class CustomComboBoxPopup: public AdaptiveWidget
{
    Q_OBJECT
public:
    CustomComboBoxPopup(QWidget *parent = nullptr);

    virtual void setModel(QAbstractItemModel *a_model);

public slots:
    void setCurrentIndex(int a_index);

protected:
    virtual void initVariantUi(QWidget * a_widget) override;
    virtual QString listViewName() = 0;
//    virtual void showEvent(QShowEvent *event) override;

signals:
    void itemSelected(int index);

private:
    QList<QListView*> allListViews();
};

#endif // CUSTOMCOMBOBOXPOPUP_H

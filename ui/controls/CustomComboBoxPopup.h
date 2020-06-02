#ifndef CUSTOMCOMBOBOXPOPUP_H
#define CUSTOMCOMBOBOXPOPUP_H

#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QScrollArea>
#include "CustomPopupComboBox.h"
#include "CustomPopup.h"

#include <QListView>
#include <QDebug>
#include "CustomPopupComboBox.h"
#include "WidgetDelegateListView.h"

class CustomComboBoxPopup: public CustomPopup
{
    Q_OBJECT
public:
    CustomComboBoxPopup(QMainWindow *parent = nullptr);

    QAbstractItemModel* model();
    virtual void setModel(QAbstractItemModel *a_model);

    void addItem(const QString &a_text, const QVariant &a_userData = QVariant());

    virtual void setCaption(const QString& a_caption);
public slots:
    void setCurrentIndex(int a_index);

protected:
    virtual void initVariantUi(QWidget * a_widget) override;
    virtual QString listViewName() = 0;

    virtual QString captionLabelName();

signals:
    void activated(int index);
    void activated(const QString &text);

private:
    QList<QListView*> allListViews();

    QAbstractItemModel* m_model{};
};

#endif // CUSTOMCOMBOBOXPOPUP_H

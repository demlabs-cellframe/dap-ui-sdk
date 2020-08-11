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

#include <QDebug>
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
    QString caption() const;

public slots:
    void setCurrentIndex(int a_index);

protected:
    virtual void initVariantUi(QWidget * a_widget) override;
    virtual QString listViewName() = 0;             // Needed to be overrided!
    QList<QListView*> allListViews();

    virtual QString captionLabelName();

signals:
    void activated(int index);
    void activated(const QString &text);

private:

    QAbstractItemModel* m_model{};
    QString m_caption;
};

#endif // CUSTOMCOMBOBOXPOPUP_H

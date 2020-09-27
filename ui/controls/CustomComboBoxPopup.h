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
    Q_PROPERTY(bool captionVisible READ captionIsVisible WRITE setCaptionVisible)
    // @brief When the ignoreIconsFromModel property is true, this list view ignores any images posted by the model.
    Q_PROPERTY(bool ignoreIconsFromModel READ isIcomsFromModelIgnored WRITE ignoreIconsFromModel DESIGNABLE true)

public:
    CustomComboBoxPopup(QMainWindow *parent = nullptr);

    QAbstractItemModel* model();
    virtual void setModel(QAbstractItemModel *a_model);

    void addItem(const QString &a_text, const QVariant &a_userData = QVariant());

    virtual void setCaption(const QString& a_caption);
    QString caption() const;

    void setCaptionVisible(bool a_visible);
    bool captionIsVisible() const;
    QList<QListView*> allListViews();

    void ignoreIconsFromModel(bool y) {
        m_ignoreIconsFromModel = y;
    }
    bool isIcomsFromModelIgnored() {
        return m_ignoreIconsFromModel;
    }

public slots:
    void setCurrentIndex(int a_index);

protected:
    virtual void initVariantUi(QWidget * a_widget) override;
    virtual QString listViewName() = 0;             // Needed to be overrided!

    virtual QString captionLabelName();

signals:
    void activated(int index);
    void activated(const QString &text);

private:

    QAbstractItemModel* m_model{};
    QString m_caption;

    bool m_captionIsVisible {true};
    bool m_ignoreIconsFromModel = true;
};

#endif // CUSTOMCOMBOBOXPOPUP_H

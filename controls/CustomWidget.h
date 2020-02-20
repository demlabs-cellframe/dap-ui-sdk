#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "StyledDropShadowEffect.h"
#include "ListModel.h"
#include <QListView>
class CustomWidget: public QWidget
{
public:
    CustomWidget(QWidget *parent = nullptr);
    CustomWidget(QString iconPath = "", QString text = "", QWidget *parent = nullptr);

    bool selected() const;
    void setSelected(bool selected);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void hideEvent(QHideEvent *event);

private:
    //Updates styles on mouseover and on setting activity of an element.
    void setStyle(int state);

    bool m_selected;





    /////////////////////////////////////////
    QLabel *m_lblFlag;
    QLabel *m_lblText;
    QLabel *m_lblCheckIcon;
    QHBoxLayout *m_mainLayout;
    StyledDropShadowEffect *m_styledshadow;
    DataModel *m_data;
    int m_index;
    QListView *m_listView;

};

#endif // CUSTOMWIDGET_H

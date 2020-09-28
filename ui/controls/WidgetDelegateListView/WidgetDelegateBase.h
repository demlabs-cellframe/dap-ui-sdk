#ifndef WIDGETDELEGATEBASE_H
#define WIDGETDELEGATEBASE_H

#include <QFrame>
#include <QBoxLayout>

#include <QDebug>
class WidgetDelegateBase : public QFrame
{
    Q_OBJECT
public:
    explicit WidgetDelegateBase(QWidget *parent = nullptr);

public slots:
    virtual void setData(const QMap<int, QVariant>& a_dataMap);
    virtual void setData(const QVariant &value, int role = Qt::DisplayRole) = 0;

    virtual void setSelected(bool a_selected);

signals:
    void sizeChanged(const QSize& a_size);
    void selected();

protected:
    virtual void resizeEvent(QResizeEvent *event);

    QBoxLayout *m_layout;
};

#endif // ABSTRACTWIDGETDELEGATE_H

#ifndef WIDGETDELEGATEBASE_H
#define WIDGETDELEGATEBASE_H

#include <QFrame>
#include <QBoxLayout>

class WidgetDelegateBase : public QFrame
{
    Q_OBJECT
    // @brief When the ignoreIconsFromModel property is true, this list view ignores any images posted by the model.
    Q_PROPERTY(bool ignoreIconsFromModel READ isImagesIgnored WRITE ignoreImages)
public:
    explicit WidgetDelegateBase(QWidget *parent = nullptr);

    void ignoreImages(bool y);
    bool isImagesIgnored() {
        return m_ignoreImages;
    }

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

    bool m_ignoreImages = true;
};

#endif // ABSTRACTWIDGETDELEGATE_H

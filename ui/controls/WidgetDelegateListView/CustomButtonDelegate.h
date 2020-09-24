#ifndef CUSTOMBUTTONDELEGATE_H
#define CUSTOMBUTTONDELEGATE_H

#include "WidgetDelegateBase.h"
#include "CustomPlacementButton.h"

class CustomButtonDelegate : public WidgetDelegateBase
{
    // @brief When the ignoreIconsFromModel property is true, this list view ignores any images posted by the model.
    Q_PROPERTY(bool ignoreIconsFromModel WRITE ignoreImages READ isImagesIgnored DESIGNABLE true)
public:
    CustomButtonDelegate(QWidget* a_parent = Q_NULLPTR);

    virtual void setData(const QVariant &value, int role = Qt::DisplayRole) override;

    CustomPlacementButton* button();

    virtual void setSelected(bool a_selected) override;

    static WidgetDelegateBase* create();

    void ignoreImages(bool y){
        m_ignoreImages = y;
    }
    bool isImagesIgnored() {
        return m_ignoreImages;
    }

private:
    CustomPlacementButton* m_button;
    bool m_ignoreImages = true;
};

#endif // CUSTOMBUTTONDELEGATE_H

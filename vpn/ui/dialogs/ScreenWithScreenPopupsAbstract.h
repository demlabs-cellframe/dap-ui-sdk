#ifndef SCREENWITHSCREENCOMBOBOXESABSTRACT_H
#define SCREENWITHSCREENCOMBOBOXESABSTRACT_H

#include "AdaptiveScreen.h"
#include "PopupScreenBase.h"
#include "ScreenWithCustomPopupsAbstract.h"

class ScreenWithScreenPopupsAbstract : public ScreenWithCustomPopupsAbstract
{
    Q_OBJECT
public:
    ScreenWithScreenPopupsAbstract(QWidget *a_parent = nullptr);
    static const QString SCREEN_NAME;

    virtual QString screenName() override = 0;

    virtual void initVariantUi(QWidget * a_widget) override;

    virtual void setPopupScreen(PopupScreenBase *a_screen);

signals:
    void popupClosingStarted();
    void screenComboBoxOpened();
    void showedScreenPopupCaptionChanged(const QString& a_caption);

    void popupHidden();

protected:
    virtual QList<CustomPopup*> customPopups() override = 0;
    virtual void connectPopup(CustomPopup *a_popup) override;
};



#endif // SCREENWITHSCREENCOMBOBOXESABSTRACT_H

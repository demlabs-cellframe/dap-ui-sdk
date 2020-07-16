#ifndef SCREENWITHSCREENCOMBOBOXESABSTRACT_H
#define SCREENWITHSCREENCOMBOBOXESABSTRACT_H

#include "AdaptiveScreen.h"
#include "ComboBoxPopupScreenBase.h"
#include "ScreenWithCustomComboBoxesAbstract.h"

class ScreenWithScreenComboBoxesAbstract : public ScreenWithCustomComboBoxesAbstract
{
    Q_OBJECT

public:
    ScreenWithScreenComboBoxesAbstract(QWidget *a_parent = nullptr);
    static const QString SCREEN_NAME;

    virtual QString screenName() override = 0;

    virtual void initVariantUi(QWidget * a_widget) override;

    virtual void setComboBoxPopupScreen(ComboBoxPopupScreenBase *a_screen);

public :
signals:
    void comboBoxPopupClosingStarted();

    void comboBoxPopupHidden();

protected:
    virtual QList<ComboBox*> customComboBoxes() override = 0;
    virtual void connectComboBox(ComboBox *a_comboBox) override;
};



#endif // SCREENWITHSCREENCOMBOBOXESABSTRACT_H

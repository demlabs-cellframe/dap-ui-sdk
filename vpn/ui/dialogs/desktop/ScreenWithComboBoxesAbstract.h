#ifndef SCREENWITHCOMBOBOXESABSTRACT_H
#define SCREENWITHCOMBOBOXESABSTRACT_H

#include "AdaptiveScreen.h"
#include "ComboBoxPopupScreenBase.h"

class ScreenWithComboBoxesAbstract : public AdaptiveScreen
{
    Q_OBJECT

public:
    ScreenWithComboBoxesAbstract(QWidget *a_parent = nullptr);
    static const QString SCREEN_NAME;

    virtual QString screenName() override= 0;
    virtual QStringList comboBoxesNames() const = 0;

    virtual void initVariantUi(QWidget * a_widget) override;

    virtual void setComboBoxPopupScreen(ComboBoxPopupScreenBase *a_screen);

public :
signals:
    void comboBoxPopupOpened();
    void hideComboBoxPopup();

    void comboBoxPopupHidden();

protected:

};

#endif // SCREENWITHCOMBOBOXESABSTRACT_H

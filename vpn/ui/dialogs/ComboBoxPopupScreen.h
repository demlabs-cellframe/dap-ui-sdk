#ifndef COMBOBOXPOPUPSCREEN_H
#define COMBOBOXPOPUPSCREEN_H

#include "PopupScreenBase.h"

class ComboBoxPopupScreen : public PopupScreenBase
{
    Q_OBJECT
public:
    ComboBoxPopupScreen(QWidget *a_parent = nullptr);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;
};



#endif // COMBOBOXPOPUPSCREEN_H

#ifndef COMBOBOXPOPUPSCREENBASE_H
#define COMBOBOXPOPUPSCREENBASE_H

#include "AdaptiveScreen.h"

class ComboBoxPopupScreenBase : public AdaptiveScreen
{
    Q_OBJECT
public:
    ComboBoxPopupScreenBase(QWidget *a_parent = nullptr);

    virtual QString screenName()  override = 0;
    virtual void initVariantUi(QWidget * a_widget) override;

signals:
    void opened();
    void hidden();
    void resized(const QSize &);


    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;


};



#endif // COMBOBOXPOPUPSCREENBASE_H

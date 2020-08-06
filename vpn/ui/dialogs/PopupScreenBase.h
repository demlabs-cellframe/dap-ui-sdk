#ifndef POPUPSCREENBASE_H
#define POPUPSCREENBASE_H

#include "AdaptiveScreen.h"

class PopupScreenBase : public AdaptiveScreen
{
    Q_OBJECT
public:
    PopupScreenBase(QWidget *a_parent = nullptr);

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

#ifndef COMBOBOXPOPUPSCREEN_H
#define COMBOBOXPOPUPSCREEN_H

#include "AdaptiveScreen.h"

class ComboBoxPopupScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    ComboBoxPopupScreen(QWidget *a_parent = nullptr);
    static const QString SCREEN_NAME;

    virtual QString screenName() override;
    virtual void initVariantUi(QWidget * a_widget) override;

signals:
    void opened();
    void hidden();
    void resized(const QSize &);


    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};



#endif // COMBOBOXPOPUPSCREEN_H

#ifndef INFORMATIONSCREEN_H
#define INFORMATIONSCREEN_H
#include "AdaptiveScreen.h"
#include "vpnDefine.h"
#include "ui_InformationScreen.h"
#include <QScroller>

class InformationScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    InformationScreen(QWidget *a_parent);
    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setText(const QString& a_text);
    void setTextCaption(const QString &a_text);
    void setVisibleCaption(bool a_visible);
protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    QScopedPointer<Ui::InformationScreen> m_ui;
};

#endif // INFORMATIONSCREEN_H

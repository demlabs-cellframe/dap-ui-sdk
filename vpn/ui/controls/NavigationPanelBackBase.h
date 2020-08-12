#ifndef NAVIGATIONPANELBACKBASE_H
#define NAVIGATIONPANELBACKBASE_H

#include "AdaptiveScreen.h"
#include "ui_NavigationPanelBack.h"


class NavigationPanelBackBase : public AdaptiveScreen
{
    Q_OBJECT
public:
    NavigationPanelBackBase(QWidget *parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

signals:
    void goBack();

protected:
    virtual void initVariantUi(QWidget *a_widget) override;

    QScopedPointer<Ui::NavigationPanelBack> m_ui;
};

#endif // NAVIGATIONPANELBACKBASE_H

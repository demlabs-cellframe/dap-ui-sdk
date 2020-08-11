#ifndef CONNECTIONSCREENBASE_H
#define CONNECTIONSCREENBASE_H

#include <QLabel>

#include "BrandAppProperties.h"

#include "StyledDropShadowEffect.h"

#include <QStateMachine>
#include <QListView>

#include "AdaptiveScreen.h"
#include "vpnDefine.h"

#include "defines.h"
#include <QStyle>
#include <QList>

#include "ui_AccountScreen.h"
#include "CustomPlacementButton.h"

class ConnectionScreenBase : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    ConnectionScreenBase(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionState a_state);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    QScopedPointer<CONNECTION_FORM> m_ui;
};

#endif // CONNECTIONSCREENBASE_H

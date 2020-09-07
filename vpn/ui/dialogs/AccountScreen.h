﻿#ifndef ACCOUNTSCREEN_H
#define ACCOUNTSCREEN_H

#include <QLabel>

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
#include "SerialRemovalConfirmationMessage.h"
#include "ScreenWithScreenPopupsAbstract.h"
#include <QDesktopServices>
#include <QUrl>
#include "TariffItem.h"

class AccountScreen : public ScreenWithScreenPopupsAbstract
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    AccountScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionState a_state);

#ifndef Q_OS_ANDROID
    enum class ActivationState {
        Activated,
        Unactivated
    };
    void setState(ActivationState a_activationState);

    void appendTariff(const QList<TariffItem> &a_tariffList);

signals:
    void serialRemovalRequested();
#endif

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    virtual QList<CustomPopup *> customPopups() override;

private:
    SerialRemovalConfirmationMessage *m_serialRemovalMessage = nullptr;

    QScopedPointer<Ui::AccountScreen> m_ui;
};

#endif // ACCOUNTSCREEN_H

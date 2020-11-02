#ifndef ACCOUNTSCREENBASE_H
#define ACCOUNTSCREENBASE_H

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
#include "TariffItem.h"

class AccountScreenBase : public ScreenWithScreenPopupsAbstract
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    AccountScreenBase(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionState a_state);


    enum class ActivationState {
        Activated,
        Unactivated
    };
    void setState(ActivationState a_activationState);

    void appendTariff(const QList<TariffItem> &a_tariffList);
public slots:
    void openTariffPopup();

signals:
    void serialRemovalRequested();

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    virtual QList<CustomPopup *> customPopups() override;

    SerialRemovalConfirmationMessage *m_serialRemovalMessage = nullptr;

    QScopedPointer<Ui::AccountScreen> m_ui;

};

#endif // ACCOUNTSCREENBASE_H

#ifndef FAQSCREEN_H
#define FAQSCREEN_H

#include <QStateMachine>
#include <QListView>
#include <QLabel>
#include "AdaptiveScreen.h"
#include "vpnDefine.h"

#include "ui_FAQScreen.h"
#include "defines.h"
#include "FAQWidget.h"
#include <QStyle>
#include <QList>

#include <QScrollArea>

class FAQScreen : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    FAQScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionStates a_state);
protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    const QString SCA_FAQ = "scaFAQ";
private:
    //Reference List
    QList<FAQDataModel> *m_dataModel;
    //main list widget
    QWidget *m_wgtScroll;

    QVBoxLayout *m_VericalLayout;
    //Widget container for reference
    QList<QWidget *> *m_listWgt;
};

#endif // FAQSCREEN_H

#ifndef SCREENWITHCUSTOMCOMBOBOXESABSTRACT_H
#define SCREENWITHCUSTOMCOMBOBOXESABSTRACT_H

#include "AdaptiveScreen.h"
#include "ComboBox.h"

class ScreenWithCustomPopupsAbstract : public AdaptiveScreen
{
    Q_OBJECT

public:
    ScreenWithCustomPopupsAbstract(QWidget *a_parent = nullptr);
    static const QString SCREEN_NAME;

    virtual QString screenName() override = 0;

public slots:
    virtual void closePopups();

protected:
    virtual QList<CustomPopup*> customPopups() = 0;

    virtual void initVariantUi(QWidget * a_widget) override;

    virtual void connectPopup(CustomPopup* a_popup);

signals:
    void popupOpened();
    void popupClosed();

protected:

};

#endif // SCREENWITHCUSTOMCOMBOBOXESABSTRACT_H

#ifndef SCREENWITHCUSTOMCOMBOBOXESABSTRACT_H
#define SCREENWITHCUSTOMCOMBOBOXESABSTRACT_H

#include "AdaptiveScreen.h"
#include "ComboBox.h"

class ScreenWithCustomComboBoxesAbstract : public AdaptiveScreen
{
    Q_OBJECT

public:
    ScreenWithCustomComboBoxesAbstract(QWidget *a_parent = nullptr);
    static const QString SCREEN_NAME;

    virtual QString screenName() override = 0;

public slots:
    virtual void closeComboBoxesPopups();

protected:
    virtual QList<ComboBox*> customComboBoxes() = 0;

    virtual void initVariantUi(QWidget * a_widget) override;

    virtual void connectComboBox(ComboBox* a_comboBox);

signals:
    void comboBoxPopupOpened();
    void comboBoxPopupClosed();

protected:

};

#endif // SCREENWITHCUSTOMCOMBOBOXESABSTRACT_H

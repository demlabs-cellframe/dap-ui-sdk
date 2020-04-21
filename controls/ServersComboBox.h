#ifndef SERVERSCOMBOBOX_H
#define SERVERSCOMBOBOX_H

#include <cmath>
#include <QComboBox>
#include <QPaintEvent>
#include <QStyleOptionComboBox>
#include <QStylePainter>
#include <QDebug>
#include "ComboBoxDelegate.h"
#include "AppStyleSheetHandler.h"

/** @brief QComboBox with changing downArrow when hover on control.
 *
 *  @details Chang downArrow when hover on QComboBox. Reduce font size if text is not fitting and cut it if is in necessary.
 *  @todo Is searching style in comments also!
*/
class ServersComboBox : public QComboBox
{
    Q_OBJECT
public:
    ServersComboBox(QWidget *parent = Q_NULLPTR);

    void updateStyleSheets();

    void setObjectName(const QString &name);

    void setArrowStyles(const QString& a_normal,
                        const QString& a_on,
                        const QString& a_hover);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void wheelEvent(QWheelEvent *e);

    void hidePopup();

    //***********************
    //daisable popup open. Remove this code for show btMultirouting.
    virtual void showPopup(){ /*Do nothing*/ }

    void paintEvent(QPaintEvent *event);

private:
    struct {
        QString normal; ///< in normal  state of ServersComboBox
        QString on; ///< when ServersComboBox popup is showed
        QString hover; ///< in hover state of ServersComboBox
    } m_downArrowStyleSheet;
};

#endif // SERVERSCOMBOBOX_H

#ifndef CUSTOMPLACEMENTBUTTON_NEW_H
#define CUSTOMPLACEMENTBUTTON_NEW_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include "StyledDropShadowEffect.h"
#include <QFontMetrics>
#include "CustomButtonAbstract.h"



/** @brief QPushButton with subControls "text" and "image"
 *

 * your button{
 * This method is sometimes used to align text and images, or to create an indent. It activates the widgets on the sides (leftSpacing, rightSpacing)
 *  qproperty-alignment: AlignRight;//AlignLeft, AlignCenter (left or right spacers)
 *
 * }
 *  @details Places image and text from left to right in QHBoxLayout.
 * Set style in .css file in format
 *> #buttonName #leftSpacing {
 *>     ...; //if max-width==0, left alinment
 * }
 *> #buttonName::text {
 *>     ...
 *> }
 *> #buttonName::text[hover="true"] {
 *>     ...
 *> }
 *> #buttonName::text[checked = "true"] {
 *>     ...
 *> }
 *> #buttonName::text:checked:hover {
 *>     ...
 *> }
 *> #buttonName::image {
 *>     ...
 *> }
 *> #buttonName::image[hover="true"]{
 *>     ...
 *> }
 *> #buttonName::image[checked = "true"]{
 *>     ...
 *> }
 *> #buttonName::image:checked:hover {
 *>     ...
 *> }
 *> #buttonName #rightSpacing {
 *>     ...//if max-width==0, right alignment
 *> }
 * @todo Is searching style in comments also!
*/
class CustomPlacementButton : public CustomButtonAbstract
{
    Q_OBJECT

    Q_PROPERTY(Qt::Alignment alignment WRITE setAlignment DESIGNABLE true)
    Q_PROPERTY(Qt::LayoutDirection layoutDirection WRITE setLayoutDirection READ layoutDirection DESIGNABLE true)
    Q_PROPERTY(bool additionalImage WRITE setAdditionalImage DESIGNABLE true)

    void initButton();
public:

    explicit CustomPlacementButton(QWidget *a_parent = Q_NULLPTR);

    QString text();
    /// Set text button.
    /// @param text Set text.
    virtual void setText(const QString &text);
    /// Form initialization.
    /// @param path Path to Image.
    void setIcon(const QString &path);
    /// Set object name..
    /// @param name Object name.
    void setObjectName(const QString &name);
    /// Sets the state of the button.
    /// @param isHover
    /// @param isChecked
    void setState(bool a_isHover, bool a_isChecked);
    void setProperty(const QString& a_property, const QVariant& a_value);
    /// .
    /// @param a_id Window GUI widget.
    void addSubcontrol(QString a_id);

    Qt::LayoutDirection layoutDirection() const;
    void setLayoutDirection(Qt::LayoutDirection a_direction);

    void setAdditionalImage(bool a_visible);

    void setGraphicsEffect(StyledDropShadowEffect *a_effect);
    ///Makes widgets visible on the sides
    /// @param a_spacer If there is ALIGNMENT_NONE or some erroneous value, the widgets will be invisible.
    void setAlignment(Qt::Alignment a_alignment);

    void updateAppearanceForFocus(bool);

    void emitTabFocus(bool isActiv);

signals:
    void tabFocusIn();
    void tabFocusOut();


protected:
    /// Cursor in.
    /// @param event Signal source.
    void enterEvent(QEvent *event);
    /// Cursor out.
    /// @param event Signal source.
    void leaveEvent(QEvent *event);

    QHBoxLayout *m_layout;
    QWidget m_wgtLeftSpacing;         ///<label for left spacing
    QList<QLabel*> m_subcontrols;
    QLabel m_lbImage;    ///<label with image
    QLabel m_lbText;     ///<label with text
    QLabel m_lbAdditionalImage;
    QWidget m_wgtRightSpacing;        ///<label for right spacing

private:
    ///For effect.
    StyledDropShadowEffect *m_styledShadow = nullptr;

    static void setWidgetState(QWidget* a_widget, bool a_isHover=false, bool a_isChecked = false);

};

#endif // CUSTOMPLACEMENTBUTTON_NEW_H

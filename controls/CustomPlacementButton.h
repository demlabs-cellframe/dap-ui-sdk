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

enum class ImagePos {Left, Right};

/** @brief QPushButton with subControls "text" and "image"
 *
 *  @details Places image and text from left to right in QHBoxLayout.
 * Set style in .css file in format
 *> #buttonName #leftSpacing {
 *>     ...; //if max-width==0, left alinment
 *> }
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
class CustomPlacementButton : public QPushButton
{
    Q_OBJECT
public:

    explicit CustomPlacementButton(QWidget *a_parent = Q_NULLPTR);
    /// Set text button.
    /// @param text Set text.
    void setText(const QString &text);
    /// Form initialization.
    /// @param path Path to Image.
    void setIcon(const QString &path);
    /// Set object name..
    /// @param name Object name.
    void setObjectName(const QString &name);
    /// Sets the state of the button.
    /// @param isHover
    /// @param isChecked
    void setState(bool isHover, bool isChecked);
    /// .
    /// @param a_id Window GUI widget.
    void addSubcontrol(QString a_id);
    /// Image Position on button.
    /// @param a_position Enum ImagePos Right or Left.
    void setImagePosition(ImagePos a_position = ImagePos::Left);

    void setGraphicsEffect(StyledDropShadowEffect *a_effect);

private:
    ///For effect.
    StyledDropShadowEffect *m_styledShadow = nullptr;
protected:
    /// Cursor in.
    /// @param event Signal source.
    void enterEvent(QEvent *event);
    /// Cursor out.
    /// @param event Signal source.
    void leaveEvent(QEvent *event);

    QHBoxLayout *m_layout;
    QLabel m_lbLeftSpacing;         ///<label for left spacing
    QList<QLabel*> m_subcontrols;
    QLabel m_lbImage;    ///<label with image
    QLabel m_lbText;     ///<label with text
    QLabel m_lbRightSpacing;        ///<label for right spacing

private:
    static void setWidgetState(QWidget* a_widget, bool a_isHover=false, bool a_isChecked = false);
};

#endif // CUSTOMPLACEMENTBUTTON_NEW_H

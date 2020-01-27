#ifndef CUSTOMPLACEMENTBUTTON_H
#define CUSTOMPLACEMENTBUTTON_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QDebug>
#include <QEvent>
#include "StyledSubcontrol.h"


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
 *> #buttonName::text:hover {
 *>     ...
 *> }
 *> #buttonName::text:checked {
 *>     ...
 *> }
 *> #buttonName::text:checked:hover {
 *>     ...
 *> }
 *> #buttonName::image {
 *>     ...
 *> }
 *> #buttonName::image:hover {
 *>     ...
 *> }
 *> #buttonName::image:checked {
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

    void setText(const QString &text);
    void setObjectName(const QString &name);
    void setCheckable(bool checkable);

    void updateStyleSheets();
    void updateAppearance();

    void addSubcontrol(QString a_id);
    void setImagePosition(ImagePos a_position = ImagePos::Left);

private:
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void changeEvent(QEvent * event);

    QHBoxLayout *m_layout;
    QLabel m_lbLeftSpacing;         ///<label for left spacing
    QList<StyledSubcontrol*> m_subcontrols;
    StyledSubcontrol *m_lbImage;    ///<label with image
    StyledSubcontrol *m_lbText;     ///<label with text
    QLabel m_lbRightSpacing;        ///<label for right spacing
};

#endif // CUSTOMPLACEMENTBUTTON_H

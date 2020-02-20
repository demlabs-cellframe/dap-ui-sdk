#ifndef CUSTOMPLACEMENTBUTTON_NEW_H
#define CUSTOMPLACEMENTBUTTON_NEW_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QDebug>
#include <QEvent>
#include <QLabel>

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
class CustomPlacementButton_New : public QPushButton
{
    Q_OBJECT
public:

    explicit CustomPlacementButton_New(QWidget *a_parent = Q_NULLPTR);

    void setText(const QString &text);
//    void setObjectName(const QString &name);
//    void setCheckable(bool checkable);

    void setState(bool isHover, bool isChecked);

    void addSubcontrol(QString a_id);
    void setImagePosition(ImagePos a_position = ImagePos::Left);

private:
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
//    void changeEvent(QEvent * event);

    QHBoxLayout *m_layout;
    QLabel m_lbLeftSpacing;         ///<label for left spacing
    QList<QLabel*> m_subcontrols;
    QLabel m_lbImage;    ///<label with image
    QLabel m_lbText;     ///<label with text
    QLabel m_lbRightSpacing;        ///<label for right spacing
};

#endif // CUSTOMPLACEMENTBUTTON_NEW_H

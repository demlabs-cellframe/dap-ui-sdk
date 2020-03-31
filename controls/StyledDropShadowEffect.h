#ifndef STYLEDDROPSHADOWEFFECT_H
#define STYLEDDROPSHADOWEFFECT_H

#include <QGraphicsDropShadowEffect>


/** @brief QGraphicsDropShadowEffect that can be adjusted from css
 *
 *  @details Set style in .css file in format
 *
 *> #elementName::shadow
 *> {
 *>    color: rgba(255, 150, 200, 255);
 *>    x: 0;
 *>    y: 10;
 *>    blur: 30;
 *>}
 * @todo Is searching style in comments also!
*/

///@details Structure with shadow settings
struct ShadowProperties
{
    int x;
    int y;
    int blur;
    QColor color;
};

enum StyleShedow{DEFAULT_SHADOW,HOVER_SHADOW};

class StyledDropShadowEffect : public QGraphicsDropShadowEffect
{
    Q_OBJECT
public:
    StyledDropShadowEffect(QObject *a_parent = Q_NULLPTR);
    ///@details Setting the shadow
    void updateStyle(StyleShedow a_style);
    ///@details Collecting data from css
    void updateStyleProperties();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void mouseEnter();
    void mouseLaeve();
private:
    ///@details Saving data to a shadow structure
    /// @param  a_property String with settings from css.
    /// @param data Data structure with shadow settings.
    void setDataShadowProperties(const QString &a_property, ShadowProperties &data);
    ///@details Set shadow options
    /// @param data Data structure with shadow settings.
    void setShadowProperties(ShadowProperties &data);
    ///@details Default Shadow Data
    ShadowProperties defaultShadow;
    ///@details Hover Shadow Data
    ShadowProperties hoverShadow;
};

#endif // STYLEDDROPSHADOWEFFECT_H

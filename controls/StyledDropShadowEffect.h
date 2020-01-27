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
class StyledDropShadowEffect : public QGraphicsDropShadowEffect
{
public:
    StyledDropShadowEffect(QObject *a_parent = Q_NULLPTR);

    void updateStyle();
};

#endif // STYLEDDROPSHADOWEFFECT_H

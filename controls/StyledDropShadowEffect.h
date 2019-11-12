#ifndef STYLEDDROPSHADOWEFFECT_H
#define STYLEDDROPSHADOWEFFECT_H

#include <QGraphicsDropShadowEffect>

class StyledDropShadowEffect : public QGraphicsDropShadowEffect
{
public:
    StyledDropShadowEffect(QObject *a_parent = Q_NULLPTR);

    void updateStyle();
};

#endif // STYLEDDROPSHADOWEFFECT_H

#ifndef CUSTOMPOPUP_H
#define CUSTOMPOPUP_H

#include "AdaptiveWidget.h"
#include "ScreenOverlaying.h"

class CustomPopup : public AdaptiveWidget
{
    Q_OBJECT

    Q_PROPERTY(Qt::WindowType windowType WRITE setWindowType READ windowType DESIGNABLE true) // Popup/Widget/Dialog

    Q_PROPERTY(qreal screenOverlayingOpacity WRITE setScreenOverlayingOpacity  DESIGNABLE true)
    Q_PROPERTY(qreal screenOverlayingBlurRadius WRITE setScreenOverlayingBlurRadius  DESIGNABLE true)
public:
    CustomPopup(QMainWindow *a_parent);

    void setScreenOverlayingBlurRadius(qreal a_blurRadius);

    void setScreenOverlayingOpacity(qreal a_opacity);

    Qt::WindowType windowType() const;
    void setWindowType(Qt::WindowType a_windowType);

public slots:
    void show();
    virtual void hide();

protected:
    virtual void initVariantUi(QWidget * a_widget) override = 0;

    virtual void resizeEvent(QResizeEvent *a_event) override;

    void updatePosition();

private:
    void createScreenOverlaying();

    ScreenOverlaying *m_screenOverlaying{};
    bool m_clouseOnClickOutside;

    Qt::WindowType m_windowType;

};

#endif // CUSTOMPOPUP_H

#ifndef CUSTOMPOPUP_H
#define CUSTOMPOPUP_H

#include "AdaptiveWidget.h"
#include "ScreenOverlaying.h"

class CustomPopup : public AdaptiveWidget
{
    Q_OBJECT

    Q_PROPERTY(Qt::WindowType windowType WRITE setWindowType READ windowType DESIGNABLE true) // Popup/Widget/Dialog/Desktop

    Q_PROPERTY(qreal screenOverlayingOpacity    WRITE setScreenOverlayingOpacity    READ screenOverlayingOpacity    DESIGNABLE true)
    Q_PROPERTY(qreal screenOverlayingBlurRadius WRITE setScreenOverlayingBlurRadius READ screenOverlayingBlurRadius DESIGNABLE true)
public:
    CustomPopup(QWidget *a_parent = nullptr);

    void setParent(QWidget *a_parent);

    qreal screenOverlayingBlurRadius() const;
    void setScreenOverlayingBlurRadius(qreal a_blurRadius);

    qreal screenOverlayingOpacity() const;
    void setScreenOverlayingOpacity(qreal a_opacity);

    Qt::WindowType windowType() const;
    void setWindowType(Qt::WindowType a_windowType);

signals:
    void opened();
    void closed();

protected:
    virtual void initVariantUi(QWidget * a_widget) override = 0;

    virtual void resizeEvent(QResizeEvent *a_event) override;

    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

    void updatePosition();

private:
    void createScreenOverlaying();

    ScreenOverlaying *m_screenOverlaying{};
    bool m_clouseOnClickOutside;

    Qt::WindowType m_windowType;

};

#endif // CUSTOMPOPUP_H

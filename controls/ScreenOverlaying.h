#ifndef SCREENOVERLAYING_H
#define SCREENOVERLAYING_H

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QGraphicsBlurEffect>
#include <QMainWindow>

/* css style
 #ScreenBlureOpocityEffect
{
    background-color: color
    qproperty-blureRadius: integer property
    qproperty-opacity: double property 0...1
}

 */
class ScreenOverlaying: public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal blurRadius WRITE setBlurRadius DESIGNABLE true)
    Q_PROPERTY(qreal opacity WRITE setOpacity DESIGNABLE true)
public:
    ScreenOverlaying(QMainWindow *a_parent = nullptr);

    /// Setting a parameter for the BlurEffect.
    /// @param a_blureRadius.
    void setBlurRadius(qreal a_blurRadius);
    /// Setting a parameter for the OpacityEffect. The value must be between 0 and 1.
    /// @param a_opocity.
    void setOpacity(qreal a_opacity);
protected:

    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override;



signals:
    void clicked();

private:
    bool acceptBlurEffect();

    float m_blurRadius = 0.0;
    QGraphicsBlurEffect* m_blurEffect = nullptr;

};


#endif // SCREENOVERLAYING_H

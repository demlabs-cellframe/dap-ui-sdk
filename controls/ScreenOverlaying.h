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
    qproperty-opocity: double property 0...1
}

 */
class ScreenOverlaying: public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int blurRadius WRITE setBlurRadius DESIGNABLE true)
    Q_PROPERTY(qreal opacity WRITE setOpacity DESIGNABLE true)
public:
    ScreenOverlaying(QMainWindow *a_parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    /// Setting a parameter for the BlurEffect.
    /// @param a_blureRadius.
    void setBlurRadius(int &a_blurRadius);
    /// Setting a parameter for the OpacityEffect.
    /// @param a_opocity.
    void setOpacity(qreal &a_opacity);

private:
    QWidget* m_blureEffect;

    int m_blurRadius = 0;
    qreal m_opacity = 0;
};
#endif // SCREENOVERLAYING_H

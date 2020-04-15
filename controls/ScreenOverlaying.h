#ifndef SCREENOVERLAYING_H
#define SCREENOVERLAYING_H

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QGraphicsBlurEffect>

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
    ScreenOverlaying(QWidget *a_parent);

    /// Constructor with two widgets specified. 1st widget for creating blurring.
    ///  The second widget must be the parent of the first and the parent
    /// of this class to create a transparent background.
    /// @param blure_parent.
    /// @param opocity_parent
    ScreenOverlaying(QWidget *blur_parent, QWidget *opacity_parent);

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

   QWidget* m_blurParent;
   QWidget* m_opacityParent;

   int blurRadius;
   qreal opacity;
};
#endif // SCREENOVERLAYING_H

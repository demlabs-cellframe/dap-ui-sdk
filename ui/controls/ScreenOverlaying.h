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
    Q_PROPERTY(qreal blurRadius WRITE setBlurRadius READ blurRadius DESIGNABLE true)
    Q_PROPERTY(qreal opacity    WRITE setOpacity    READ opacity    DESIGNABLE true)

public:
    ScreenOverlaying(QMainWindow *a_parent = nullptr);

    void setParent(QMainWindow *a_parent);

    qreal blurRadius() const;
    void setBlurRadius(qreal a_blurRadius);

    qreal opacity() const;
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

    QString *m_backgroundColor = nullptr;

};


#endif // SCREENOVERLAYING_H

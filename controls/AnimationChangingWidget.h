#ifndef ANIMATIONCHANGINGWIDGET_H
#define ANIMATIONCHANGINGWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPropertyAnimation>
#include <QStyle>

/**
 * @brief Widget that change content by animation transition
 */
class AnimationChangingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationChangingWidget(QWidget *a_parent = nullptr);

    void insertWidget   (int a_index, QWidget *a_widget);
    void removeWidget   (int a_index);
    void removeWidget   (QWidget *a_widget);
    void replaceWidget  (int a_index, QWidget *a_widget);

    int  indexOf         (QWidget *a_widget);

    void setCurrentWidget(QWidget *a_widget);
    void setCurrentIndex (int a_index);

    int  currentIndex ();

    int  count() const;

signals:
    void animationFinished();                ///< Emits this signal after the animation transition has reached the end.

protected:
    void updateWidgetSize();

private:
    QWidget             m_widget;            ///< Widget in wich will be placed all widgets side by side. Will move when currentIndex will be chenged
    QBoxLayout          m_ltWidgetPlacement; ///< Layout for placing all widgets side by side

    QPropertyAnimation  m_animation;         ///< Animation thet will implement moving of m_widget when currentIndex will be chenged
    int                 m_currentIndex;      ///< Current index
};



#endif // ANIMATIONCHANGINGWIDGET_H

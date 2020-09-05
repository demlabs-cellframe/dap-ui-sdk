#include "AnimationChangingWidget.h"
#include <QDebug>

class QLayout;

/**
 * @brief Create AnimationChangingWidget
 * @param a_parent object parent
 */
AnimationChangingWidget::AnimationChangingWidget(QWidget *a_parent) :
    QWidget(a_parent),
    m_widget(this),
    m_ltWidgetPlacement(QBoxLayout::LeftToRight, &m_widget),
    m_animation(&m_widget, "pos", &m_widget)
{
    show();     //needed for right positioning

    //Initialization of Layout for placing all widgets
    m_ltWidgetPlacement.setDirection(QBoxLayout::LeftToRight);
    m_ltWidgetPlacement.setSpacing(0);
    m_ltWidgetPlacement.setContentsMargins(0, 0, 0, 0);
    m_widget.setLayout(&m_ltWidgetPlacement);

    //Animation setup
    m_animation.setDuration(100);
    m_animation.setEasingCurve(QEasingCurve::OutCirc);

    connect(&m_animation, SIGNAL(finished()), this, SIGNAL(animationFinished()));
    connect(this, SIGNAL(animationFinished()), SLOT(hideUnactiveWidgets()));
}

/**
 * @brief Remove widget by index
 * @param a_index index of removing widget
 */
void AnimationChangingWidget::removeWidget(int a_index)
{
    //Find Index:
    QLayoutItem *existingItem = m_ltWidgetPlacement.itemAt(a_index);
    Q_ASSERT_X(existingItem, "removeWidget", "Index %1 not found!");

    removeWidget(existingItem->widget());
}

/**
 * @brief Remove widget by pointer
 * @param a_widget Pointer to removing widget
 */
void AnimationChangingWidget::removeWidget(QWidget *a_widget)
{
    // Remove and hide:
    if(this->indexOf(a_widget) < m_currentIndex)
        m_currentIndex--;

    m_ltWidgetPlacement.removeWidget(a_widget);
    a_widget->hide();
}

/**
 * @brief Index of a_widget
 * @param a_widget widget
 * @return Index a_widget
 */
int AnimationChangingWidget::indexOf(QWidget *a_widget)
{
    return m_ltWidgetPlacement.indexOf(a_widget);
}


/**
 * @brief Count of widgets
 */
int AnimationChangingWidget::count() const
{
    return m_ltWidgetPlacement.count();
}

/**
 * @brief Insert widget
 * @param a_index Index that will be assigned to a_widget
 * @param a_widget Pointer to inserting widget
 */
void AnimationChangingWidget::insertWidget(int a_index, QWidget *a_widget)
{
    m_ltWidgetPlacement.insertWidget(a_index, a_widget);
    if (a_index >=0 && a_index <= this->currentIndex())
        m_currentIndex++;

    a_widget->hide();
}

/**
 * @brief Change current index to index of a_widget and start animation transition
 * @param a_widget Pointer to widget that will be current
 */
void AnimationChangingWidget::setCurrentWidget(QWidget *a_widget)
{
    QWidget *wgParent = a_widget->parentWidget();   Q_ASSERT(wgParent); // parent
    QLayout *ltParent = wgParent->layout();         Q_ASSERT(ltParent); // parent layout
    int index = ltParent->indexOf(a_widget);        Q_ASSERT(index!=-1);// a_widget index

    setCurrentIndex(index);
}

/**
 * @brief Change current index to a_index and start animation transition
 * @param a_index Index of widget that will be current
 */
void AnimationChangingWidget::setCurrentIndex(int a_index)
{
    //Q_ASSERT_X((a_index <= (count()-1)), "setCurrentIndex", "Index doesn't exist");
    if (m_currentIndex == a_index)
        return;
    bool animationIsNeeded = (this->currentIndex() != -1); // if no index was current, animation is not needed

    this->widgetAt(a_index)->show();
    this->updateWidgetSize();

    if (animationIsNeeded)
    {
        // Animation setup and start:
        QPoint endPos = -this->widgetPosition(a_index);

#ifdef ANDROID
        //TODO: Refactoring
        m_widget.move(endPos);
        m_currentIndex = a_index;
        emit this->animationFinished();
        return;
#else
        m_animation.setStartValue(m_widget.pos());
        m_animation.setEndValue(endPos);
        m_animation.start();
#endif

    }
    m_currentIndex = a_index;
}

QWidget *AnimationChangingWidget::currentWidget()
{
    return this->widgetAt(this->currentIndex());
}

/**
 * @brief Getter for currentIndex
 * @return Current index
 */
int AnimationChangingWidget::currentIndex()
{
    return m_currentIndex;
}

/**
 * @brief Update size of container widget for fitting all including widgets
 */
void AnimationChangingWidget::updateWidgetSize()
{
    int countOfVisible = 0;
    int oneScreenWidth = this->width();

    if (currentIndex() == -1)
        return m_widget.resize(oneScreenWidth, height()); ;

    for (int i = 0; i < this->count(); i++)
    {
        if (i == this->currentIndex())
        {
            int xPos = -countOfVisible * oneScreenWidth; //countOfVisible - count of visible screens placed to the left of current screen.
            m_widget.move(QPoint(xPos, 0));
        }

        if (this->widgetAt(i) && this->widgetAt(i)->isVisible())
            countOfVisible++;
    }
    m_widget.resize(oneScreenWidth * countOfVisible, height());
}

void AnimationChangingWidget::hideUnactiveWidgets()
{
    for (int i = 0; i < this->count(); i++)
    {
        if (i != this->currentIndex())
            this->widgetAt(i)->hide();
    }
    this->updateWidgetSize();
}

/**
 * @brief Change current index to a_index and start animation transition
 * @param a_index Index of widget that will be current
 */
void AnimationChangingWidget::replaceWidget(int a_index, QWidget *a_widget)
{
    QLayoutItem *existingItem = m_ltWidgetPlacement.itemAt(a_index);
    // If a_widget does not exist or exist but isn't the same: ...
    if (!existingItem or (existingItem and existingItem->widget() != a_widget)) {
        // If exist but isn't the same -
        if (existingItem)
            // 1. remove existing
            removeWidget(existingItem->widget());
        // 2. insert
        insertWidget(a_index, a_widget);
    }
    // If a_widget exist and is the same do nothing
}

/**
 * @brief Return widget from layout placed at a_index
 * @param a_index Index of widget that will be returned
 * @return QWidget at a_index
 */
QWidget *AnimationChangingWidget::widgetAt(int a_index)
{
    QLayoutItem* layoutItem = m_ltWidgetPlacement.itemAt(a_index);
    if (!layoutItem)
        return nullptr;

    return layoutItem->widget();
}

/**
 * @brief Return widget with index a_index position if its existed
 * @param a_index Index of widget
 * @return QPoint position of witget with a_index index
 */
QPoint AnimationChangingWidget::widgetPosition(int a_index)
{
    QWidget* widget = this->widgetAt(a_index);

    if (!widget)
        return QPoint();

    return widget->pos();
}


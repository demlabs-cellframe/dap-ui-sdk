/* INCLUDES */
#include "kelguiwidget.h"

#include <QStyleOption>
#include <QPainter>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiWidget::KelGuiWidget(QWidget *parent)
  : QWidget(parent)
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

bool KelGuiWidget::frame() const
{
  return m_frame;
}

void KelGuiWidget::setFrame(bool frame)
{
  m_frame = frame;
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiWidget::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init (this);

  QPainter p (this);
  QWidget::style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

/*-----------------------------------------*/

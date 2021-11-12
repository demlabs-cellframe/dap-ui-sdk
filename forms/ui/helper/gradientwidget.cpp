/* INCLUDES */
#include "gradientwidget.h"
#include <QPaintEvent>
#include <QPainter>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

GradientWidget::GradientWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gradient.setColorAt(0, "#E62CC7");
  m_gradient.setColorAt(1, "#F53172");
}

/********************************************
 * OVERRIDE
 *******************************************/

void GradientWidget::paintEvent(QPaintEvent *)
{
  QPainter painter (this);
  m_gradient.setStart (0, 0);
  m_gradient.setFinalStop (width(), height());
  painter.fillRect(QRect(0, 0, width(), height()), m_gradient);
}

/*-----------------------------------------*/

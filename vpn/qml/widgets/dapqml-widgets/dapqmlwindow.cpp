/* INCLUDES */
#include "dapqmlwindow.h"
#include <QDebug>
#include "dapqmlstyle.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlWindow::DapQmlWindow()
{
  qDebug() << __FUNCTION__;

  /* signals */
  connect (this, &QWindow::widthChanged, [this] { this->_resized(); });
  connect (this, &QWindow::heightChanged, [this] { this->_resized(); });
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlWindow::_resized()
{
  int w = width(), h = height();
  DapQmlStyle::sWindowResized (w, h);
  emit resized (w, h);
}

/*-----------------------------------------*/

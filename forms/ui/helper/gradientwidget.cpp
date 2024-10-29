/* INCLUDES */
#include "gradientwidget.h"
#include "style/dapguistylemanager.h"
#include <QPaintEvent>
#include <QPainter>

/* DEFS */
class StyleAccess : public DapGuiStyleManager
{
public:
  static QString styleByClassList (const QStringList &classNameList)
  {
    return DapGuiStyleManager::styleByClassList (classNameList);
  }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

GradientWidget::GradientWidget(QWidget *parent)
  : QWidget(parent)
{
  auto color1 = StyleAccess::styleByClassList({"gradientcolor1"});
  auto color2 = StyleAccess::styleByClassList({"gradientcolor2"});
  color1  = color1.remove("color:").remove(";");
  color2  = color2.remove("color:").remove(";");
  m_gradient.setColorAt(0, color1); //"#E62CC7");
  m_gradient.setColorAt(1, color2); //"#F53172");
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

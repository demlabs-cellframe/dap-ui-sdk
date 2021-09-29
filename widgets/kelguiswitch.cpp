/* INCLUDES */
#include "kelguiswitch.h"
#include "ui_kelguiswitch.h"

#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiSwitch::KelGuiSwitch (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::KelGuiSwitch)
  , m_checked (false)
{
  /* setup style */
  ui->setupUi (this);
  __kgsm.forcedStyleUpdate();
}

/********************************************
 * METHODS
 *******************************************/

bool KelGuiSwitch::isChecked() const
{
  return m_checked;
}

void KelGuiSwitch::setChecked(bool a_checked)
{
  m_checked = a_checked;
  emit stateChanged (m_checked);
}

void KelGuiSwitch::toggle()
{
  setChecked (!isChecked());
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiSwitch::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    {
      emit clicked();
      toggle();
      _debugInfoClipboard();
    }
}

/********************************************
 *
 *******************************************/

void KelGuiSwitch::_debugInfoClipboard()
{
#ifdef QT_DEBUG
  if(qApp->keyboardModifiers() & Qt::AltModifier)
    {
      QJsonObject jobj;
      jobj["cssStyle"]      = QJsonObject{
        {"name", __kgsm.cssStyle()},
        {"text", styleSheet()},
        {"switch_bg_css", ui->switch_bg->cssStyle()},
        {"switch_bg_qss", ui->switch_bg->styleSheet()},
        {"switch_toggle_css", ui->switch_toggle->cssStyle()},
        {"switch_toggle_qss", ui->switch_toggle->styleSheet()},
      };
      jobj["rect"] = QJsonObject{
        {"x",pos().x()},
        {"y",pos().y()},
        {"width",size().width()},
        {"heigh",size().height()},
      };
      auto dump = QJsonDocument (jobj).toJson();
      qApp->clipboard()->setText (dump);
    }
#endif // QT_DEBUG
}

/*-----------------------------------------*/

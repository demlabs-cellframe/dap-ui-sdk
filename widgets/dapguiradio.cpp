/* INCLUDES */
#include "dapguiradio.h"
#include "ui_dapguiradio.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiRadioBase::DapGuiRadioBase (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::DapGuiRadioButtonUI)
  , m_checked (false)
  , m_separator (false)
{
  /* setup ui */
  ui->setupUi (this);
  setChecked (checked());
  setSeparator (separator());
  setTextCssClass ("crb_textstyle font16 darkblue normal lato");
  setCheckedCssClass ("crb_indicator crb_on");
  setUncheckedCssClass ("crb_indicator crb_off");

  ui->indicator_quality->setVisible(false);

  /* signals */
  connect (ui->label, &DapGuiLabel::clicked,
           this, &DapGuiRadioBase::clicked);
  connect (ui->indicator_checked, &DapGuiLabel::clicked,
           this, &DapGuiRadioBase::clicked);
  connect (ui->indicator_unchecked, &DapGuiLabel::clicked,
           this, &DapGuiRadioBase::clicked);
  connect (ui->indicator_quality, &DapGuiLabel::clicked,
          this, &DapGuiRadioBase::clicked);
}

/********************************************
 * METHODS
 *******************************************/



/****************************************//**
 * @property DapGuiRadioBase::text
 * @brief text label's text
 * @note stored inside DapGuiLabel
 * @accessors %text(), %setText()
 *******************************************/

QString DapGuiRadioBase::text() const
{
  return ui->label->text();
}

void DapGuiRadioBase::setText(const QString &a_text)
{
  ui->label->setText (a_text);
}

DapServerInfo::connectionQuality DapGuiRadioBase::qulityIcon() const
{
  return m_connetion_quality;
}

void DapGuiRadioBase::setQulityIcon(const DapServerInfo::connectionQuality connetion_quality)
{
  m_connetion_quality = connetion_quality;

  switch (connetion_quality) {
    case DAP_CONNECTION_QUALITY::NO_CONNECTION:
      setQualityCssClass("crb_indicator_connection crb_no_connection");
      break;
    case DAP_CONNECTION_QUALITY::FULL:
      setQualityCssClass("crb_indicator_connection crb_full");
      break;
    case DAP_CONNECTION_QUALITY::ALMOST_FULL:
      setQualityCssClass("crb_indicator_connection crb_almost_full");
      break;
    case DAP_CONNECTION_QUALITY::MIDDLE:
      setQualityCssClass("crb_indicator_connection crb_middle");
      break;
    case DAP_CONNECTION_QUALITY::LOW:
      setQualityCssClass("crb_indicator_connection crb_low");
      break;
    case DAP_CONNECTION_QUALITY::VERY_LOW:
      setQualityCssClass("crb_indicator_connection crb_very_low");
      break;

    default:
      setQualityCssClass("crb_indicator_connection crb_no_connection");
      break;
  }
}

void DapGuiRadioBase::setPingToolTip(const qint16 a_ping)
{
  if (a_ping == -1){
    ui->indicator_quality->setToolTip("unavailable");
    return;
  }
  ui->indicator_quality->setToolTip("ping - " + QString::number(a_ping) + "ms");
}

/****************************************//**
 * @property DapGuiRadioBase::textCssClass
 * @brief text label's css style
 * @note stored inside DapGuiLabel
 * @accessors %textCssClass(), %setTextCssClass()
 *******************************************/

QString DapGuiRadioBase::textCssClass() const
{
  return ui->label->cssStyle();
}

void DapGuiRadioBase::setTextCssClass(const QString &a_cssClass)
{
  ui->label->setCssStyle (a_cssClass);
}

/****************************************//**
 * @property DapGuiRadioBase::checkedCssClass
 * @brief checked label's css style
 * @note stored inside DapGuiLabel
 * @accessors %checkedCssClass(), %setCheckedCssClass()
 *******************************************/

QString DapGuiRadioBase::checkedCssClass() const
{
  return ui->indicator_checked->cssStyle();
}

void DapGuiRadioBase::setCheckedCssClass(const QString &a_cssClass)
{
  ui->indicator_checked->setCssStyle (a_cssClass);
}

/****************************************//**
 * @property DapGuiRadioBase::uncheckedCssClass
 * @brief unchecked label's css style
 * @note stored inside DapGuiLabel
 * @accessors %uncheckedCssClass(), %setUncheckedCssClass()
 *******************************************/

QString DapGuiRadioBase::uncheckedCssClass() const
{
  return ui->indicator_unchecked->cssStyle();
}

void DapGuiRadioBase::setUncheckedCssClass(const QString &a_cssClass)
{
  ui->indicator_unchecked->setCssStyle (a_cssClass);
}

/****************************************//**
 * @property DapGuiRadioBase::qualityCssClass
 * @brief quality label's css style
 * @note stored inside DapGuiLabel
 * @accessors %qualityCssClass(), %setQualityCssClass()
 *******************************************/


QString DapGuiRadioBase::qualityCssClass() const
{
  return ui->indicator_quality->cssStyle();
}

void DapGuiRadioBase::setQualityCssClass(const QString &a_cssClass)
{
  ui->indicator_quality->setVisible(true);
  ui->indicator_quality->setCssStyle (a_cssClass);
}

/****************************************//**
 * @property DapGuiRadioBase::checked
 * @brief radio state
 * @accessors %checked(), %setIndicatorChecked(), %setChecked()
 *******************************************/

bool DapGuiRadioBase::checked() const
{
  return m_checked;
}

void DapGuiRadioBase::setIndicatorChecked(bool a_newChecked)
{
  m_checked = a_newChecked;
  ui->indicator_checked->setVisible (m_checked);
  ui->indicator_unchecked->setVisible (!m_checked);
}

void DapGuiRadioBase::setChecked(bool a_newChecked)
{
  setIndicatorChecked(a_newChecked);
  emit toggled (a_newChecked);
}

/****************************************//**
 * @property DapGuiRadioBase::separator
 * @brief separator show/hide
 * @accessors %separator(), %setSeparator()
 *******************************************/

bool DapGuiRadioBase::separator() const
{
  return m_separator;
}

void DapGuiRadioBase::setSeparator(bool a_separator)
{
  m_separator = a_separator;
  ui->separator->setVisible (a_separator);
}

/********************************************
 * SLOTS
 *******************************************/




/****************************************//**
 * @brief send signal and uncheck other radio's
 *
 * Will try to control other instances of
 * DapGuiRadioBase classes inside parent's layout
 * when clicked (checked/unchecked)
 *******************************************/
void DapGuiRadioBase::_slotClicked()
{
  /* get parent layout */
  auto layout =
    (parentWidget())
    ? (parentWidget()->layout())
    : (nullptr);

  /* change states of layout items */

  /* swap state */
//  if (!layout)
//    setChecked (!checked());
//  else if(!checked())
//    setChecked (true);
  setChecked (true);

  /* if parent has layout */
  if(layout)
    {
      /* list every layout item */
      for(auto i = 0, e = layout->count(); i < e; i++)
        {
          /* get item */
          auto item = layout->itemAt(i);

          /* if can be converted to radio */
          if (auto radio = qobject_cast<DapGuiRadioBase*> (item->widget()))

            /* if not this */
            if(radio != this)

              /* uncheck */
              radio->setChecked (false);
        }
    }
}

/*-----------------------------------------*/

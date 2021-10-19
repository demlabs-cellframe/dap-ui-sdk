/* INCLUDES */
#include "dapguiradio.h"
#include "ui_dapguiradio.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiRadio::DapGuiRadio (QWidget *parent)
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

  /* signals */
  connect (ui->label, &DapGuiLabel::clicked,
           this, &DapGuiRadio::_slotClicked);
  connect (ui->indicator_checked, &DapGuiLabel::clicked,
           this, &DapGuiRadio::_slotClicked);
  connect (ui->indicator_unchecked, &DapGuiLabel::clicked,
           this, &DapGuiRadio::_slotClicked);
}

/********************************************
 * METHODS
 *******************************************/

QString DapGuiRadio::text() const
{
  return ui->label->text();
}

void DapGuiRadio::setText(const QString &a_text)
{
  ui->label->setText (a_text);
}

QString DapGuiRadio::textCssClass() const
{
  return ui->label->cssStyle();
}

void DapGuiRadio::setTextCssClass(const QString &a_cssClass)
{
  ui->label->setCssStyle (a_cssClass);
}

QString DapGuiRadio::checkedCssClass() const
{
  return ui->indicator_checked->cssStyle();
}

void DapGuiRadio::setCheckedCssClass(const QString &a_cssClass)
{
  ui->indicator_checked->setCssStyle (a_cssClass);
}

QString DapGuiRadio::uncheckedCssClass() const
{
  return ui->indicator_unchecked->cssStyle();
}

void DapGuiRadio::setUncheckedCssClass(const QString &a_cssClass)
{
  ui->indicator_unchecked->setCssStyle (a_cssClass);
}

bool DapGuiRadio::checked() const
{
  return m_checked;
}

void DapGuiRadio::setChecked(bool a_newChecked)
{
  m_checked = a_newChecked;
  ui->indicator_checked->setVisible (m_checked);
  ui->indicator_unchecked->setVisible (!m_checked);
  emit toggled (a_newChecked);
}

bool DapGuiRadio::separator() const
{
  return m_separator;
}

void DapGuiRadio::setSeparator(bool a_separator)
{
  m_separator = a_separator;
  ui->separator->setVisible (a_separator);
}

/********************************************
 * SLOTS
 *******************************************/

void DapGuiRadio::_slotClicked()
{
  /* send signal */
  emit clicked();

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
          if (auto radio = qobject_cast<DapGuiRadio*> (item->widget()))

            /* if not this */
            if(radio != this)

              /* uncheck */
              radio->setChecked (false);
        }
    }
}

/*-----------------------------------------*/

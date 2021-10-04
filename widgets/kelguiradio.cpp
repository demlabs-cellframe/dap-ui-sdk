/* INCLUDES */
#include "kelguiradio.h"
#include "ui_kelguiradio.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiRadio::KelGuiRadio (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::KelGuiRadioButtonUI)
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
  connect (ui->label, &KelGuiLabel::clicked,
           this, &KelGuiRadio::_slotClicked);
  connect (ui->indicator_checked, &KelGuiLabel::clicked,
           this, &KelGuiRadio::_slotClicked);
  connect (ui->indicator_unchecked, &KelGuiLabel::clicked,
           this, &KelGuiRadio::_slotClicked);
}

/********************************************
 * METHODS
 *******************************************/

QString KelGuiRadio::text() const
{
  return ui->label->text();
}

void KelGuiRadio::setText(const QString &a_text)
{
  ui->label->setText (a_text);
}

QString KelGuiRadio::textCssClass() const
{
  return ui->label->cssStyle();
}

void KelGuiRadio::setTextCssClass(const QString &a_cssClass)
{
  ui->label->setCssStyle (a_cssClass);
}

QString KelGuiRadio::checkedCssClass() const
{
  return ui->indicator_checked->cssStyle();
}

void KelGuiRadio::setCheckedCssClass(const QString &a_cssClass)
{
  ui->indicator_checked->setCssStyle (a_cssClass);
}

QString KelGuiRadio::uncheckedCssClass() const
{
  return ui->indicator_unchecked->cssStyle();
}

void KelGuiRadio::setUncheckedCssClass(const QString &a_cssClass)
{
  ui->indicator_unchecked->setCssStyle (a_cssClass);
}

bool KelGuiRadio::checked() const
{
  return m_checked;
}

void KelGuiRadio::setChecked(bool a_newChecked)
{
  m_checked = a_newChecked;
  ui->indicator_checked->setVisible (m_checked);
  ui->indicator_unchecked->setVisible (!m_checked);
  emit toggled (a_newChecked);
}

bool KelGuiRadio::separator() const
{
  return m_separator;
}

void KelGuiRadio::setSeparator(bool a_separator)
{
  m_separator = a_separator;
  ui->separator->setVisible (a_separator);
}

/********************************************
 * SLOTS
 *******************************************/

void KelGuiRadio::_slotClicked()
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
          if (auto radio = qobject_cast<KelGuiRadio*> (item->widget()))

            /* if not this */
            if(radio != this)

              /* uncheck */
              radio->setChecked (false);
        }
    }
}

/*-----------------------------------------*/

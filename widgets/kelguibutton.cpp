/* INCLUDES */
#include "kelguibutton.h"
#include "ui_kelguibutton.h"
#include "kelguicommon.h"

#include <QFile>
#include <QStyleOption>
#include <QPainter>

#include <QMouseEvent>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPair>
//#include <QClipboard>
//#include <QApplication>

/* DEFS */
/// helps to decide value
template<class T>
class BoolHelper
{
public:
  struct Setup
  {
    T clear;  // when boolean is clear
    T set;    // when boolean is set
  } m_first, m_second;

  BoolHelper (Setup first, Setup second) : m_first (first), m_second (second) {}

  T operator() (const bool &first, const bool &second) const
  {
    return T (
             int (first ? m_first.set : m_first.clear)
             | int (second ? m_second.set : m_second.clear)
           );
  }
};

#define LINK_WIDTH (22)

/* VARS */
static thread_local BoolHelper<Qt::AlignmentFlag> s_mainHelper (
{Qt::AlignHCenter, Qt::AlignLeft},    // left is visible
{Qt::AlignVCenter, Qt::AlignBottom}); // sub is not empty

/* FUNCS */
template<class T, class U>
T *as (U *u)
{
  static T s;
  auto r = dynamic_cast<T *> (u);
  return (r != nullptr) ? r : &s;
}

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiButton::KelGuiButton (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::KelGuiButtonUI)
  , m_btnStyle (TopMainBottomSub)
  , m_mainText ("Main text")
  , m_subText ("Sub text")
  , m_leftText ("$0.00")
  , m_mainCssClass ("darkblue normalbold")
  , m_subCssClass ("gray uppercase")
  , m_leftCssClass ("darkblue normalbold")
  , m_link (false)
  , m_frame (false)
  , m_effect (new QGraphicsDropShadowEffect)
  , m_lLink (new QLabel (this))
{
  /* setup style */
  ui->setupUi (this);

  /* fill widgets map */
  m_widgets.insert ("styledWidgets", ui->Style0);
  m_widgets.insert ("styledWidgets", ui->Style1);
  m_widgets.insert ("styledWidgets", ui->Style2);
  m_widgets.insert ("styledWidgets", ui->Style3);
  m_widgets.insert ("textMain", ui->KelGuiButtonTextMain);
  m_widgets.insert ("textMain", ui->KelGuiButtonTextMain_2);
  m_widgets.insert ("textMain", ui->KelGuiButtonTextMain_3);
  m_widgets.insert ("textMain", ui->KelGuiButtonTextMain_4);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub_2);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub_3);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub_4);
  m_widgets.insert ("marginOnLink", ui->KelGuiButtonTextSub_4);
  m_widgets.insert ("textLeft", ui->KelGuiButtonTextLeft);

  /* prepare link icon */
  m_lLink->setPixmap (QPixmap ("://gfx/ic_arrow-right.png"));
  m_lLink->setMinimumWidth (LINK_WIDTH);
  m_lLink->setMaximumWidth (LINK_WIDTH);
  m_lLink->setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
  m_lLink->setParent (ui->KelGuiButtonBackground);
  m_lLink->hide();

  /* final style manip */
  auto labelStyle = Common::fromFile ("://styles/buttonlabel.css");
  for (auto i = m_widgets.begin(), e = m_widgets.end(); i != e; i++)
    if (i.key() != "styledWidgets")
      (*i)->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextMain->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextMain_2->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextMain_3->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextMain_4->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextSub->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextSub_2->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextSub_3->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextSub_4->setStyleSheet (labelStyle);
//  ui->KelGuiButtonTextLeft->setStyleSheet (labelStyle);

  setIcon (m_icon);
  setupStyle();
  setSeparator (false);

  /* setup shadow effect */
  m_effect->setBlurRadius (40);
  m_effect->setXOffset (10);
  m_effect->setYOffset (10);
  m_effect->setColor (QColor::fromRgba (qRgba (0x40, 0x14, 0x24, 0x24)));
  m_effect->setEnabled (true);

  setGraphicsEffect (m_effect);
  m_effect->setEnabled (false);
}

KelGuiButton::~KelGuiButton()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

KelGuiButton::ButtonStyle KelGuiButton::btnStyle() const
{
  return m_btnStyle;
}

void KelGuiButton::setBtnStyle (const ButtonStyle &style)
{
  m_btnStyle = style;
  setupLabels();

  int i = 0;
  auto array = m_widgets.values ("styledWidgets");
  std::reverse (array.begin(), array.end());
  for (auto j = array.begin(), e = array.end(); j != e; i++, j++)
    (*j)->setVisible (i == int (m_btnStyle));
}

QString KelGuiButton::mainText() const
{
  return m_mainText;
}

void KelGuiButton::setMainText (const QString &mainText)
{
  m_mainText = mainText;

  auto array = m_widgets.values ("textMain");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    as<KelGuiLabel> (*i)->setText (m_mainText);

//  ui->KelGuiButtonTextMain->setText (m_mainText);
//  ui->KelGuiButtonTextMain_2->setText (m_mainText);
//  ui->KelGuiButtonTextMain_3->setText (m_mainText);
//  ui->KelGuiButtonTextMain_4->setText (m_mainText);
}

QString KelGuiButton::subText() const
{
  return m_subText;
}

void KelGuiButton::setSubText (const QString &subText)
{
  m_subText = subText;

  auto array = m_widgets.values ("textSub");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    {
      auto j = as<KelGuiLabel> (*i);
      j->setText (m_subText);
      j->setVisible (!m_subText.isEmpty());
    }

  array = m_widgets.values ("textMain");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    {
      auto j = as<KelGuiLabel> (*i);
      j->setAlignment (
        s_mainHelper (
          btnStyle() == LeftTopMainBottomSub
          || btnStyle() == IconMainSub, !m_subText.isEmpty()));
    }

//  ui->KelGuiButtonTextSub->setVisible (!subText.isEmpty());
//  ui->KelGuiButtonTextSub_2->setVisible (!subText.isEmpty());
//  ui->KelGuiButtonTextSub_3->setVisible (!subText.isEmpty());
//  ui->KelGuiButtonTextSub_4->setVisible (!subText.isEmpty());
//  ui->KelGuiButtonTextSub->setText (m_subText);
//  ui->KelGuiButtonTextSub_2->setText (m_subText);
//  ui->KelGuiButtonTextSub_3->setText (m_subText);
//  ui->KelGuiButtonTextSub_4->setText (m_subText);
}

QString KelGuiButton::leftText() const
{
  return m_leftText;
}

void KelGuiButton::setLeftText (const QString &leftText)
{
  m_leftText = leftText;

  auto array = m_widgets.values ("textLeft");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    as<KelGuiLabel> (*i)->setText (m_leftText);

  //ui->KelGuiButtonTextLeft->setText (m_leftText);
}

QString KelGuiButton::mainCssClass() const
{
  return m_mainCssClass;
}

void KelGuiButton::setMainCssClass (const QString &mainCssClass)
{
  m_mainCssClass = mainCssClass;
  setupLabels();
}

QString KelGuiButton::subCssClass() const
{
  return m_subCssClass;
}

void KelGuiButton::setSubCssClass (const QString &subCssClass)
{
  m_subCssClass = subCssClass;
  setupLabels();
}

QString KelGuiButton::leftCssClass() const
{
  return m_leftCssClass;
}

void KelGuiButton::setLeftCssClass (const QString &leftCssClass)
{
  m_leftCssClass = leftCssClass;
  setupLabels();
}

bool KelGuiButton::link() const
{
  return m_link;
}

void KelGuiButton::setLink (bool link)
{
  m_link = link;
  m_lLink->setVisible (link);

  auto array = m_widgets.values ("marginOnLink");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    {
      auto j  = as<KelGuiLabel> (*i);
      auto js = j->cssStyle();
      j->setCssStyle (js + " margin-right28");
    }
  //setStyleSheet(fromFile ("://styles/buttonlabel.css") + "QLabel{margin-right: 28px;}");
}

bool KelGuiButton::frame() const
{
  return m_frame;
}

void KelGuiButton::setFrame (bool frame)
{
  m_frame = frame;
  setupStyle();
}

bool KelGuiButton::separator() const
{
  return m_separator;
}

void KelGuiButton::setSeparator (bool separator)
{
  m_separator = separator;
  ui->kelGuiSeparator->setVisible (m_separator);
}

QUrl KelGuiButton::icon() const
{
  return m_icon;
}

void KelGuiButton::setIcon (const QUrl &icon)
{
  m_icon = icon;
  QPixmap px (QString (":/") + m_icon.path());
  auto style =
    QString ("image: url(%1);\n"
             "min-width: %2px;\n"
             "min-height: %3px;\n"
             "max-width: %2px;\n"
             "max-height: %3px;\n")
    .arg (QString (":/") + m_icon.path())
    .arg (px.width())
    .arg (px.height());
//  m_lIcon->setStyleSheet (style);
  ui->KelGuiButtonIcon->setStyleSheet (style);
//  QApplication::clipboard()->setText(style);
}

void KelGuiButton::setupStyle()
{
  //ui->KelGuiButtonBackground->setFrame (m_frame);
  ui->KelGuiButtonBackground->setCssStyle (m_frame ? "frame" : "");
  //ui->KelGuiButtonBackground->setStyleSheet (ui->KelGuiButtonBackground->styleSheet());
  //ui->KelGuiButtonBackground->repaint();

  m_effect->setEnabled (m_frame);
  setBtnStyle (m_btnStyle);
  setStyleSheet (styleSheet());
  repaint();
}

void KelGuiButton::setupLabels()
{
  QMap <QString, QString> labelMap =
  {
    {"textMain",  mainCssClass()},
    {"textSub",   subCssClass()},
    {"textLeft",  leftCssClass()},
  };

  auto labelStyle = Common::fromFile ("://styles/buttonlabel.css");
  for (auto i = m_widgets.begin(), e = m_widgets.end(); i != e; i++)
    if (i.key() != "styledWidgets")
      as<KelGuiLabel> (*i)->setCssStyle (labelMap.value (i.key()));

//  ui->KelGuiButtonTextMain->setStyleId (mainStyle());
//  ui->KelGuiButtonTextMain_2->setStyleId (mainStyle());
//  ui->KelGuiButtonTextMain_3->setStyleId (mainStyle());
//  ui->KelGuiButtonTextMain_4->setStyleId (mainStyle());
//  ui->KelGuiButtonTextSub->setStyleId (subStyle());
//  ui->KelGuiButtonTextSub_2->setStyleId (subStyle());
//  ui->KelGuiButtonTextSub_3->setStyleId (subStyle());
//  ui->KelGuiButtonTextSub_4->setStyleId (subStyle());
//  ui->KelGuiButtonTextLeft->setStyleId (leftStyle());
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiButton::paintEvent (QPaintEvent *)
{
  QStyleOption opt;
  opt.init (this);

  QPainter p (this);
  QWidget::style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void KelGuiButton::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();
}

void KelGuiButton::resizeEvent (QResizeEvent *ev)
{
  m_lLink->move (ev->size().width() - LINK_WIDTH - 10, 0);
  m_lLink->resize (LINK_WIDTH, ev->size().height());
}

/*-----------------------------------------*/

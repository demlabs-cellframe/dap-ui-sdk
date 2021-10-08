/* INCLUDES */
#include "kelguibutton.h"
#include "ui_kelguibutton.h"
#include "kelguicommon.h"

#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>

#include <QMouseEvent>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPair>
//#include <QClipboard>
//#include <QApplication>

#include "ui/helper/auxiliary/UiScaling.h"

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

static thread_local int s_linkWidth = LINK_WIDTH;

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
  , m_mainCssClass ("darkblue normalbold font20")
  , m_subCssClass ("gray uppercase font12")
  , m_leftCssClass ("darkblue normalbold font30")
  , m_iconCssClass ("")
  , m_link (false)
  , m_frame (false)
  , m_shadowEffect (new QGraphicsDropShadowEffect)
  , m_lLink (new QLabel (this))
{
  /* setup style */
  ui->setupUi (this);
  s_linkWidth = UiScaling::pointsToPixels (LINK_WIDTH, UiScaling::getNativDPI());

  /* fill widgets map */
  m_widgets.insert ("styledWidgets", ui->Style0);
  m_widgets.insert ("styledWidgets", ui->Style1);
  m_widgets.insert ("styledWidgets", ui->Style2);
  m_widgets.insert ("styledWidgets", ui->Style3);
  m_widgets.insert ("styledWidgets", ui->Style4);
  m_widgets.insert ("textMain", ui->KelGuiButtonTextMain);
  m_widgets.insert ("textMain", ui->KelGuiButtonTextMain_2);
  m_widgets.insert ("textMain", ui->KelGuiButtonTextMain_3);
  m_widgets.insert ("textMid", ui->KelGuiButtonTextMid);
  m_widgets.insert ("textEdit", ui->kelGuiLineEditMain);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub_2);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub_3);
  m_widgets.insert ("textSub", ui->KelGuiButtonTextSub_4);
  m_widgets.insert ("textRight", ui->KelGuiButtonTextRight);
  m_widgets.insert ("marginOnLink", ui->KelGuiButtonTextRight);
  m_widgets.insert ("textLeft", ui->KelGuiButtonTextLeft);
  m_widgets.insert ("iconLeft", ui->KelGuiButtonIcon);

  /* prepare link icon */
//  m_lLink->setPixmap (QPixmap ("://gfx/ic_arrow-right.png"));
//  m_lLink->setMinimumWidth (s_linkWidth);
//  m_lLink->setMaximumWidth (s_linkWidth);
  m_lLink->setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
  m_lLink->setParent (ui->KelGuiButtonBackground);
  m_lLink->setStyleSheet (
        "background-color: rgba(0,0,0,0);"
        "border-image: url(\"://gfx/ic_arrow-right.png\");");
  m_lLink->hide();

  //setIcon (m_icon);
  setupStyle();
  setSeparator (false);

  /* setup shadow effect */
  m_shadowEffect->setBlurRadius (60);
  m_shadowEffect->setXOffset (5);
  m_shadowEffect->setYOffset (5);
  m_shadowEffect->setColor (QColor::fromRgba (qRgba (0x40, 0x14, 0x24, 0x24)));

  setGraphicsEffect (m_shadowEffect);
  m_shadowEffect->setEnabled (false);

  /* connect clicked signals */
  for (auto i = m_widgets.begin(), e = m_widgets.end(); i != e; i++)
    {
      auto *label  = qobject_cast<KelGuiLabel*>(*i);

      if(!label)
        continue;

      connect (label, &KelGuiLabel::clicked,
               this, &KelGuiButton::clicked);
#ifdef QT_DEBUG
      connect (label, &KelGuiLabel::clicked,
               this, &KelGuiButton::_slotDebugInfo);
#endif // QT_DEBUG
    }

  /* signals */
  connect (ui->kelGuiLineEditMain, &KelGuiLineEdit::textChanged,
           this, &KelGuiButton::textChanged);
  connect (ui->kelGuiLineEditMain, &KelGuiLineEdit::textEdited,
           this, &KelGuiButton::textEdited);
  connect (ui->kelGuiLineEditMain, &KelGuiLineEdit::textEdited,
           this, &KelGuiButton::_slotTextEdited);
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

  auto array = m_widgets.values ("textMain") + m_widgets.values ("textMid") + m_widgets.values ("textEdit");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    //as<KelGuiLabel> (*i)->setText (m_mainText);
    as<QObject> (*i)->setProperty("text", m_mainText);
}

QString KelGuiButton::subText() const
{
  return m_subText;
}

void KelGuiButton::setSubText (const QString &subText)
{
  m_subText = subText;

  auto array = m_widgets.values ("textSub") + m_widgets.values ("textRight");
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

      /* for swapped top and bottom labels */
      if (j == ui->KelGuiButtonTextMain_2)
        j->setAlignment (Qt::AlignHCenter | Qt::AlignTop);
      else if (j == ui->KelGuiButtonTextSub_2)
        j->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);

      /* for usual (top main, bottom sub) */
      else
        j->setAlignment (
          s_mainHelper (
            btnStyle() == LeftTopMainBottomSub
            || btnStyle() == IconMainSub, !m_subText.isEmpty()));
    }
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

QString KelGuiButton::iconCssClass() const
{
  return m_iconCssClass;
}

void KelGuiButton::setIconCssClass (const QString &iconCssClass)
{
  m_iconCssClass = iconCssClass;
  setupLabels();
}

QString KelGuiButton::inputMask() const
{
  return m_inputMask;
}

void KelGuiButton::setInputMask(const QString &inputMask)
{
  m_inputMask = inputMask;
  ui->kelGuiLineEditMain->setInputMask(inputMask);
}

KelGuiLineEdit *KelGuiButton::edit() const
{
  return ui->kelGuiLineEditMain;
}

KelGuiLineEdit::cbTextEdit KelGuiButton::inputCallback() const
{
  return ui->kelGuiLineEditMain->callbackTextEdit();
}

void KelGuiButton::setInputCallback(const KelGuiLineEdit::cbTextEdit &cb)
{
  ui->kelGuiLineEditMain->setCallbackTextEdit (cb);
}

KelGuiLineEdit::cbFocusEvent KelGuiButton::inputFocusCallback() const
{
  return ui->kelGuiLineEditMain->callbackFocusEvent();
}

void KelGuiButton::setInputFocusCallback(const KelGuiLineEdit::cbFocusEvent &cb)
{
  ui->kelGuiLineEditMain->setCallbackFocusEvent (cb);
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

int KelGuiButton::maxLength() const
{
  return ui->kelGuiLineEditMain->maxLength();
}

void KelGuiButton::setMaxLength(const int &max)
{
  ui->kelGuiLineEditMain->setMaxLength (max);
}

void KelGuiButton::setupStyle()
{
  //ui->KelGuiButtonBackground->setFrame (m_frame);
  ui->KelGuiButtonBackground->setCssStyle (m_frame ? "frame" : "");
  //ui->KelGuiButtonBackground->setStyleSheet (ui->KelGuiButtonBackground->styleSheet());
  //ui->KelGuiButtonBackground->repaint();

  m_shadowEffect->setEnabled (m_frame);
  setBtnStyle (m_btnStyle);
  setStyleSheet (styleSheet());
  repaint();
}

void KelGuiButton::setupLabels()
{
  QMap <QString, QString> labelMap =
  {
    {"textMain",  mainCssClass()  + " cwb_top"},
    {"textMid",   mainCssClass()  + " cwb_mid"},
    {"textEdit",  mainCssClass()  + " cwb_edit"},
    {"textSub",   subCssClass()   + " cwb_bottom"},
    {"textRight", subCssClass()   + " cwb_right"},
    {"textLeft",  leftCssClass()  + " cwb_left"},
    {"iconLeft",  iconCssClass()},
  };

  for (auto i = m_widgets.begin(), e = m_widgets.end(); i != e; i++)
    {
      as<QObject> (*i)->setProperty("cssStyle", mainCssClass() + " cwb_bottom");
      if (labelMap.contains (i.key()))
        {
          /* for swapped top and bottom labels */
          if (*i == ui->KelGuiButtonTextMain_2)
            /*as<KelGuiLabel> (*i)->setCssStyle (*/ as<QObject> (*i)->setProperty("cssStyle", mainCssClass() + " cwb_bottom");
          else if (*i == ui->KelGuiButtonTextSub_2)
            /*as<KelGuiLabel> (*i)->setCssStyle (*/ as<QObject> (*i)->setProperty("cssStyle", subCssClass() + " cwb_top");

          /* for usual (top main, bottom sub) */
          else
            /*as<KelGuiLabel> (*i)->setCssStyle (*/ as<QObject> (*i)->setProperty("cssStyle", labelMap.value (i.key()));
        }
    }
}

/********************************************
 * OVERRIDE
 *******************************************/

//void KelGuiButton::paintEvent (QPaintEvent *)
//{
//  QStyleOption opt;
//  opt.init (this);

//  QPainter p (this);
//  QWidget::style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
//}

void KelGuiButton::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();
  _slotDebugInfo();
}

void KelGuiButton::resizeEvent (QResizeEvent *ev)
{
  s_linkWidth = UiScaling::pointsToPixels (LINK_WIDTH, UiScaling::getNativDPI());
  auto sz     = ev->size();
  QPoint pos{
    sz.width() - s_linkWidth - s_linkWidth/4,
    (sz.height() - s_linkWidth) / 2
  };
  m_lLink->move (pos);
  m_lLink->resize (s_linkWidth, s_linkWidth);
}

void KelGuiButton::_slotDebugInfo()
{
#ifdef QT_DEBUG
  if(qApp->keyboardModifiers() & Qt::AltModifier)
    {
      QJsonObject jobj;
      jobj["mainText"]      = m_mainText;
      jobj["subText"]       = m_subText;
      jobj["leftText"]      = m_leftText;
      //jobj["mainCssClass"]  = m_mainCssClass;
      //jobj["subCssClass;"]  = m_subCssClass;
      //jobj["leftCssClass"]  = m_leftCssClass;
      //jobj["iconCssClass"]  = m_iconCssClass;
      jobj["inputMask"]     = m_inputMask;
      jobj["btnStyle"]      = m_btnStyle;
      jobj["cssStyle"]      = QJsonObject{
        {"name", cssStyle()},
        {"text", styleSheet()},
      };
      jobj["mainCssStyle"]      = QJsonObject{
        {"name", ui->KelGuiButtonTextMain->cssStyle()},
        {"text", ui->KelGuiButtonTextMain->styleSheet()},
      };
      jobj["subCssStyle"]      = QJsonObject{
        {"name", ui->KelGuiButtonTextSub->cssStyle()},
        {"text", ui->KelGuiButtonTextSub->styleSheet()},
      };
      jobj["leftCssStyle"]      = QJsonObject{
        {"name", ui->KelGuiButtonTextLeft->cssStyle()},
        {"text", ui->KelGuiButtonTextLeft->styleSheet()},
      };
      jobj["iconCssStyle"]      = QJsonObject{
        {"name", ui->KelGuiButtonIcon->cssStyle()},
        {"text", ui->KelGuiButtonIcon->styleSheet()},
      };
//      jobj["inputCssStyle"]      = QJsonObject{
//        {"name", ui->kelGuiLineEditMain->cssStyle()},
//        {"text", ui->kelGuiLineEditMain->styleSheet()},
//      };
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

void KelGuiButton::_slotTextEdited(const QString &a_text)
{
  m_mainText = a_text;
}

/*-----------------------------------------*/

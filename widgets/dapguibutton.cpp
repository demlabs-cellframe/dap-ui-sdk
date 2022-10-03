/* INCLUDES */
#include "dapguibutton.h"
#include "ui_dapguibutton.h"
#include "dapguicommon.h"

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

#include "../forms/ui/helper/auxiliary/UiScaling.h"

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

DapGuiButton::DapGuiButton (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::DapGuiButtonUI)
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
  , m_textChangesSignalLock(false)
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
  m_widgets.insert ("styledWidgets", ui->Style5);
  m_widgets.insert ("styledWidgets", ui->Style6);
  m_widgets.insert ("textMain", ui->DapGuiButtonTextMain);
  m_widgets.insert ("textMain", ui->DapGuiButtonTextMain_2);
  m_widgets.insert ("textMain", ui->DapGuiButtonTextMain_3);
  m_widgets.insert ("textMid", ui->DapGuiButtonTextMid);
  m_widgets.insert ("textMid", ui->DapGuiButtonTextMid_2);
  m_widgets.insert ("textEdit", ui->kelGuiLineEditMain);
  m_widgets.insert ("textEdit", ui->kelGuiLineEditMainNative);
  m_widgets.insert ("textSub", ui->DapGuiButtonTextSub);
  m_widgets.insert ("textSub", ui->DapGuiButtonTextSub_2);
  m_widgets.insert ("textSub", ui->DapGuiButtonTextSub_3);
  m_widgets.insert ("textSub", ui->DapGuiButtonTextSub_4);
  m_widgets.insert ("textRight", ui->DapGuiButtonTextRight);
  m_widgets.insert ("textRight", ui->DapGuiButtonTextRight_2);
  m_widgets.insert ("marginOnLink", ui->DapGuiButtonTextRight);
  m_widgets.insert ("textLeft", ui->DapGuiButtonTextLeft);
  m_widgets.insert ("iconLeft", ui->DapGuiButtonIcon);
  m_widgets.insert ("iconLeft", ui->DapGuiButtonIconLeft);
  m_widgets.insert ("iconLeft", ui->DapGuiButtonIcon_61);
  m_widgets.insert ("iconRight", ui->DapGuiButtonIconRight);

  /* prepare link icon */
//  m_lLink->setPixmap (QPixmap ("://gfx/ic_arrow-right.png"));
//  m_lLink->setMinimumWidth (s_linkWidth);
//  m_lLink->setMaximumWidth (s_linkWidth);
  m_lLink->setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
  m_lLink->setParent (ui->DapGuiButtonBackground);
  m_lLink->hide();

  //setIcon (m_icon);
  setupStyle();
  setSeparator (false);

  /* connect clicked signals */
  for (auto i = m_widgets.begin(), e = m_widgets.end(); i != e; i++)
    {
      auto *label  = qobject_cast<DapGuiLabel*>(*i);

      if(!label)
        continue;

      if (i.key() == "marginOnLink")
        continue;
      if (i.key() == "iconRight")
        continue;

      connect (label, &DapGuiLabel::clicked,
               this, &DapGuiButton::clicked);
#ifdef QT_DEBUG
      connect (label, &DapGuiLabel::clicked,
               this, &DapGuiButton::_slotDebugInfo);
#endif // QT_DEBUG
    }

  /* temporary arrays */
  _setMainTextWidgets           = m_widgets.values ("textMain") + m_widgets.values ("textMid") + m_widgets.values ("textEdit");
  _setSubTextWidgetsSubRight    = m_widgets.values ("textSub") + m_widgets.values ("textRight");
  _setSubTextWidgetsMain        = m_widgets.values ("textMain");

  /* signals */
  connect (ui->kelGuiLineEditMain, &DapGuiLineEdit::textChanged, [this](const QString& text)
  {
      if (!m_textChangesSignalLock)
        emit DapGuiButton::textChanged(text);
  });
  connect (ui->kelGuiLineEditMain, &DapGuiLineEdit::textEdited,
           this, &DapGuiButton::textEdited);
  connect (ui->kelGuiLineEditMain, &DapGuiLineEdit::textEdited,
           this, &DapGuiButton::_slotTextEdited);


  connect (ui->kelGuiLineEditMainNative, &DapGuiLineEditNative::textChanged, [this](const QString& text)
  {
      if (!m_textChangesSignalLock)
        emit DapGuiButton::textChanged(text);
  });
  connect (ui->kelGuiLineEditMainNative, &DapGuiLineEditNative::textEdited,
           this, &DapGuiButton::textEdited);
  connect (ui->kelGuiLineEditMainNative, &DapGuiLineEditNative::textEdited,
           this, &DapGuiButton::_slotTextEdited);


  connect (&__kgsm, &DapGuiStyleManager::forceStyleUpdate,
           this, &DapGuiButton::_slotStyleUpdate);
  connect (ui->DapGuiButtonIconRight, &DapGuiLabel::clicked, [&](){
    emit rightIconClicked(this->mainText());
  });

  /* update style staff */
  _slotStyleUpdate();
}

DapGuiButton::~DapGuiButton()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/



/****************************************//**
 * @property DapGuiButton::buttonStyle
 * @brief button's current style.
 *
 * All styles is present inside
 * file **dapguibutton.ui**
 *
 * @accessors %btnStyle(), %setBtnStyle()
 * @see ButtonStyle
 *******************************************/

DapGuiButton::ButtonStyle DapGuiButton::btnStyle() const
{
  return m_btnStyle;
}

void DapGuiButton::setBtnStyle (const ButtonStyle &style)
{
  m_btnStyle = style;
  setupLabels();

  int i = 0;
  auto array = m_widgets.values ("styledWidgets");
  std::reverse (array.begin(), array.end());
  for (auto j = array.begin(), e = array.end(); j != e; i++, j++)
    (*j)->setVisible (i == int (m_btnStyle));
//  ui->kelGuiLineEditMain->setVisible(tmp_style == ButtonStyle::EditTopMainBottomSub);
//  ui->kelGuiLineEditMainPrivate->setVisible(tmp_style == ButtonStyle::EditTopMainBottomSubPrivate);
}

/****************************************//**
 * @property DapGuiButton::mainText
 * @brief main label text
 * @accessors %mainText(), %setMainText()
 *******************************************/

QString DapGuiButton::mainText() const
{
  return m_mainText;
}

void DapGuiButton::setMainText (const QString &mainText)
{
  m_mainText = mainText;

  for (auto i = _setMainTextWidgets.begin(), e = _setMainTextWidgets.end(); i != e; i++)
    (*i)->setProperty("text", m_mainText);
}

/****************************************//**
 * @property DapGuiButton::subText
 * @brief second label text
 * @accessors %subText(), %setSubText()
 *******************************************/

QString DapGuiButton::subText() const
{
  return m_subText;
}

void DapGuiButton::setSubText (const QString &subText)
{
  m_subText = subText;

  for (auto i = _setSubTextWidgetsSubRight.begin(), e = _setSubTextWidgetsSubRight.end(); i != e; i++)
    {
      auto j = as<DapGuiLabel> (*i);
      j->setText (m_subText);
      j->setVisible (!m_subText.isEmpty());
    }

  for (auto i = _setSubTextWidgetsMain.begin(), e = _setSubTextWidgetsMain.end(); i != e; i++)
    {
      auto j = as<DapGuiLabel> (*i);

      /* for swapped top and bottom labels */
      if (j == ui->DapGuiButtonTextMain_2)
        j->setAlignment (Qt::AlignHCenter | Qt::AlignTop);
      else if (j == ui->DapGuiButtonTextSub_2)
        j->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);

      /* for usual (top main, bottom sub) */
      else
        j->setAlignment (
          s_mainHelper (
            btnStyle() == LeftTopMainBottomSub
            || btnStyle() == IconMainSub, !m_subText.isEmpty()));
    }
}

/****************************************//**
 * @property DapGuiButton::leftText
 * @brief left label text
 * @accessors %leftText(), %setLeftText()
 *******************************************/

QString DapGuiButton::leftText() const
{
  return m_leftText;
}

void DapGuiButton::setLeftText (const QString &leftText)
{
  m_leftText = leftText;

  auto array = m_widgets.values ("textLeft");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    as<DapGuiLabel> (*i)->setText (m_leftText);
}

/****************************************//**
 * @property DapGuiButton::mainCssClass
 * @brief css list for main label
 * @see DapGuiStyleManager
 * @accessors %mainCssClass(), %setMainCssClass()
 *******************************************/

QString DapGuiButton::mainCssClass() const
{
  return m_mainCssClass;
}

void DapGuiButton::setMainCssClass (const QString &mainCssClass)
{
  m_mainCssClass = mainCssClass;
  setupLabels();
}

/****************************************//**
 * @property DapGuiButton::subCssClass
 * @brief css list for second label
 * @see DapGuiStyleManager
 * @accessors %subCssClass(), %setSubCssClass()
 *******************************************/

QString DapGuiButton::subCssClass() const
{
  return m_subCssClass;
}

void DapGuiButton::setSubCssClass (const QString &subCssClass)
{
  m_subCssClass = subCssClass;
  setupLabels();
}

/****************************************//**
 * @property DapGuiButton::leftCssClass
 * @brief css list for left label
 * @see DapGuiStyleManager
 * @accessors %leftCssClass(), %setLeftCssClass()
 *******************************************/

QString DapGuiButton::leftCssClass() const
{
  return m_leftCssClass;
}

void DapGuiButton::setLeftCssClass (const QString &leftCssClass)
{
  m_leftCssClass = leftCssClass;
  setupLabels();
}

/****************************************//**
 * @property DapGuiButton::iconCssClass
 * @brief css list for icon label
 * @see DapGuiStyleManager
 * @accessors %iconCssClass(), %setIconCssClass()
 *******************************************/

QString DapGuiButton::iconCssClass() const
{
  return m_iconCssClass;
}

void DapGuiButton::setIconCssClass (const QString &iconCssClass)
{
  m_iconCssClass = iconCssClass;
  setupLabels();
}

/****************************************//**
 * @property DapGuiButton::iconRightCssClass
 * @brief css list for right icon label
 * @see DapGuiStyleManager
 * @accessors %iconRightCssClass(), %setIconRightCssClass()
 *******************************************/

QString DapGuiButton::iconRightCssClass() const
{
  return m_iconRightCssClass;
}

void DapGuiButton::setIconRightCssClass(const QString &iconCssClass)
{
  m_iconRightCssClass = iconCssClass;
  setupLabels();
}

/****************************************//**
 * @property DapGuiButton::inputMask
 * @brief regexp for input field
 * @see <a href="https://doc.qt.io/qt-5/qlineedit.html#inputMask-prop" target="_blank">QLineEdit::inputMask</a>
 * @accessors %inputMask(), %setInputMask()
 *******************************************/

QString DapGuiButton::inputMask() const
{
  return m_inputMask;
}

void DapGuiButton::setInputMask(const QString &inputMask)
{
  m_inputMask = inputMask;
  ui->kelGuiLineEditMain->setInputMask(inputMask);
}

/****************************************//**
 * @brief custom widget inside edit field
 *
 * By default, property is nullptr
 *******************************************/

DapGuiLineEdit *DapGuiButton::edit() const
{
    return ui->kelGuiLineEditMain;
}

DapGuiLineEditNative *DapGuiButton::editNative() const
{
    return ui->kelGuiLineEditMainNative;
}

/****************************************//**
 * @brief setup custom widget into edit field
 * @warning this is permanent. ask Mikhail Shilenko for more info
 *******************************************/

void DapGuiButton::setEdit(QWidget *newEdit) const
{
  /* take items from style 4 */
  auto lay      = ui->Style4->layout();
  auto subText  = lay->takeAt (1);
  auto editText = lay->takeAt (0);

  /* insert items */
  lay->addWidget (newEdit);
  lay->addItem (editText);
  lay->addItem (subText);

  ui->kelGuiLineEditMain->hide();
}

/****************************************//**
 * @see DapGuiLineEdit::callbackKeyEvent
 *******************************************/

DapGuiLineEdit::cbKeyEvent DapGuiButton::callbackKeyEvent() const
{
    return ui->kelGuiLineEditMain->callbackKeyEvent();
}

void DapGuiButton::setCallbackKeyEvent(DapGuiLineEdit::cbKeyEvent cb)
{
    ui->kelGuiLineEditMain->setCallbackKeyEvent(cb);
}

/****************************************//**
 * @see DapGuiLineEdit::callbackTextEdit
 *******************************************/

DapGuiLineEdit::cbTextEdit DapGuiButton::inputCallback() const
{
  return ui->kelGuiLineEditMain->callbackTextEdit();
}

/****************************************//**
 * @see DapGuiLineEdit::setCallbackTextEdit
 *******************************************/

void DapGuiButton::setInputCallback(const DapGuiLineEdit::cbTextEdit &cb)
{
  ui->kelGuiLineEditMain->setCallbackTextEdit (cb);
}

/****************************************//**
 * @see DapGuiLineEdit::callbackFocusEvent
 *******************************************/

DapGuiLineEdit::cbFocusEvent DapGuiButton::inputFocusCallback() const
{
  return ui->kelGuiLineEditMain->callbackFocusEvent();
}

/****************************************//**
 * @see DapGuiLineEdit::setCallbackFocusEvent
 *******************************************/

void DapGuiButton::setInputFocusCallback(const DapGuiLineEdit::cbFocusEvent &cb)
{
  ui->kelGuiLineEditMain->setCallbackFocusEvent (cb);
}

/****************************************//**
 * @property DapGuiButton::link
 * @brief link icon
 *
 * Draw small link icon at the right side of the button
 *
 * @accessors %link(), %setLink()
 *******************************************/

bool DapGuiButton::link() const
{
  return m_link;
}

void DapGuiButton::setLink (bool link)
{
  m_link = link;
  m_lLink->setVisible (link);

  auto array = m_widgets.values ("marginOnLink");
  for (auto i = array.begin(), e = array.end(); i != e; i++)
    {
      auto j  = as<DapGuiLabel> (*i);
      auto js = j->cssStyle();
      j->setCssStyle (js + " margin-right28");
    }
}

/****************************************//**
 * @property DapGuiButton::frame
 * @brief show frame
 *
 * Draw frame around button
 *
 * @accessors %frame(), %setFrame()
 *******************************************/

bool DapGuiButton::frame() const
{
  return m_frame;
}

void DapGuiButton::setFrame (bool frame)
{
  m_frame = frame;
  setupStyle();
}

/****************************************//**
 * @property DapGuiButton::separator
 * @brief show separator
 *
 * Draw separator at the bottom of the button
 *
 * @accessors %separator(), %setSeparator()
 *******************************************/

bool DapGuiButton::separator() const
{
  return m_separator;
}

void DapGuiButton::setSeparator (bool separator)
{
  m_separator = separator;
  ui->kelGuiSeparator->setVisible (m_separator);
}

/****************************************//**
 * @property DapGuiButton::maxLength
 * @brief edit field's max length
 *
 * Set text max length limit to edit field
 *
 * @accessors %maxLength(), %setMaxLength()
 *******************************************/

int DapGuiButton::maxLength() const
{
  return ui->kelGuiLineEditMain->maxLength();
}

void DapGuiButton::setMaxLength(const int &max)
{
  ui->kelGuiLineEditMain->setMaxLength (max);
}

void DapGuiButton::setMainAlignment(const Qt::Alignment &a_align)
{
  for (auto i = _setMainTextWidgets.begin(), e = _setMainTextWidgets.end(); i != e; i++)
    (*i)->setProperty("alignment", QVariant::fromValue (a_align));
}

void DapGuiButton::setSubAlignment(const Qt::Alignment &a_align)
{
  for (auto i = _setSubTextWidgetsSubRight.begin(), e = _setSubTextWidgetsSubRight.end(); i != e; i++)
    (*i)->setProperty("alignment", QVariant::fromValue (a_align));
}

void DapGuiButton::setupStyle()
{
  //ui->DapGuiButtonBackground->setFrame (m_frame);
  ui->DapGuiButtonBackground->setCssStyle (m_frame ? "frame" : "");

  //m_shadowEffect->setEnabled (m_frame);
  setBtnStyle (m_btnStyle);
  setStyleSheet (styleSheet());
  repaint();
}

void DapGuiButton::setupLabels()
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
    {"iconRight", iconRightCssClass()},
  };

  for (auto i = m_widgets.begin(), e = m_widgets.end(); i != e; i++)
    {
      (*i)->setProperty("cssStyle", mainCssClass() + " cwb_bottom");
      if (labelMap.contains (i.key()))
        {
          /* for swapped top and bottom labels */
          if (*i == ui->DapGuiButtonTextMain_2)
            (*i)->setProperty("cssStyle", mainCssClass() + " cwb_bottom");
          else if (*i == ui->DapGuiButtonTextSub_2)
            (*i)->setProperty("cssStyle", subCssClass() + " cwb_top");

          /* for usual (top main, bottom sub) */
          else
            (*i)->setProperty("cssStyle", labelMap.value (i.key()));
        }
    }
}

void DapGuiButton::setPlaceholderText(const QString &text)
{
    ui->kelGuiLineEditMain->setPlaceholderText(text);
}

void DapGuiButton::insert(const QString &text)
{
    ui->kelGuiLineEditMain->clear();
    ui->kelGuiLineEditMain->insert(text);
}

/********************************************
 * OVERRIDE
 *******************************************/

//void DapGuiButton::paintEvent (QPaintEvent *)
//{
//  QStyleOption opt;
//  opt.init (this);

//  QPainter p (this);
//  QWidget::style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
//}

void DapGuiButton::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();
  _slotDebugInfo();
}

void DapGuiButton::resizeEvent (QResizeEvent *ev)
{
  s_linkWidth = UiScaling::pointsToPixels (LINK_WIDTH, UiScaling::getNativDPI());
  auto sz     = ev->size();
  QPoint pos{
    sz.width() - int(s_linkWidth * 1.5 * (1 + m_frame)),
    (sz.height() - s_linkWidth) / 2
  };
  m_lLink->move (pos);
  m_lLink->resize (s_linkWidth, s_linkWidth);
}

void DapGuiButton::textChangedSignalLock(bool lock)
{
    m_textChangesSignalLock = lock;
}

void DapGuiButton::_slotDebugInfo()
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
        {"name", ui->DapGuiButtonTextMain->cssStyle()},
        {"text", ui->DapGuiButtonTextMain->styleSheet()},
      };
      jobj["subCssStyle"]      = QJsonObject{
        {"name", ui->DapGuiButtonTextSub->cssStyle()},
        {"text", ui->DapGuiButtonTextSub->styleSheet()},
      };
      jobj["leftCssStyle"]      = QJsonObject{
        {"name", ui->DapGuiButtonTextLeft->cssStyle()},
        {"text", ui->DapGuiButtonTextLeft->styleSheet()},
      };
      jobj["iconCssStyle"]      = QJsonObject{
        {"name", ui->DapGuiButtonIcon->cssStyle()},
        {"text", ui->DapGuiButtonIcon->styleSheet()},
      };
      jobj["iconLeftCssStyle"]      = QJsonObject{
        {"name", ui->DapGuiButtonIconLeft->cssStyle()},
        {"text", ui->DapGuiButtonIconLeft->styleSheet()},
      };
      jobj["iconRightCssStyle"]      = QJsonObject{
        {"name", ui->DapGuiButtonIconRight->cssStyle()},
        {"text", ui->DapGuiButtonIconRight->styleSheet()},
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

void DapGuiButton::_slotTextEdited(const QString &a_text)
{
  m_mainText = a_text;
}

void DapGuiButton::_slotStyleUpdate()
{
  m_lLink->setStyleSheet (
    QString(
      "background-color: rgba(0,0,0,0);"
      "border-image: url(\"://%1/ic_arrow-right.png\");"
    ).arg (__kgsm.themeDir()));
}

/*-----------------------------------------*/

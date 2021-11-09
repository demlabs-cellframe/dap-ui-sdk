/* INCLUDES */
#include "dapguiplaintextedit.h"
#include "ui_dapguiplaintextedit.h"
#include <QStyle>
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiPlainTextEdit::DapGuiPlainTextEdit (QWidget *parent) :
    QWidget (parent),
    ui (new Ui::DapGuiPlainTextEditUI),
    m_callbackTextEdit (nullptr)
{
  ui->setupUi(this);

  /* callbacks */
  ui->kgpteuPlainTextEdit->setCallbackFocusEvent (_cbFocusEvent);
  ui->kgpteuPlainTextEdit->setCallbackUnfocusEvent (_cbUnfocusEvent);
  ui->kgpteuPlainTextEdit->setCallbackKeyEvent (_cbKeyEvent);
  ui->kgpteuPlainTextEdit->setCallbackTextEdit (_cbTextEdit);
  ui->kgpteuPlainTextEdit->installEventFilter (this);

  /* signals */
  connect(ui->kgpteuPlainTextEdit, &QPlainTextEdit::textChanged,
          [=] () { _updateStyle(); });
}

DapGuiPlainTextEdit::~DapGuiPlainTextEdit()
{
  delete ui;
}

/********************************************
 * PUBLIC METHODS
 *******************************************/

Qt::ScrollBarPolicy DapGuiPlainTextEdit::verticalScrollBarPolicy() const
{
  return ui->kgpteuPlainTextEdit->verticalScrollBarPolicy();
}

void DapGuiPlainTextEdit::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setVerticalScrollBarPolicy (a_policy);
}

Qt::ScrollBarPolicy DapGuiPlainTextEdit::horizontalScrollBarPolicy() const
{
  return ui->kgpteuPlainTextEdit->horizontalScrollBarPolicy();
}

void DapGuiPlainTextEdit::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setHorizontalScrollBarPolicy (a_policy);
}

QAbstractScrollArea::SizeAdjustPolicy DapGuiPlainTextEdit::sizeAdjustPolicy() const
{
  return ui->kgpteuPlainTextEdit->sizeAdjustPolicy();
}

void DapGuiPlainTextEdit::setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setSizeAdjustPolicy (a_policy);
}

QString DapGuiPlainTextEdit::placeholderText() const
{
  return ui->kgpteuPlainTextEdit->placeholderText();
}

void DapGuiPlainTextEdit::setPlaceholderText(const QString &placeholderText)
{
  ui->kgpteuPlainTextEdit->setPlaceholderText (placeholderText);
}

QString DapGuiPlainTextEdit::plainText() const
{
  return ui->kgpteuPlainTextEdit->toPlainText();
}

void DapGuiPlainTextEdit::setPlainText(const QString &plainText)
{
  ui->kgpteuPlainTextEdit->setPlainText(plainText);
}

void DapGuiPlainTextEdit::setTextCursor(const QTextCursor &a_cursor)
{
  ui->kgpteuPlainTextEdit->setTextCursor (a_cursor);
}

QTextCursor DapGuiPlainTextEdit::textCursor() const
{
  return ui->kgpteuPlainTextEdit->textCursor();
}

QPlainTextEdit *DapGuiPlainTextEdit::editWidget() const
{
  return ui->kgpteuPlainTextEdit;
}

void DapGuiPlainTextEdit::unfocus()
{
  ui->kgpteuPlainTextEdit->unfocus();
}

DapGuiPlainTextEdit::cbTextEdit DapGuiPlainTextEdit::callbackTextEdit() const
{
  return m_callbackTextEdit;
}

void DapGuiPlainTextEdit::setCallbackTextEdit(cbTextEdit newCallbackTextEdit)
{
  m_callbackTextEdit = newCallbackTextEdit;
}

/********************************************
 * PROTECTED METHODS
 *******************************************/

static const char * const s_styles[2] =
{
  "QPlainTextEdit\n"
  "{\n"
  "color: #04004E;\n"
  "background-color: #F7F8FA;\n"
  "font: 16px/25px;\n"
  "font-weight: normal;\n"
  "font-style: normal;\n"
  "font-size: normal;\n"
  "font-family: 'Lato';\n"
  "}\n"
  ,
  "QPlainTextEdit\n"
  "{\n"
  "color: #9C9B9D;\n"
  "}\n"
};

bool DapGuiPlainTextEdit::_cbTextEdit(DapGuiPlainTextEditInterface *e, QString &preedit, QString &commit, int from, int to)
{
  auto p  = qobject_cast<DapGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    {
      p->ui->kgpteuPlainTextEdit->setStyleSheet (
          (!commit.isEmpty() || !preedit.isEmpty() || !e->toPlainText().isEmpty())
          ? QString() + s_styles[0]
          : QString() + s_styles[0] + s_styles[1]
        );
      p->_updateStyle();
    }

  if (p->m_callbackTextEdit)
    return p->m_callbackTextEdit (e, preedit, commit, from, to);

  return false;
}

void DapGuiPlainTextEdit::_cbFocusEvent(DapGuiPlainTextEditInterface *e, const Qt::FocusReason &reason)
{
  Q_UNUSED(e)
  Q_UNUSED(reason)
  auto p  = qobject_cast<DapGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    p->_updateStyle();
}

void DapGuiPlainTextEdit::_cbUnfocusEvent(DapGuiPlainTextEditInterface *e, const Qt::FocusReason &reason)
{
  Q_UNUSED(e)
  Q_UNUSED(reason)
  auto p  = qobject_cast<DapGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    p->_updateStyle();
}

bool DapGuiPlainTextEdit::_cbKeyEvent(DapGuiPlainTextEditInterface *e, QKeyEvent *event)
{
  Q_UNUSED(e)
  Q_UNUSED(event)
  auto p  = qobject_cast<DapGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    p->_updateStyle();
  return false;
}

bool DapGuiPlainTextEdit::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    qDebug() << __PRETTY_FUNCTION__
             << "key:" << keyEvent->key()
             << "text:" << keyEvent->text();
  }
  return QObject::eventFilter(obj, event);
}

void DapGuiPlainTextEdit::_updateStyle()
{
  style()->polish (ui->kgpteuPlainTextEdit);
  emit textChanged();
}

/*-----------------------------------------*/

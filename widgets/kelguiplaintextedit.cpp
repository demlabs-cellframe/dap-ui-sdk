/* INCLUDES */
#include "kelguiplaintextedit.h"
#include "ui_kelguiplaintextedit.h"
#include <QStyle>
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiPlainTextEdit::KelGuiPlainTextEdit (QWidget *parent) :
    QWidget (parent),
    ui(new Ui::KelGuiPlainTextEditUI)
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

KelGuiPlainTextEdit::~KelGuiPlainTextEdit()
{
  delete ui;
}

/********************************************
 * PUBLIC METHODS
 *******************************************/

Qt::ScrollBarPolicy KelGuiPlainTextEdit::verticalScrollBarPolicy() const
{
  return ui->kgpteuPlainTextEdit->verticalScrollBarPolicy();
}

void KelGuiPlainTextEdit::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setVerticalScrollBarPolicy (a_policy);
}

Qt::ScrollBarPolicy KelGuiPlainTextEdit::horizontalScrollBarPolicy() const
{
  return ui->kgpteuPlainTextEdit->horizontalScrollBarPolicy();
}

void KelGuiPlainTextEdit::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setHorizontalScrollBarPolicy (a_policy);
}

QAbstractScrollArea::SizeAdjustPolicy KelGuiPlainTextEdit::sizeAdjustPolicy() const
{
  return ui->kgpteuPlainTextEdit->sizeAdjustPolicy();
}

void KelGuiPlainTextEdit::setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy a_policy)
{
  ui->kgpteuPlainTextEdit->setSizeAdjustPolicy (a_policy);
}

QString KelGuiPlainTextEdit::placeholderText() const
{
  return ui->kgpteuPlainTextEdit->placeholderText();
}

void KelGuiPlainTextEdit::setPlaceholderText(const QString &placeholderText)
{
  ui->kgpteuPlainTextEdit->setPlaceholderText (placeholderText);
}

QString KelGuiPlainTextEdit::plainText() const
{
  return ui->kgpteuPlainTextEdit->toPlainText();
}

void KelGuiPlainTextEdit::setPlainText(const QString &plainText)
{
  ui->kgpteuPlainTextEdit->setPlainText(plainText);
}

void KelGuiPlainTextEdit::setTextCursor(const QTextCursor &a_cursor)
{
  ui->kgpteuPlainTextEdit->setTextCursor (a_cursor);
}

QTextCursor KelGuiPlainTextEdit::textCursor() const
{
  return ui->kgpteuPlainTextEdit->textCursor();
}

QPlainTextEdit *KelGuiPlainTextEdit::editWidget()
{
  return ui->kgpteuPlainTextEdit;
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

void KelGuiPlainTextEdit::_cbTextEdit(KelGuiPlainTextEditInterface *e, QString &preedit, QString &commit, int from, int to)
{
  Q_UNUSED(e)
  Q_UNUSED(preedit)
  Q_UNUSED(commit)
  Q_UNUSED(from)
  Q_UNUSED(to)
  auto p  = qobject_cast<KelGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    {
      p->ui->kgpteuPlainTextEdit->setStyleSheet (
          (!commit.isEmpty() || !preedit.isEmpty() || !e->toPlainText().isEmpty())
          ? QString() + s_styles[0]
          : QString() + s_styles[0] + s_styles[1]
        );
      p->_updateStyle();
    }
}

void KelGuiPlainTextEdit::_cbFocusEvent(KelGuiPlainTextEditInterface *e, const Qt::FocusReason &reason)
{
  Q_UNUSED(e)
  Q_UNUSED(reason)
  auto p  = qobject_cast<KelGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    p->_updateStyle();
}

void KelGuiPlainTextEdit::_cbUnfocusEvent(KelGuiPlainTextEditInterface *e, const Qt::FocusReason &reason)
{
  Q_UNUSED(e)
  Q_UNUSED(reason)
  auto p  = qobject_cast<KelGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    p->_updateStyle();
}

bool KelGuiPlainTextEdit::_cbKeyEvent(KelGuiPlainTextEditInterface *e, QKeyEvent *event)
{
  Q_UNUSED(e)
  Q_UNUSED(event)
  auto p  = qobject_cast<KelGuiPlainTextEdit*> (e->parentWidget());
  if (p)
    p->_updateStyle();
  return false;
}

bool KelGuiPlainTextEdit::eventFilter(QObject *obj, QEvent *event)
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

void KelGuiPlainTextEdit::_updateStyle()
{
  style()->polish (ui->kgpteuPlainTextEdit);
  emit textChanged();
}

/*-----------------------------------------*/

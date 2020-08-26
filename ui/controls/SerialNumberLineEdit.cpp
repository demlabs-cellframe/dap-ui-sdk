#include "SerialNumberLineEdit.h"

bool SerialFieldEdit::inFocus=false;

//public
SerialNumberLineEdit::SerialNumberLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    init();
}

void SerialNumberLineEdit::setPlaceholderText(const QString& text)
{
    m_placeholder->setText(text);
}

void SerialNumberLineEdit::setFont(const QFont &font)
{
    m_placeholder->setFont(font);
    for(auto& el:m_vecLabel)
        el->setFont(font);
    for(auto& el:m_vecLineEdit)
        el->setFont(font);
}

bool SerialNumberLineEdit::setSerial(const QString &serialNumber)
{
    QString temp{serialNumber};
    QString strRegExp{"["+QString(VALIDATOR)+"]"+
                "{"+QString::number(MAX_COUNT_CHAR*MAX_COUNT_PART)+"}"};
    QRegExp regExp(strRegExp);

    if (regExp.exactMatch(serialNumber))
    {
        for(auto el:m_vecLineEdit)
        {
            el->setText(temp.left(MAX_COUNT_CHAR));
            temp.remove(0,MAX_COUNT_CHAR);
        }
        return true;
    }

    else
    {
        qDebug()<<"SerialNumberLineEdit::setSerial() serialNumber is not valid"
               <<strRegExp;
        return false;
    }
}

void SerialNumberLineEdit::clear()
{
    m_serialNumber.clear();
    for(auto& el:m_vecLineEdit)
        el->clear();
    m_isFilledOut=false;
    paint_outFocusWithoutContent();
}

//private
void SerialNumberLineEdit::init()
{
    m_layout = new QGridLayout;

    m_placeholder=new QLabel;
    m_placeholder->setObjectName("placeholderLabel");
    m_placeholder->setLayoutDirection(Qt::LeftToRight);
    m_layout->addWidget(m_placeholder,1,0,1,1);

    m_layout->setContentsMargins(0, 0, 0, 0);
    setObjectName("serialNumberLineEdit");
    m_regExp=QRegExp("["+QString(VALIDATOR)+"]"
                     "{0,"+QString::number(MAX_COUNT_CHAR)+"}");

    m_vecLabel.reserve(MAX_COUNT_PART-1);
    m_vecLineEdit.reserve(MAX_COUNT_PART);

    m_labelPaste = new LabelPaste;
    m_layout->addWidget(m_labelPaste,0,1,1,2);
    connect(this,SIGNAL(visiblePaste(bool)),m_labelPaste,SLOT(setVisibility(bool)));
    setPlaceholderText();

    m_vecLineEdit.push_back(createLineEdit());
    m_layout->addWidget(m_vecLineEdit.back(),1,1,1,1);

    int j{1};
    for(int i=1;i<MAX_COUNT_PART;i++)
    {
        m_vecLabel.push_back(createDashLabel());
        m_layout->addWidget(m_vecLabel[i-1],1,i+j,1,1);
        m_vecLineEdit.push_back(createLineEdit());
        ++j;
        m_layout->addWidget(m_vecLineEdit[i],1,i+j,1,1);
    }

    setLayout(m_layout);
}

SerialFieldEdit *SerialNumberLineEdit::createLineEdit()
{
    static int i=1;
    SerialFieldEdit* serialFieldEdit=new SerialFieldEdit(this);
    serialFieldEdit->setObjectName("serialFieldEdit_"+QString::number(i));
    if(i==1) connect(m_labelPaste,SIGNAL(signal_clickPaste()),serialFieldEdit,SLOT(paste()));
    if (i>1) serialFieldEdit->setFocusPolicy(Qt::NoFocus);
    ++i;
    QRegExp azAZ09("[a-z"+QString(VALIDATOR)+"]"
                   "{0,"+QString::number(MAX_COUNT_CHAR)+"}");
    QValidator* validator=new QRegExpValidator(azAZ09);
    serialFieldEdit->setValidator(validator);
    serialFieldEdit->setMaxLength(MAX_COUNT_CHAR);
    serialFieldEdit->setVisible(false);
    serialFieldEdit->setInputMethodHints(Qt::ImhLatinOnly|Qt::ImhNoPredictiveText|Qt::ImhPreferUppercase|Qt::ImhUppercaseOnly);
    serialFieldEdit->setAlignment(Qt::AlignHCenter);
    connect(serialFieldEdit,SIGNAL(signal_outOfLineEdit()),SLOT(slot_checkOutOfLineEdit()));
    connect(serialFieldEdit,SIGNAL(signal_inLineEdit()),SLOT(slot_checkInLineEdit()));
    connect(serialFieldEdit,SIGNAL(textEdited(QString)),this,SLOT(slot_textEdited(QString)));
    connect(serialFieldEdit,SIGNAL(signal_lineEditIsEmpty()),SLOT(slot_changeFocusBackspace()));
    connect(serialFieldEdit,SIGNAL(textChanged(QString)),SLOT(slot_textChanged(QString)));
    connect(serialFieldEdit,SIGNAL(signal_pasteEvent(QString)),SLOT(slot_paste(QString)));
    return serialFieldEdit;
}

QLabel* SerialNumberLineEdit::createDashLabel()
{
    QLabel* temp=new QLabel(DASH);
    temp->setObjectName("dashLabel");
    temp->setAlignment(Qt::AlignCenter);
    temp->setVisible(false);
    return temp;
}

void SerialNumberLineEdit::paint_outFocusWithContent()
{
    for(auto& el:m_vecLineEdit)
    {
        Utils::setPropertyAndUpdateStyle(el,"inFocus",state::outFocus_withContent);
        el->setVisible(true);
    }
    for(auto& el:m_vecLabel)
    {
        Utils::setPropertyAndUpdateStyle(el,"inFocus",state::outFocus_withContent);
        el->setVisible(true);
    }
    m_placeholder->setVisible(false);
    Utils::setPropertyAndUpdateStyle(this,"inFocus",state::outFocus_withContent);
}

void SerialNumberLineEdit::paint_outFocusWithoutContent()
{
    m_placeholder->setVisible(true);
    for(auto& el:m_vecLineEdit)
        el->setVisible(false);
    for(auto& el:m_vecLabel)
        el->setVisible(false);
    Utils::setPropertyAndUpdateStyle(this,"inFocus",state::outFocus_withoutContent);
}

void SerialNumberLineEdit::paint_inFocus()
{
    m_placeholder->setVisible(false);
    for(auto& el:m_vecLineEdit)
    {
        el->setVisible(true);
        Utils::setPropertyAndUpdateStyle(el,"inFocus",state::inFocus);
    }
    for(auto& el:m_vecLabel)
        el->setVisible(true);
    Utils::setPropertyAndUpdateStyle(this,"inFocus",state::inFocus);
}


void SerialNumberLineEdit::slot_textEdited(QString text)
{
    emit serialEdited(text);
}

void SerialNumberLineEdit::slot_textChanged(QString text)
{
    QString name = qobject_cast<QLineEdit*>(sender())->objectName();
    int obj=QString(name[name.size()-1]).toInt();

    if(!m_regExp.exactMatch(m_vecLineEdit[obj-1]->text()))
    {
        m_vecLineEdit[obj-1]->setText(m_vecLineEdit[obj-1]->text().toUpper());
        return;
    }

    m_serialNumber.clear();
    m_isFilledOut=false;
    for(const auto& el:m_vecLineEdit)
        m_serialNumber.append(el->text());

    bool empty{};
    if(m_serialNumber.count()>0)
        emit visiblePaste(false);
    else
    {
        empty=true;
        emit visiblePaste(true);
    }
    for(auto& el:m_vecLabel)
    {
        Utils::setPropertyAndUpdateStyle(el,"empty",empty);
    }

    if (text.count()==MAX_COUNT_CHAR && obj!=MAX_COUNT_PART
            && m_serialNumber.count() != MAX_COUNT_CHAR*MAX_COUNT_PART)
    {
        m_vecLineEdit[obj]->setFocusPolicy(Qt::StrongFocus);
        m_vecLineEdit[obj]->setFocus();
    }

    if (m_serialNumber.count()==MAX_COUNT_CHAR * MAX_COUNT_PART)
    {
        qDebug()<<"filledOut\t"<<m_serialNumber;
        emit filledOut();
        m_isFilledOut=true;
        QFocusEvent event(QEvent::FocusOut);
        m_vecLineEdit[obj-1]->focusOutEvent(&event);
    }


    //сигнал выходит 4 раза при вставке Ctrl+V
    QTimer::singleShot(0,this,[this]()
    {
        emit serialChanged(m_serialNumber);
        //qDebug()<<__FUNCTION__<<m_serialNumber;
    });

}

void SerialNumberLineEdit::slot_checkOutOfLineEdit()
{
    QTimer::singleShot(0,this,[this]()
    {
        if(!SerialFieldEdit::inFocus)
        {
            bool empty{true};
            emit visiblePaste(false);
            for(const auto& el:m_vecLineEdit)
            {
                if (el->text().count()!=0)
                    empty=false;
            }
            if(empty)
            {
                paint_outFocusWithoutContent();    
            }
            else paint_outFocusWithContent();
        }
    });
}

void SerialNumberLineEdit::slot_checkInLineEdit()
{
    paint_inFocus();
}

void SerialNumberLineEdit::slot_changeFocusBackspace()
{
    QString name = qobject_cast<QLineEdit*>(sender())->objectName();
    int obj=QString(name[name.size()-1]).toInt();

    if(obj==1) return;

    m_vecLineEdit[obj-1]->setFocusPolicy(Qt::NoFocus);

    m_vecLineEdit[obj-2]->setFocus();
    QString text{m_vecLineEdit[obj-2]->text()};
    text.chop(1);
    m_vecLineEdit[obj-2]->setText(text);
}

void SerialNumberLineEdit::slot_paste(QString text)
{
    QString temp{text}, strRegExp{"["+QString(VALIDATOR)+"]"
                "{1,"+QString::number(MAX_COUNT_CHAR*MAX_COUNT_PART)+"}"};
    QRegExp regExp(strRegExp);
    if (regExp.exactMatch(text)&&text.count())
    {
        for (auto& el:m_vecLineEdit)
        {
            el->setText(temp.left(MAX_COUNT_CHAR));
            temp.remove(0,MAX_COUNT_CHAR);
        }
    }
}

//protected
void SerialNumberLineEdit::focusInEvent(QFocusEvent*)
{
    qDebug()<<__FUNCTION__;
    SerialFieldEdit::inFocus=true;
    paint_inFocus();
    m_vecLineEdit.front()->setFocus();
    if (m_serialNumber.count()==0) emit visiblePaste(true);
}



//********************SerialFieldEdit********************

void SerialFieldEdit::paste()
{
    QString text = QApplication::clipboard()->text().toUpper().replace("-","");
    qDebug()<<text;
    if(text.count()==MAX_COUNT_CHAR*MAX_COUNT_PART)
        pasteEvent();
    else QLineEdit::paste();
}

void SerialFieldEdit::focusInEvent(QFocusEvent *event)
{
    qDebug()<<__FUNCTION__;
    inFocus=true;

#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->show();
#endif
    QLineEdit::focusInEvent(event);
    if(event->reason() == Qt::MouseFocusReason)
        QTimer::singleShot(0, this, &QLineEdit::selectAll);
    emit signal_inLineEdit();
}

void SerialFieldEdit::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<__FUNCTION__;
    inFocus=false;
    QLineEdit::focusOutEvent(event);
#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->hide();
#endif
    emit signal_outOfLineEdit();
}

void SerialFieldEdit::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<__FUNCTION__;
    if (event->key()==Qt::Key_Backspace)
    {
        if (text().count()==0) emit signal_lineEditIsEmpty();
        else QLineEdit::keyPressEvent(event);
        return;
    }

    if(event==QKeySequence::Paste)
    {
        QString text = QApplication::clipboard()->text().toUpper().replace("-","");
        if (text.count()==MAX_COUNT_CHAR*MAX_COUNT_PART)
        {
            pasteEvent();
            return;
        }
    }
    QLineEdit::keyPressEvent(event);
}

void SerialFieldEdit::pasteEvent()
{
    QString text = QApplication::clipboard()->text().toUpper().replace("-","");
    emit signal_pasteEvent(text);
}

//********************LabelPaste********************

LabelPaste::LabelPaste(QWidget *parent)
    :QLabel(parent)
{
    setObjectName("labelPaste");
    m_op=new QGraphicsOpacityEffect;
    m_op->setOpacity(0);
    setGraphicsEffect(m_op);
    m_layout=new QVBoxLayout;
    m_labelPaste=new QLabel("Paste");
    m_layout->addWidget(m_labelPaste);
    m_labelPaste->setAlignment(Qt::AlignCenter);
    setLayout(m_layout);
}

void LabelPaste::setFont(const QFont &font)
{
    m_labelPaste->setFont(font);
}

void LabelPaste::setVisibility(bool vis)
{
    if(vis)
    {
        m_op->setOpacity(1);
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        m_op->setOpacity(0);
        unsetCursor();
    }
    setGraphicsEffect(m_op);
}

void LabelPaste::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        emit signal_clickPaste();
        return;
    }
    QLabel::mousePressEvent(ev);
}

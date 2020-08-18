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
    QString temp{};
    QString strRegExp{static_cast<QString>(VALIDATOR)+
                "{1,"+QString::number(MAX_COUNT_CHAR*MAX_COUNT_PART)+"}"};
    QRegExp regExp(strRegExp);
    if (regExp.exactMatch(serialNumber))
    {
        QQueue<QString>qq;
        for(const auto& el:serialNumber)
            qq.enqueue(el);
        for(int i=0;i<MAX_COUNT_PART;i++)
        {
            for(int j=0;j<MAX_COUNT_CHAR;j++)
            {
                if (!qq.isEmpty()) temp.append(qq.dequeue());
                else break;
            }
            m_vecLineEdit[i]->setText(temp);
            temp.clear();
        }
        paint_outFocusWithContent();
        emit serialChanged(serialNumber);
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
    m_regExp=QRegExp(QString(VALIDATOR)+
                     "{0,"+QString::number(MAX_COUNT_CHAR)+"}");
    qDebug()<<m_regExp;

    m_vecLabel.reserve(MAX_COUNT_PART-1);
    m_vecLineEdit.reserve(MAX_COUNT_PART);

    m_labelPaste = new LabelPaste;
    m_layout->addWidget(m_labelPaste,0,1,1,2);
    connect(this,SIGNAL(visiblePaste(bool)),m_labelPaste,SLOT(setVisibility(bool)));

    setPlaceholderText();

    m_vecLineEdit.push_back(createLineEdit());
    m_layout->addWidget(m_vecLineEdit[0],1,1,1,1);

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
    SerialFieldEdit* temp=new SerialFieldEdit;
    temp->setObjectName("serialFieldEdit_"+QString::number(i));
    if(i==1) connect(m_labelPaste,SIGNAL(signal_clickPaste()),temp,SLOT(paste()));
    ++i;
    QValidator* validator=new QRegExpValidator(m_regExp);
    temp->setValidator(validator);
    temp->setMaxLength(MAX_COUNT_CHAR);
    temp->setVisible(false);
    connect(temp,SIGNAL(signal_outOfLineEdit()),SLOT(slot_checkOutOfLineEdit()));
    connect(temp,SIGNAL(signal_inLineEdit()),SLOT(slot_checkInLineEdit()));
    connect(temp,SIGNAL(textEdited(QString)),this,SLOT(slot_textEdited(QString)));
    connect(temp,SIGNAL(signal_lineEditIsEmpty()),SLOT(slot_changeFocusBackspace()));
    connect(temp,SIGNAL(textChanged(QString)),SLOT(slot_textChanged(QString)));
    connect(temp,SIGNAL(signal_pasteEvent(QString)),SLOT(slot_paste(QString)));  
    return temp;
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

    /*нужно что-то придумать с сигналами,
    чтобы они не дублировались*/
    emit serialEdited(text);
    //qDebug()<<__FUNCTION__<<m_serialNumber;


    QString name = qobject_cast<QLineEdit*>(sender())->objectName();
    int obj=QString(name[name.size()-1]).toInt();

    if (text.count()==MAX_COUNT_CHAR && obj!=MAX_COUNT_PART)
    {
        m_vecLineEdit[obj]->setFocus();
    }
}

void SerialNumberLineEdit::slot_textChanged(QString)
{
    qDebug()<<__FUNCTION__;
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

    if (m_serialNumber.count()==MAX_COUNT_CHAR * MAX_COUNT_PART)
    {
        qDebug()<<"filledOut\t"<<m_serialNumber;
        emit filledOut();
        m_isFilledOut=true;
    }


    /*работает не так как хотел,
    нужно подумать, сигналы дублируются*/
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

    /*#ifdef Q_OS_ANDROID
        QApplication::inputMethod()->hide();
    #endif*/

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

    m_vecLineEdit[obj-2]->setFocus();
    //QTimer::singleShot(0,m_vecLineEdit[obj-2],&QLineEdit::deselect);
    QString text{m_vecLineEdit[obj-2]->text()};
    text.chop(1);
    m_vecLineEdit[obj-2]->setText(text);
}

void SerialNumberLineEdit::slot_paste(QString text)
{
    QString temp{text}, strRegExp{static_cast<QString>(VALIDATOR)+
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
    m_vecLineEdit.back()->setFocus();
    //QTimer::singleShot(0, m_vecLineEdit.back(), &QLineEdit::deselect);
}

//protected
void SerialNumberLineEdit::focusInEvent(QFocusEvent*)
{
    SerialFieldEdit::inFocus=true;
    paint_inFocus();
    m_vecLineEdit[0]->setFocus();
    if (m_serialNumber.count()==0) emit visiblePaste(true);
/*#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->show();
#endif*/
}
void SerialNumberLineEdit::focusOutEvent(QFocusEvent*)
{
    //empty
}


//********************SerialFieldEdit********************

void SerialFieldEdit::paste()
{
    QString text = QApplication::clipboard()->text();
    if(text.count()==MAX_COUNT_CHAR*MAX_COUNT_PART)
        pasteEvent();
    else QLineEdit::paste();
    //QTimer::singleShot(0, this, &QLineEdit::deselect);
}

void SerialFieldEdit::focusInEvent(QFocusEvent *event)
{
    qDebug()<<__FUNCTION__;
    inFocus=true;
#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->setVisible(true);
#endif
    QLineEdit::focusInEvent(event);
    //QTimer::singleShot(0, this, &QLineEdit::selectAll);
    emit signal_inLineEdit();


/*#ifdef Q_OS_ANDROID
    if(!QApplication::inputMethod()->isVisible())
        QApplication::inputMethod()->setVisible(true);
#endif*/
}
void SerialFieldEdit::focusOutEvent(QFocusEvent *event)
{
#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->setVisible(false);
#endif
    qDebug()<<__FUNCTION__;
    inFocus=false;
    QLineEdit::focusOutEvent(event);
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
        QString text = QApplication::clipboard()->text();
        if (text.count()==MAX_COUNT_CHAR*MAX_COUNT_PART)
        {
            pasteEvent();
            return;
        }
    }
    QLineEdit::keyPressEvent(event);
}

void SerialFieldEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    if (hasFocus())
    {
/*перекрасить каретку у QTextEdit получилось,
у QLineEdit принцип должен быть такой же,
но если не получится, придется использовать QTextEdit

(m_paint)?m_paint=false:m_paint=true;
const QRect qRect = cursorRect(textCursor());
QPainter qPainter(viewport());
if(m_paint) qPainter.fillRect(qRect, QColor("#00A86C"));
else qPainter.fillRect(qRect, QColor("white"));*/

        /*QSize sizeRect(-5,20);
        QRect qRect = cursorRect();
        qRect.setSize(sizeRect);
        qRect.setSize(sizeRect);
        qDebug()<<cursorRect()<<qRect;
        qDebug()<<cursorPosition();*/

        /*const QRect qRect(-3,1,4,15);
        QPainter p(this);
        p.fillRect(qRect, QColor("#00A86C"));*/

        //QTextLayout::drawCursor(qPainter,point,cursorPosition(),5);
    }
}

void SerialFieldEdit::pasteEvent()
{
    QString text = QApplication::clipboard()->text();
    emit signal_pasteEvent(text);
}

#ifdef Q_OS_ANDROID
void SerialFieldEdit::inputMethodEvent(QInputMethodEvent *event)
{
    if(event->preeditString().length()>0)
    {
        if (event->preeditString().length()==MAX_COUNT_CHAR)
        {
            qDebug()<<"FULL";
            QApplication::inputMethod()->commit();
            QTimer::singleShot(0, this, &QLineEdit::selectAll);
            return;
        }

        qDebug()<<"preeditString";
        QLineEdit::inputMethodEvent(event);
        QApplication::inputMethod()->commit();
        QApplication::inputMethod()->reset();
        return;
        /*QString strRegExp{static_cast<QString>(VALIDATOR)+
                    "{1,"+QString::number(MAX_COUNT_CHAR)+"}"};
        QRegExp regExp(strRegExp);
        if (regExp.exactMatch(event->preeditString()))
        {
            QLineEdit::inputMethodEvent(event);
            QApplication::inputMethod()->commit();
            QApplication::inputMethod()->reset();
        }
        else
        {
            qDebug()<<"IGNORE";
            QApplication::inputMethod()->reset();
        }
        qDebug()<<this->text();
        return;*/
    }
    else if (event->preeditString().length()==0)
    {
        qDebug()<<"EMPTY";
    }

    /*if(this->text().isEmpty())
    {
        qDebug()<<"EMPTY";
        //emit signal_lineEditIsEmpty();
        return;
    }*/
    QLineEdit::inputMethodEvent(event);
}
#endif

//********************LabelPaste********************

LabelPaste::LabelPaste(QWidget *parent)
    :QLabel(parent)
{
    setObjectName("obj_labelPaste");
    setPixmap(QPixmap(":/Popover.svg"));
    m_op=new QGraphicsOpacityEffect;
    m_op->setOpacity(0);
    setGraphicsEffect(m_op);
    m_layout=new QVBoxLayout;
    m_labelPaste=new QLabel("Paste");
    m_layout->addWidget(m_labelPaste);
    m_labelPaste->setAlignment(Qt::AlignCenter);
    m_layout->setContentsMargins(0,10,0,15);
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



//********************Secondary Functions********************

/*void updateCSS(QWidget *widget)
{
    QFile style(PATH_CSS);
    if (style.open(QFile::ReadOnly))
    {
        if(widget==nullptr)qApp->setStyleSheet(style.readAll());
        else widget->setStyleSheet(style.readAll());
        style.close();
    }
}*/

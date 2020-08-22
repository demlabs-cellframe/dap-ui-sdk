#include "CustomLineEditBase.h"

#include "Utilz.h"
#include "defines.h"

#include <QMargins>

CustomLineEditBase::CustomLineEditBase(QWidget* parent)
    : QLineEdit(parent)
    , m_useCustomPlaceholder(false)
    , m_layoutCtrl(Q_NULLPTR)
    , m_placeHolderCtrl(Q_NULLPTR)
    , m_lblIcon(Q_NULLPTR)
    , m_btnControl(Q_NULLPTR)
    , m_spacer(Q_NULLPTR)
{
    recreateSubControls();
}

CustomLineEditBase::CustomLineEditBase(const QString& contents,
                                       QWidget* parent)
    : QLineEdit(contents, parent)
    , m_useCustomPlaceholder(false)
    , m_layoutCtrl(Q_NULLPTR)
    , m_placeHolderCtrl(Q_NULLPTR)
    , m_lblIcon(Q_NULLPTR)
    , m_btnControl(Q_NULLPTR)
    , m_spacer(Q_NULLPTR)
{
    recreateSubControls();
}

void CustomLineEditBase::setVisibleIcon(bool &a_visible)
{
    if(a_visible)
    {
        if(m_lblIcon == Q_NULLPTR)
        {
            m_lblIcon = new ResizableIconLabel(this);
            connect(m_lblIcon, SIGNAL(resized()), this, SLOT(adjustTextMargins()), Qt::DirectConnection);

            m_lblIcon->setObjectName("image");

            if(m_layoutCtrl!=Q_NULLPTR)
            {
                m_layoutCtrl->insertWidget(0, m_lblIcon, 0, (Qt::AlignJustify | Qt::AlignVCenter));
            }
        }
        m_lblIcon->setVisible(a_visible);
        adjustTextMargins();
    }
    else
    {
        if(m_lblIcon != Q_NULLPTR)
        {
            delete m_lblIcon;
            m_lblIcon = Q_NULLPTR;
        }
    }
}

void CustomLineEditBase::setVisibleButton(bool a_visible)
{
    if(a_visible)
    {
        if(m_btnControl == Q_NULLPTR)
        {
            m_btnControl = new ResizablePushButton(this);
            connect(m_btnControl, SIGNAL(resized()), this, SLOT(adjustTextMargins()), Qt::DirectConnection);
            m_btnControl->setObjectName("control");

            if(m_layoutCtrl!=Q_NULLPTR)
            {
                m_layoutCtrl->addWidget(m_btnControl);
            }
            setSettingsButton();
        }

        m_btnControl->setVisible(a_visible);
    }
    else
    {
        if(m_btnControl!=Q_NULLPTR)
        {
            delete m_btnControl;
            m_btnControl = Q_NULLPTR;
        }
    }
}

void CustomLineEditBase::setWrongState(bool a_wrong)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::WRONG, a_wrong);
}

bool CustomLineEditBase::usingCustomPlaceholder() const
{
    return m_useCustomPlaceholder;
}

void CustomLineEditBase::setUsingCustomPlaceholder(bool useCustom)
{
    if(m_useCustomPlaceholder ? !useCustom : useCustom)
    {
        m_useCustomPlaceholder = useCustom;
        adjustPlaceholder();
    }
}

void CustomLineEditBase::focusOutEvent(QFocusEvent *event)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE, false);

    showCustomPlaceholder();
#ifdef Q_OS_ANDROID
    emit focusChanged(true);
    QApplication::inputMethod()->hide();
#endif
    QLineEdit::focusOutEvent(event);
}

void CustomLineEditBase::focusInEvent(QFocusEvent *event)
{
    hideCustomPlaceholder();

#ifdef Q_OS_ANDROID
    emit focusChanged(false);
    QApplication::inputMethod()->show();
#endif

    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE, true);

    QLineEdit::focusInEvent(event);
}

void CustomLineEditBase::recreateSubControls()
{
    m_spacer = new QSpacerItem(0, 0);

    m_layoutCtrl = new QHBoxLayout();

    m_layoutCtrl->setSpacing(0);
    m_layoutCtrl->setContentsMargins(0, 0, 0, 0);

    m_layoutCtrl->addItem(m_spacer);

    setLayout(m_layoutCtrl);

    adjustPlaceholder();

    adjustTextMargins();
}
///
/// \brief CustomLineEditBase::setUseCustomPlaceholder
/// \param a_alignment
// Sets the placeholder position to the left or center
void CustomLineEditBase::setPlaceholderAlignment(Qt::Alignment &a_alignment)
{
    if(m_spacer!=Q_NULLPTR)
    {
        switch (a_alignment)
        {
        case Qt::AlignLeft:
            m_spacer->changeSize(16000,0);
            break;
        case Qt::AlignCenter:
            m_spacer->changeSize(0,0);
            break;
        }
    }
}

void CustomLineEditBase::adjustPlaceholder()
{
    if(m_useCustomPlaceholder)
    {
        if(Q_NULLPTR == m_placeHolderCtrl)
        {
            m_placeHolderCtrl = new QLabel(this);
            m_placeHolderCtrl->setObjectName("placeholder");
            m_placeHolderCtrl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_placeHolderCtrl->setFocusPolicy(Qt::NoFocus);

            if(m_lblIcon == Q_NULLPTR)
                m_layoutCtrl->insertWidget(0, m_placeHolderCtrl, 0, (Qt::AlignJustify | Qt::AlignVCenter));
            else
                m_layoutCtrl->insertWidget(1, m_placeHolderCtrl, 0, (Qt::AlignJustify | Qt::AlignVCenter));

            if(hasFocus())
            {
                hideCustomPlaceholder();
            }
            else
            {
                showCustomPlaceholder();
            }
        }
    }
    else
    {
        if(Q_NULLPTR != m_placeHolderCtrl)
        {
            delete m_placeHolderCtrl;
            m_placeHolderCtrl = Q_NULLPTR;
        }
    }
}

void CustomLineEditBase::adjustPlaceholderText()
{
    if(Q_NULLPTR != m_placeHolderCtrl)
    {
        const QString placeHolderText(placeholderText());
        if(!placeHolderText.isEmpty())
        {
            m_placeHolderCtrl->setText(placeHolderText);
            setPlaceholderText(QString());

            update();
            m_placeHolderCtrl->update();
        }
    }
}

void CustomLineEditBase::showCustomPlaceholder()
{
    adjustPlaceholderText();

    if(Q_NULLPTR != m_placeHolderCtrl)
    {
        if(displayText().isEmpty())
        {
            m_placeHolderCtrl->show();
            m_layoutCtrl->setStretch(2, 0);
        }
        else
        {
            m_placeHolderCtrl->hide();
            m_layoutCtrl->setStretch(2, 1);
        }
    }
}

void CustomLineEditBase::hideCustomPlaceholder()
{
    adjustPlaceholderText();

    if(Q_NULLPTR != m_placeHolderCtrl)
    {
        m_placeHolderCtrl->hide();
        m_layoutCtrl->setStretch(2, 1);
    }
}

void CustomLineEditBase::adjustTextMargins()
{
    const QMargins marginsPrev(textMargins());
    QMargins marginsNew(marginsPrev.left(), marginsPrev.top(), 0, marginsPrev.bottom());

    if(Q_NULLPTR != m_lblIcon)
    {
        marginsNew.setLeft(m_lblIcon->width());
    }
    else
    {
        marginsNew.setLeft(0);
    }

    if(Q_NULLPTR != m_btnControl)
    {
        marginsNew.setRight(m_btnControl->width());
    }
    else
    {
        marginsNew.setRight(0);
    }

    if(marginsPrev != marginsNew)
    {
        setTextMargins(marginsNew);
    }

}


//////////////////////////////////////////////////////////////////////////

ResizableIconLabel::ResizableIconLabel(QWidget* parent)
    : QLabel(parent)
{
}

void ResizableIconLabel::resizeEvent(QResizeEvent* event)
{
    QLabel::resizeEvent(event);

    emit resized();
}

ResizablePushButton::ResizablePushButton(QWidget* parent)
    : QPushButton(parent)
{
}

void ResizablePushButton::resizeEvent(QResizeEvent* event)
{
    QPushButton::resizeEvent(event);

    emit resized();
}


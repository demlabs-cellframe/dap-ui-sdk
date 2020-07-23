#include "CustomLineEditBase.h"

#include "Utilz.h"
#include "defines.h"

CustomLineEditBase::CustomLineEditBase(QWidget* parent)
    : QLineEdit(parent)
    , customPlaceholder(false)
    , layoutCtrl(Q_NULLPTR)
    , placeHolderCtrl(Q_NULLPTR)
    , lblIcon(Q_NULLPTR)
    , btnControl(Q_NULLPTR)
    , spacer(Q_NULLPTR)
{
    recreateSubControls();
}

CustomLineEditBase::CustomLineEditBase(const QString& contents,
                                       QWidget* parent)
    : QLineEdit(contents, parent)
    , customPlaceholder(false)
    , layoutCtrl(Q_NULLPTR)
    , placeHolderCtrl(Q_NULLPTR)
    , lblIcon(Q_NULLPTR)
    , btnControl(Q_NULLPTR)
    , spacer(Q_NULLPTR)
{
    recreateSubControls();
}

void CustomLineEditBase::setUseCustomPlaceholder(bool custom)
{
    if(custom ? !customPlaceholder : customPlaceholder)
    {
        customPlaceholder = custom;
        adjustPlaceholder();
    }
}

void CustomLineEditBase::setVisibleIcon(bool &a_visible)
{
    lblIcon->setVisible(a_visible);
}

void CustomLineEditBase::setVisibleButton(bool a_visible)
{
    btnControl->setVisible(a_visible);
}

void CustomLineEditBase::setWrongState(bool a_wrong)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::WRONG, a_wrong);
}

void CustomLineEditBase::focusOutEvent(QFocusEvent *event)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE, false);

    if(text().isEmpty())
    {
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED, false);
    }
    else
    {
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED, true);
    }

    showCustomPlaceholder();

    QLineEdit::focusOutEvent(event);
}

void CustomLineEditBase::focusInEvent(QFocusEvent *event)
{
    hideCustomPlaceholder();

    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE, true);

    QLineEdit::focusInEvent(event);
}

void CustomLineEditBase::recreateSubControls()
{
    lblIcon = new QLabel(this);
    lblIcon->setObjectName("image");
    lblIcon->hide();

    spacer = new QSpacerItem(16000, 10);

    btnControl = new QPushButton(this);
    btnControl->setObjectName("control");
    connect(btnControl,&QPushButton::clicked,[=]{
        this->clear();
        setVisibleButton(false);
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED, false);
        this->setFocus();
    });
    btnControl->hide();

    layoutCtrl = new QHBoxLayout();
    layoutCtrl->setSpacing(0);
    layoutCtrl->setContentsMargins(0, 0, 0, 0);

    layoutCtrl->addWidget(lblIcon);
    layoutCtrl->setStretch(0, 0);
    layoutCtrl->addItem(spacer);
    layoutCtrl->setStretch(1, 1);
    layoutCtrl->addWidget(btnControl);
    layoutCtrl->setStretch(2, 0);

    setLayout(layoutCtrl);

    adjustPlaceholder();
}

void CustomLineEditBase::adjustPlaceholder()
{
    if(customPlaceholder)
    {
        if(Q_NULLPTR == placeHolderCtrl)
        {
            placeHolderCtrl = new QLabel(this);
            placeHolderCtrl->setObjectName("placeholder");
            placeHolderCtrl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            placeHolderCtrl->setFocusPolicy(Qt::NoFocus);

            layoutCtrl->insertWidget(1, placeHolderCtrl, 0, (Qt::AlignJustify | Qt::AlignVCenter));

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
        if(Q_NULLPTR != placeHolderCtrl)
        {
            delete placeHolderCtrl;
            placeHolderCtrl = Q_NULLPTR;
            layoutCtrl->setStretch(1, 1);
        }
    }
}

void CustomLineEditBase::adjustPlaceholderText()
{
    if(Q_NULLPTR != placeHolderCtrl)
    {
        const QString placeHolderText(placeholderText());
        if(!placeHolderText.isEmpty())
        {
            placeHolderCtrl->setText(placeHolderText);
            setPlaceholderText(QString());

            update();
            placeHolderCtrl->update();
        }
    }
}

void CustomLineEditBase::showCustomPlaceholder()
{
    adjustPlaceholderText();

    if(Q_NULLPTR != placeHolderCtrl)
    {
        if(displayText().isEmpty())
        {
            placeHolderCtrl->show();
            layoutCtrl->setStretch(2, 0);
        }
        else
        {
            placeHolderCtrl->hide();
            layoutCtrl->setStretch(2, 1);
        }
    }
}

void CustomLineEditBase::hideCustomPlaceholder()
{
    adjustPlaceholderText();

    if(Q_NULLPTR != placeHolderCtrl)
    {
        placeHolderCtrl->hide();
        layoutCtrl->setStretch(2, 1);
    }
}

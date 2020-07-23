#include "CustomLineEditBase.h"

#include "Utilz.h"
#include "defines.h"

CustomLineEditBase::CustomLineEditBase(QWidget* parent)
    : QLineEdit(parent)
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
    , layoutCtrl(Q_NULLPTR)
    , placeHolderCtrl(Q_NULLPTR)
    , lblIcon(Q_NULLPTR)
    , btnControl(Q_NULLPTR)
    , spacer(Q_NULLPTR)
{
    recreateSubControls();
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
        Utils::setPropertyAndUpdateStyle(this, Properties::WRONG,a_wrong);
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

    if(displayText().isEmpty())
    {
        placeHolderCtrl->show();
        layoutCtrl->setStretch(2, 0);
    }

    QLineEdit::focusOutEvent(event);
}

void CustomLineEditBase::focusInEvent(QFocusEvent *event)
{
    placeHolderCtrl->hide();
    layoutCtrl->setStretch(2, 1);

    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE, true);

    QLineEdit::focusInEvent(event);
}

void CustomLineEditBase::paintEvent(QPaintEvent* event)
{
    if(!adjustPlaceholderText())
    {
        QLineEdit::paintEvent(event);
    }
    else
    {
        event->accept();
    }
}

void CustomLineEditBase::recreateSubControls()
{
    lblIcon = new QLabel(this);
    lblIcon->setObjectName("image");
    lblIcon->hide();

    placeHolderCtrl = new QLabel(this);
    placeHolderCtrl->setObjectName("placeholder");
    placeHolderCtrl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    placeHolderCtrl->setFocusPolicy(Qt::NoFocus);
    placeHolderCtrl->hide();

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
    layoutCtrl->addWidget(placeHolderCtrl);
    layoutCtrl->setStretch(1, 1);
    layoutCtrl->addItem(spacer);
    layoutCtrl->setStretch(2, 0);
    layoutCtrl->addWidget(btnControl);
    layoutCtrl->setStretch(3, 0);

    setLayout(layoutCtrl);
}

bool CustomLineEditBase::adjustPlaceholderText()
{
    const QString placeHolderText(placeholderText());
    if(!placeHolderText.isEmpty())
    {
        placeHolderCtrl->setText(placeHolderText);
        setPlaceholderText(QString());

        update();
        placeHolderCtrl->update();

        return true;
    }

    return false;
}


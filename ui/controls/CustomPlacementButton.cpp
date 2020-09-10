#include "CustomPlacementButton.h"

#include "AppStyleSheetHandler.h"
#include "defines.h"
#include "Utilz.h"

/** @brief constructor
 *  @param a_parent object parent
 */
CustomPlacementButton::CustomPlacementButton(QWidget *a_parent)
    :CustomButtonAbstract (a_parent),
    m_layout        (new QHBoxLayout(this)),
    m_wgtLeftSpacing (this),
    m_lbImage       (this),
    m_lbText        (this),
    m_lbAdditionalImage(this),
    m_wgtRightSpacing(this)
{
    initButton();
}


void CustomPlacementButton::initButton()
{
    m_wgtLeftSpacing .setObjectName("leftSpacing");
    m_lbImage       .setObjectName("image");
    m_lbText        .setObjectName("text");
    m_lbAdditionalImage .setObjectName("additionalImage");
    m_wgtRightSpacing.setObjectName("rightSpacing");


    m_wgtLeftSpacing.setVisible(false);
    m_wgtRightSpacing.setVisible(false);
    m_lbAdditionalImage.setVisible(false);

    m_subcontrols.append(&m_lbImage);
    m_subcontrols.append(&m_lbText);
    m_subcontrols.append(&m_lbAdditionalImage);

    // Set up subcontroll object names:
    //Setup layout
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    //Setup spacing setSizePolicy
    m_wgtLeftSpacing .setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_wgtRightSpacing.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Adding subcontrols to layout
    m_layout->addWidget(&m_wgtLeftSpacing);
    for (QWidget *subcontrol: m_subcontrols)
    {
//        subcontrol->setScaledContents(true);
        m_layout->addWidget(subcontrol);

        CustomPlacementButton::setWidgetState(subcontrol);
    }
    m_layout->addWidget(&m_wgtRightSpacing);

//        m_layout->setDirection(QBoxLayout::LeftToRight);
    setLayout(m_layout);

    // on toggled update Appearance
    connect(this, &QAbstractButton::toggled, [=](bool a_checked)
    {
        this->setProperty(Properties::CHECKED, a_checked);
    });


    connect(this, &CustomPlacementButton::tabFocusIn, [=]()
    {
        this->updateAppearanceForFocus(true);
    });
    connect(this, &CustomPlacementButton::tabFocusOut, [=]()
    {
        this->updateAppearanceForFocus(false);
    });
}

void CustomPlacementButton::emitTabFocus(bool isActiv)
{
    isActiv ? emit tabFocusIn() : emit tabFocusOut();
}

QLabel *CustomPlacementButton::additionalImage(){return &m_lbAdditionalImage;}

QLabel *CustomPlacementButton::image(){return &m_lbImage;}

/** @brief Reimplemented QPushButton::setText method. Sets text property of text subcontrol.
 *  @param text Text
 */
void CustomPlacementButton::setText(const QString &text)
{
    m_lbText.setText(text);
}

void CustomPlacementButton::setIcon(const QString &path)
{
    QPixmap icon(path);
    m_lbImage.setPixmap(icon);
}

void CustomPlacementButton::setProperty(const QString &a_property, const QVariant &a_value)
{
    Utils::setPropertyAndUpdateStyle(this, a_property, a_value);

    Utils::setPropertyAndUpdateStyle(&m_wgtRightSpacing, a_property, a_value);
    for (QWidget* subcontrol: m_subcontrols)
    {
        if(subcontrol->layout()!=nullptr)
        {
            for(int numItem = 0; numItem < subcontrol->layout()->count(); numItem++)
            {
                Utils::setPropertyAndUpdateStyle(subcontrol->layout()->itemAt(numItem)->widget(), a_property, a_value);
            }
        }
        else
        Utils::setPropertyAndUpdateStyle(subcontrol, a_property, a_value);
    }
    Utils::setPropertyAndUpdateStyle(&m_wgtRightSpacing, a_property, a_value);
}

/** @brief Updates appearance of image and text
 */
void CustomPlacementButton::updateAppearanceForFocus(bool target)
{
    Q_UNUSED(target);
//    for (StyledSubcontrol *subcontrol: m_subcontrols){
//        subcontrol->setStylesheet(target, true);
//    }

    update();//for appliyng hover/normal stylesheet of button
}

/** @brief add new subcontrol and place it in layout
 *  @param a_id text id of subcontrol wich is using in CSS file
 *  @return created subcontrol
 */
QLabel* CustomPlacementButton::addSubcontrol(QString a_objectName)
{
    QLabel *newSubcontrol = new QLabel((QPushButton*)this);
    newSubcontrol->setObjectName(a_objectName);

    CustomPlacementButton::setWidgetState(newSubcontrol, this->underMouse(), isChecked());

    this->addSubcontrol(*newSubcontrol);
    return newSubcontrol;
}

QLabel *CustomPlacementButton::addSubcontrol(QWidget &a_widgetSubcontroll)
{
    m_subcontrols.append(&a_widgetSubcontroll);
    m_layout->insertWidget(m_layout->count() - 2, &a_widgetSubcontroll);
}

Qt::LayoutDirection CustomPlacementButton::layoutDirection() const
{
    return Utils::toQtLayoutDirection(m_layout->direction());
}

void CustomPlacementButton::setLayoutDirection(Qt::LayoutDirection a_direction)
{
    m_layout->setDirection(Utils::toQBoxLayoutDirection(a_direction));
}


/** @brief event is sent to the widget when the mouse cursor enters the widget.
 *  @param event
 */
void CustomPlacementButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    if (isEnabled())
        this->setProperty(Properties::HOVER, true);
}

/** @brief A leave event is sent to the widget when the mouse cursor leaves the widget.
 *  @param event
 */
void CustomPlacementButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    if (isEnabled())
        this->setProperty(Properties::HOVER, false);
}

void CustomPlacementButton::setWidgetState(QWidget *a_widget, bool a_isHover, bool a_isChecked)
{
    Utils::setPropertyAndUpdateStyle(a_widget, Properties::HOVER  , a_isHover  );
    Utils::setPropertyAndUpdateStyle(a_widget, Properties::CHECKED, a_isChecked);
}

Qt::Alignment CustomPlacementButton::alignment() const
{
    return m_alignment;
}
//If there is ALIGNMENT_NONE or some erroneous value, the widgets will be invisible.
void CustomPlacementButton::setAlignment(Qt::Alignment a_alignment)
{
    switch (a_alignment)
    {
        case Qt::AlignLeft:
        {
            m_wgtLeftSpacing.setVisible(false);
            m_wgtRightSpacing.setVisible(true);
            break;
        }
        case Qt::AlignRight:
        {
            m_wgtLeftSpacing.setVisible(true);
            m_wgtRightSpacing.setVisible(false);
            break;
        }
        case Qt::AlignHCenter:
        {
            m_wgtLeftSpacing.setVisible(true);
            m_wgtRightSpacing.setVisible(true);
            break;
        }
        case Qt::AlignJustify:
        default:
        {
            m_wgtLeftSpacing.setVisible(false);
            m_wgtRightSpacing.setVisible(false);
        }
    }
}

QString CustomPlacementButton::text()
{
    return m_lbText.text();
}

void CustomPlacementButton::setAdditionalImage(bool a_visible)
{
    m_lbAdditionalImage.setVisible(a_visible);
}
void CustomPlacementButton::checkStateSet()
{
    this->setProperty(Properties::CHECKED, isChecked());
    QPushButton::checkStateSet();
}

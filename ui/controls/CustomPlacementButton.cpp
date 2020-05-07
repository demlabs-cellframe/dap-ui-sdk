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
    m_wgtRightSpacing(this)
{
    initButton();
}


void CustomPlacementButton::initButton()
{
    m_wgtLeftSpacing .setObjectName("leftSpacing");
    m_lbImage       .setObjectName("image");
    m_lbText        .setObjectName("text");
    m_wgtRightSpacing.setObjectName("rightSpacing");


    m_wgtLeftSpacing.setVisible(false);
    m_wgtRightSpacing.setVisible(false);

    m_subcontrols.append(&m_lbImage);
    m_subcontrols.append(&m_lbText);

    // Set up subcontroll object names:
    //Setup layout
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    //Setup spacing setSizePolicy
    m_wgtLeftSpacing .setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_wgtRightSpacing.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Adding subcontrols to layout
    m_layout->addWidget(&m_wgtLeftSpacing);
    for (QLabel *subcontrol: m_subcontrols)
    {
        // Set up subcontroll ScaledContents:
        subcontrol->setScaledContents(true);
        m_layout->addWidget(subcontrol);

        CustomPlacementButton::setWidgetState(subcontrol);
    }
    m_layout->addWidget(&m_wgtRightSpacing);

//        m_layout->setDirection(QBoxLayout::LeftToRight);
    setLayout(m_layout);

    // on toggled update Appearance
    connect(this, &QAbstractButton::toggled, [=](bool a_checked) {
        this->setState(this->underMouse(), a_checked);
    });

    connect(this, &CustomPlacementButton::tabFocusIn, [=]() {

     //   this->updateAppearanceForFocus(true);

    });
    connect(this, &CustomPlacementButton::tabFocusOut, [=]() {

     //   this->updateAppearanceForFocus(false);

    });

    connect(this, &CustomPlacementButton::tabFocusIn, [=]() {

        this->updateAppearanceForFocus(true);

    });
    connect(this, &CustomPlacementButton::tabFocusOut, [=]() {

        this->updateAppearanceForFocus(false);

    });
}

void CustomPlacementButton::emitTabFocus(bool isActiv)
{
    isActiv ? emit tabFocusIn() : emit tabFocusOut();
}

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

/** @brief Updates appearance of image and text
 */
void CustomPlacementButton::setState(bool a_isHover, bool a_isChecked)
{
    setProperty(Properties::HOVER  , a_isHover  );
    setProperty(Properties::CHECKED, a_isChecked);
}

void CustomPlacementButton::setProperty(const QString &a_property, const QVariant &a_value)
{
    const char* property = a_property.toStdString().c_str();

    if (this->property(property) == a_value)
        return;

    Utils::setPropertyAndUpdateStyle(&m_wgtRightSpacing, property, a_value);

    for (QWidget* subcontrol: m_subcontrols)
    {
        Utils::setPropertyAndUpdateStyle(subcontrol, property, a_value);
    }

    Utils::setPropertyAndUpdateStyle(&m_wgtRightSpacing, property, a_value);
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
 */
void CustomPlacementButton::addSubcontrol(QString a_id)
{
    QLabel *newSubcontrol = new QLabel((QPushButton*)this);
    newSubcontrol->setObjectName(a_id);

    CustomPlacementButton::setWidgetState(newSubcontrol, this->underMouse(), isChecked());

    //add to list and layout
    m_subcontrols.append(newSubcontrol);
    m_layout->insertWidget(m_layout->count() - 1, newSubcontrol);
}

Qt::LayoutDirection CustomPlacementButton::layoutDirection() const
{
    return Utils::toQtLayoutDirection(m_layout->direction());
}

void CustomPlacementButton::setLayoutDirection(Qt::LayoutDirection a_direction)
{
    m_layout->setDirection(Utils::toQBoxLayoutDirection(a_direction));
}

void CustomPlacementButton::setGraphicsEffect(StyledDropShadowEffect *a_effect)
{
    m_styledShadow = a_effect;
    QPushButton::setGraphicsEffect(a_effect);
}


/** @brief event is sent to the widget when the mouse cursor enters the widget.
 *  @param event
 */
void CustomPlacementButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    if (isEnabled())
        setState(true, isChecked());

    if (m_styledShadow)
        m_styledShadow->updateStyle(HOVER_SHADOW);
}

/** @brief A leave event is sent to the widget when the mouse cursor leaves the widget.
 *  @param event
 */
void CustomPlacementButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    if (isEnabled())
        setState(false, isChecked());

    if (m_styledShadow)
        m_styledShadow->updateStyle(DEFAULT_SHADOW);
}

void CustomPlacementButton::setWidgetState(QWidget *a_widget, bool a_isHover, bool a_isChecked)
{
    Utils::setPropertyAndUpdateStyle(a_widget, Properties::HOVER  , a_isHover  );
    Utils::setPropertyAndUpdateStyle(a_widget, Properties::CHECKED, a_isChecked);
}

/** @brief Reimplemented QPushButton::setObjectName method. Updates stylesheets.
 *  @param name Name
 */
void CustomPlacementButton::setObjectName(const QString &name)
{
    QPushButton::setObjectName(name);

    if (m_styledShadow)
        m_styledShadow->updateStyleProperties();
}

//If there is ALIGNMENT_NONE or some erroneous value, the widgets will be invisible.
void CustomPlacementButton::setAlignment(Qt::Alignment a_alignment)
{
    if(a_alignment == Qt::AlignLeft)
    {
        m_wgtLeftSpacing.setVisible(false);
        m_wgtRightSpacing.setVisible(true);
        return;
    }
    if(a_alignment == Qt::AlignRight)
    {
        m_wgtLeftSpacing.setVisible(true);
        m_wgtRightSpacing.setVisible(false);
        return;
    }
    if(a_alignment == Qt::AlignHCenter)
    {
        m_wgtLeftSpacing.setVisible(true);
        m_wgtRightSpacing.setVisible(true);
        return;
    }

    m_wgtLeftSpacing.setVisible(false);
    m_wgtRightSpacing.setVisible(false);

}

QString CustomPlacementButton::text()
{
    return m_lbText.text();
}

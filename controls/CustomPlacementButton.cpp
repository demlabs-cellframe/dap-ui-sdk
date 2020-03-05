#include "CustomPlacementButton.h"

#include "AppStyleSheetHandler.h"
#include "defines.h"
#include "Utils.h"

/** @brief constructor
 *  @param a_parent object parent
 */
CustomPlacementButton::CustomPlacementButton(QWidget *a_parent)
    :QPushButton (a_parent),
    m_layout        (new QHBoxLayout(this)),
    m_lbLeftSpacing (this),
    m_lbImage       (this),
    m_lbText        (this),
    m_lbRightSpacing(this)
{
    m_lbLeftSpacing .setObjectName("leftSpacing");
    m_lbImage       .setObjectName("image");
    m_lbText        .setObjectName("text");
    m_lbRightSpacing.setObjectName("rightSpacing");

    m_lbRightSpacing.setVisible(false);
    m_lbLeftSpacing.setVisible(false);

    m_subcontrols.append(&m_lbImage);
    m_subcontrols.append(&m_lbText);

    // Set up subcontroll object names:

    //Setup layout
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    //Setup spacing setSizePolicy
    m_lbLeftSpacing .setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_lbRightSpacing.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Adding subcontrols to layout
    m_layout->addWidget(&m_lbLeftSpacing);
    for (QLabel *subcontrol: m_subcontrols)
    {
        // Set up subcontroll ScaledContents:
        subcontrol->setScaledContents(true);
        m_layout->addWidget(subcontrol);

        CustomPlacementButton::setWidgetState(subcontrol);
    }
    m_layout->addWidget(&m_lbRightSpacing);

    setLayout(m_layout);

    // on toggled update Appearance
    connect(this, &QAbstractButton::toggled, [=](bool a_checked) {
        this->setState(this->underMouse(), a_checked);
    });
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

    Utils::setPropertyAndUpdateStyle(&m_lbLeftSpacing, property, a_value);

    for (QWidget* subcontrol: m_subcontrols)
    {
        Utils::setPropertyAndUpdateStyle(subcontrol, property, a_value);
    }

    Utils::setPropertyAndUpdateStyle(&m_lbRightSpacing, property, a_value);
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

/** @brief Set image position relative to text (left or right)
 *  @param a_position image position relatife to text (Left/Right)
 */
void CustomPlacementButton::setImagePosition(ImagePos a_position /*= ImagePos::Left*/)
{
    int imageIndex = m_layout->indexOf(&m_lbImage);
    if (a_position == ImagePos::Left && imageIndex == 2)
    {
        m_layout->removeWidget(&m_lbImage);
        m_layout->insertWidget(1, &m_lbImage);
    }
    else if (a_position == ImagePos::Right && imageIndex == 1)
    {
        m_layout->removeWidget(&m_lbImage);
        m_layout->insertWidget(2, &m_lbImage);
    }
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

#include "CustomPlacementButton.h"
#include "AppStyleSheetHandler.h"
/** @brief constructor
 *  @param a_parent object parent
 */
CustomPlacementButton::CustomPlacementButton(QWidget *a_parent)
    :QPushButton (a_parent),
    m_layout        (new QHBoxLayout(this)),
    m_lbLeftSpacing (this),
    m_lbImage       (new StyledSubcontrol("image", this)),
    m_lbText        (new StyledSubcontrol("text", this)),
    m_lbRightSpacing(this)
{
    m_subcontrols.append(m_lbImage);
    m_subcontrols.append(m_lbText);

    // Set up subcontroll object names:
    m_lbLeftSpacing .setObjectName("leftSpacing");
    m_lbRightSpacing.setObjectName("rightSpacing");

    //Setup layout
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    //Setup spacing setSizePolicy
    m_lbLeftSpacing .setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_lbRightSpacing.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Adding subcontrols to layout
    m_layout->addWidget(&m_lbLeftSpacing);
    for (StyledSubcontrol *subcontrol: m_subcontrols){
        // Set up subcontroll ScaledContents:
        subcontrol->widget()->setScaledContents(true);
        m_layout->addWidget(subcontrol->widget());
    }
    m_layout->addWidget(&m_lbRightSpacing);

    setLayout(m_layout);

    // on toggled update Appearance
    connect(this, &QAbstractButton::toggled, [=]() {
        this->updateAppearance();
    });
}

/** @brief Reimplemented QPushButton::setText method. Sets text property of text subcontrol.
 *  @param text Text
 */
void CustomPlacementButton::setText(const QString &text)
{
    m_lbText->widget()->setText(text);
}

/** @brief Reimplemented QPushButton::setObjectName method. Updates stylesheets.
 *  @param text Text
 */
void CustomPlacementButton::setObjectName(const QString &name)
{
    QObject::setObjectName(name);

    updateStyleSheets();
}

/** @brief Reimplemented QAbstractButton::setCheckable method. Updates stylesheets.
 *  @param checkable whether button is checkable
 */
void CustomPlacementButton::setCheckable(bool checkable)
{
    QPushButton::setCheckable(checkable);

    updateStyleSheets();
}

/** @brief Initialization of image and text stylesheets
 */
void CustomPlacementButton::updateStyleSheets()
{
    for (StyledSubcontrol *subcontrol: m_subcontrols){
        subcontrol->updateStylesheets();
    }
    updateAppearance();
}

/** @brief Updates appearance of image and text
 */
void CustomPlacementButton::updateAppearance()
{
    for (StyledSubcontrol *subcontrol: m_subcontrols){
        bool isHover = isEnabled() ? underMouse() : false;
        subcontrol->setStylesheet(isHover, isChecked());
    }

    update();//for appliyng hover/normal stylesheet of button
}

/** @brief add new subcontrol and place it in layout
 *  @param a_id text id of subcontrol wich is using in CSS file
 */
void CustomPlacementButton::addSubcontrol(QString a_id)
{
    StyledSubcontrol *newSubcontrol = new StyledSubcontrol(a_id, this);
    newSubcontrol->widget()->setScaledContents(true);

    //add to list and layout
    m_subcontrols.append(newSubcontrol);
    m_layout->insertWidget(m_layout->count() - 1, newSubcontrol->widget());

    updateStyleSheets();
}

/** @brief Set image position relative to text (left or right)
 *  @param a_position image position relatife to text (Left/Right)
 */
void CustomPlacementButton::setImagePosition(ImagePos a_position /*= ImagePos::Left*/)
{
    int imageIndex = m_layout->indexOf(m_lbImage->widget());
    if (a_position == ImagePos::Left && imageIndex == 2)
    {
        m_layout->removeWidget(m_lbImage->widget());
        m_layout->insertWidget(1, m_lbImage->widget());
    }
    else if (a_position == ImagePos::Right && imageIndex == 1)
    {
        m_layout->removeWidget(m_lbImage->widget());
        m_layout->insertWidget(2, m_lbImage->widget());
    }
}


/** @brief event is sent to the widget when the mouse cursor enters the widget.
 *  @param event
 */
void CustomPlacementButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    if (isEnabled())
        updateAppearance();
}

/** @brief A leave event is sent to the widget when the mouse cursor leaves the widget.
 *  @param event
 */
void CustomPlacementButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    if (isEnabled())
        updateAppearance();
}

/** @brief  Reimplemented QWidget::changeEvent is sent to the widget when "enabled" changed.
 *  @param event
 */
void CustomPlacementButton::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::EnabledChange) {

        updateAppearance();

        return QWidget::changeEvent(event);
    }
    return QWidget::changeEvent(event);
}

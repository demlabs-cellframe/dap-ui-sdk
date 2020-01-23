#include "ServersComboBox.h"

/** @brief constructor
 *  @param a_parent object parent
 */
ServersComboBox::ServersComboBox(QWidget *parent):
    QComboBox (parent)
{
    setItemDelegate(new ComboBoxDelegate(this, this));

    view()->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

/**
 * @brief ServersComboBox::setArrowStyles
 * @param a_normal
 * @param a_on
 * @param a_hover
 */
void ServersComboBox::setArrowStyles(const QString& a_normal, const QString& a_on,
                                     const QString& a_hover)
{
    m_downArrowStyleSheet.normal = a_normal;
    m_downArrowStyleSheet.on = a_on;
    m_downArrowStyleSheet.hover = a_hover;
    updateStyleSheets();
}

/** @brief Initialization of stylesheets
 */
void ServersComboBox::updateStyleSheets()
{
    QString stylesheet("#%1::down-arrow { %2"
                       "; image:none "         //hide drop-down. Remove it if drop-down is needed!!!
                       "}");
    stylesheet = stylesheet.arg(objectName());

    //***********************
    //disable hover style.  Remove it if drop-down is needed!!!
    QString tempStyleSheet ("#%1:hover{%2%3}");
    tempStyleSheet = tempStyleSheet
            .arg(objectName())
            .arg(AppStyleSheetHandler::getWidgetStyleSheet(objectName()))
            .arg("background-color: transparent;");
    //***********************
    //disable drop-doun style.  Change it if drop-down is needed!!!
    m_downArrowStyleSheet.normal = tempStyleSheet + stylesheet.arg(""); //stylesheet.arg(m_downArrowStyleSheet.normal);
    m_downArrowStyleSheet.on     = tempStyleSheet + stylesheet.arg(""); //stylesheet.arg(m_downArrowStyleSheet.on);
    m_downArrowStyleSheet.hover  = tempStyleSheet + stylesheet.arg(""); //stylesheet.arg(m_downArrowStyleSheet.hover);


    setStyleSheet(m_downArrowStyleSheet.normal);
}

/** @brief Reimplemented QComboBox::setObjectName method. Updates stylesheets.
 *  @param text Text
 */
void ServersComboBox::setObjectName(const QString &name)
{
    QComboBox::setObjectName(name);

    updateStyleSheets();
}


/** @brief Reimplemented QComboBox::enterEvent is sent to the widget when the mouse cursor enters the widget.
 *  @param event
 */
void ServersComboBox:: enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    setStyleSheet(m_downArrowStyleSheet.hover);
}

/** @brief Reimplemented QComboBox::leaveEvent is sent to the widget when the mouse cursor leaves the widget.
 *  @param event
 */
void ServersComboBox::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    if (view()->isVisible()) //if popup is showed
        setStyleSheet(m_downArrowStyleSheet.on);
    else
        setStyleSheet(m_downArrowStyleSheet.normal);
}

/** @brief Reimplemented QComboBox::hidePopup is sent to the widget when the popup is closing.
 *  @param event
 */
void ServersComboBox::hidePopup()
{
    setStyleSheet(m_downArrowStyleSheet.normal);

    QComboBox::hidePopup();
}

/** @brief Reimplemented QComboBox::paintEvent method wich draw element.
 *
 * Chek text for fitting to subcontrol and cut it if is it necessary
 *  @param event
 */
void ServersComboBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStylePainter painter(this);

    QStyleOptionComboBox opt;
    this->initStyleOption(&opt);

    QRect textRect = style()->proxy()->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);

    //reduce textRect to icon size and spacings
    int iconSize = opt.iconSize.width() + 4;
    textRect = textRect.adjusted(iconSize + 1, 0, -1, 0);

    QString elidedText = fontMetrics().elidedText(opt.currentText, Qt::ElideRight, textRect.width());
    //if text is too long:
    if (elidedText != opt.currentText){

        //Reduce font size:
        QFont smallFont;
        smallFont.setPixelSize(int(round(font().pixelSize() / 1.1)));
        painter.setFont(smallFont);

        //Cut text if it is not fitting in comboBox still
         opt.currentText = QFontMetrics(smallFont).elidedText(opt.currentText, Qt::ElideRight, int(round(textRect.width() * 1.1)));
    }

    // draw the combobox frame, focusrect and selected etc.
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    // draw the icon and text
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

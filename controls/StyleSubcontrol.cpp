#include "StyleSubcontrol.h"
#include "DapStyleHolder.h"
#include <QDebug>
#include <QPushButton>

/** @brief Constructor
 *  @param a_id Text id of new subcontrol
 *  @param a_parent Object parent
 */
StyleSubcontrol::StyleSubcontrol(QString a_id, DapStyleHolder* holder, QWidget *a_parent)
    :QObject (a_parent), m_styleHolder(holder), m_widget(new QLabel(a_parent))
{
    Q_ASSERT(holder);
    m_widget->setObjectName(a_id);
}

/** @brief Gettter for id
 */
QString StyleSubcontrol::id()
{
    return m_widget->objectName();
}

/** @brief Gettter for widget
 */
QLabel *StyleSubcontrol::widget()
{
    return m_widget;
}

/** @brief Reload stylesheets from StyleHolder
 */
void StyleSubcontrol::updateStylesheets()
{
    QString objName = parent()->objectName();
    objName = objName.simplified().replace(" ", "");

    // If objName isn't empty
    if (objName.isEmpty())
        qWarning() << "CustomPlacementButton hasn't Object name";
    else {
        objName = "#" + objName;

        // Initialize stylesheets
        m_normalStylesheet     = m_styleHolder->getWidgetStyleSheet(objName, "", id());
        m_hoverStylesheet      = m_styleHolder->getWidgetStyleSheet(objName, "", id() + ":hover");
        //if isCheckable initialize checked stylesheet
        if (qobject_cast<QPushButton*>(parent())->isCheckable()) {
            m_checkedStylesheet      = m_styleHolder->getWidgetStyleSheet(objName, "", id() + ":checked");
            m_checkedHoverStylesheet = m_styleHolder->getWidgetStyleSheet(objName, "", id() + ":(?:checked:hover|hover:checked)");
        }
    }
}

/** @brief Apply stylesheet for particular state of CustomPlacementButton to it's own subcontrol widget
 *  @param a_hover Is CustomPlacementButton hovered
 *  @param a_checked Is CustomPlacementButton checked
 */
void StyleSubcontrol::setStylesheet(bool a_hover, bool a_checked)
{
    widget()->setStyleSheet(styleSheet(a_hover, a_checked));
}

/** @brief Get stylesheet for particular state of CustomPlacementButton
 *  @param a_hover Is CustomPlacementButton hovered
 *  @param a_checked Is CustomPlacementButton checked
 *  @return Prepeared stylesheet for all active states
 */
QString StyleSubcontrol::styleSheet(bool a_hover, bool a_checked/*= false*/)
{
    QString strStyle(m_normalStylesheet);
    //if hover - add hover stylesheet
    if (a_hover)
        strStyle += ";" + m_hoverStylesheet;
    //if isCheckable and checked - add checked stylesheet
    bool isCheckable = qobject_cast<QPushButton*>(parent())->isCheckable();
    if (isCheckable && a_checked) {
        if (a_hover)
            strStyle += ";" + m_checkedHoverStylesheet;
        else
            strStyle += ";" + m_checkedStylesheet;
    }
    return strStyle;
}

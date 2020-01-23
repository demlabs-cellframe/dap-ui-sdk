#include "StyledSubcontrol.h"
#include "AppStyleSheetHandler.h"
#include <QDebug>
#include <QPushButton>

/** @brief Constructor
 *  @param a_id Text id of new subcontrol
 *  @param a_parent Object parent
 */
StyledSubcontrol::StyledSubcontrol(QString a_id, QWidget *a_parent)
    : QObject (a_parent)
    , m_widget(new QLabel(a_parent))
{
    m_widget->setObjectName(a_id);
}

/** @brief Gettter for id
 */
QString StyledSubcontrol::id()
{
    return m_widget->objectName();
}

/** @brief Gettter for widget
 */
QLabel *StyledSubcontrol::widget()
{
    return m_widget;
}

/** @brief Reload stylesheets from StyleHolder
 */
void StyledSubcontrol::updateStylesheets()
{
    QString objName = parent()->objectName();
    objName = objName.simplified().replace(" ", "");

    // If objName isn't empty
    if (objName.isEmpty())
        qWarning() << "CustomPlacementButton hasn't Object name";
    else {
        objName = "#" + objName;

        // Initialize stylesheets
        StyleSheatSearchPar searchPar;
        searchPar.widgetName = objName;
        searchPar.subcontrol = id();

        m_normalStylesheet     = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);
        searchPar.pseudoClass = "hover";
        m_hoverStylesheet      = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);
        //if isCheckable initialize checked stylesheet
        if (qobject_cast<QPushButton*>(parent())->isCheckable()) {
            searchPar.pseudoClass = "checked";
            m_checkedStylesheet      = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);
            searchPar.pseudoClass = "(?:checked:hover|hover:checked)";
            m_checkedHoverStylesheet = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);
        }
    }
}

/** @brief Apply stylesheet for particular state of CustomPlacementButton to it's own subcontrol widget
 *  @param a_hover Is CustomPlacementButton hovered
 *  @param a_checked Is CustomPlacementButton checked
 */
void StyledSubcontrol::setStylesheet(bool a_hover, bool a_checked)
{
    widget()->setStyleSheet(styleSheet(a_hover, a_checked));
}

/** @brief Get stylesheet for particular state of CustomPlacementButton
 *  @param a_hover Is CustomPlacementButton hovered
 *  @param a_checked Is CustomPlacementButton checked
 *  @return Prepeared stylesheet for all active states
 */
QString StyledSubcontrol::styleSheet(bool a_hover, bool a_checked/*= false*/)
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

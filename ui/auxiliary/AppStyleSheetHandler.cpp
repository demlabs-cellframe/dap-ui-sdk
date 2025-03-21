#include "AppStyleSheetHandler.h"
#include <qdebug.h>
#include <QApplication>
#include <QFileInfo>
#include <QWidget>
#include <cmath>
//#include "UiScaling.h"
#include "math.h"

/// Read stylesheet from file and convert points to pixels
/// @details Check for existing and available to open
/// @param a_filePath file path
/// @return The read stylesheet
QString AppStyleSheetHandler::readStyleSheetFromFile(const QString &a_filePath)
{
    if (!QFileInfo::exists(a_filePath)) {
        QString errorMsg("File " + a_filePath + " not exists");
        qCritical() << errorMsg;
        throw errorMsg;
    }
    QFile styleSheetFile(a_filePath);

    if (!styleSheetFile.open(QFile::ReadOnly))
    {
        QString errorMsg("Can't load style sheet. " + styleSheetFile.errorString());
        qCritical() << errorMsg;
        throw errorMsg;
    }

    QString styleSheet(styleSheetFile.readAll());
    styleSheetFile.close();

    return convertPointsToPixels(styleSheet);
}

/// Get widget stylesheet by searching parameters
/// @param a_searchPar Searching parameters.
/// @return css-stylesheet of widget in a string representation.
QString AppStyleSheetHandler::getWidgetStyleSheet(StyleSheatSearchPar a_searchPar)
{
    // format subcontrol if exist:
    if (!a_searchPar.subcontrol.isEmpty())
        a_searchPar.subcontrol = "\\s*::\\s*" + a_searchPar.subcontrol;

    // format property if exist:
    if (!a_searchPar.dinamicProperty.isEmpty())
        a_searchPar.dinamicProperty = "\\s*\\[" + a_searchPar.dinamicProperty + "\\]";

    // format pseudoClass if exist:
    if (!a_searchPar.pseudoClass.isEmpty())
        a_searchPar.pseudoClass = ":" + a_searchPar.pseudoClass;

    QRegularExpression regExp(QString("%1%2%3%4\\s*"
                           "\\{"
                                "([^\\}]*)"
                           "\\}")
                   .arg(a_searchPar.widgetName).arg(a_searchPar.subcontrol).arg(a_searchPar.dinamicProperty).arg(a_searchPar.pseudoClass));

    // If not found output and return empty
    if (regExp.lastIndexIn(appStyleSheet()) == -1) {
        qWarning() << "styleSheet not found: " << regExp;
        return "";
    }

    //If found return found capture 1 without spaces and new line symbols
    return regExp.cap(1).remove(QRegularExpression("(\\s|\\n)"));

}

/// Get widget style by name, property, subcontrol(optional), pseudoClass.
/// @param a_widgetName Widget name.
/// @param a_dinamicProperty Dinamic property.
/// @param a_subcontrol Subcontrol for e.g.: "text","image:hover", etc...
/// @param a_pseudoClass Pseudoclass for e.g.: "hover","clicked", etc...
/// @return css-stylesheet of widget in a string representation.
QString AppStyleSheetHandler::getWidgetStyleSheet(const QString& a_widgetName,
                                            QString a_dinamicProperty /*= ""*/,
                                            QString a_subcontrol /*= ""*/,
                                            QString a_pseudoClass /*= ""*/)
{
    StyleSheatSearchPar searchPar;
    searchPar.widgetName        = a_widgetName;
    searchPar.dinamicProperty   = a_dinamicProperty;
    searchPar.subcontrol        = a_subcontrol;
    searchPar.pseudoClass       = a_pseudoClass;
    return getWidgetStyleSheet(searchPar);
}

/// set stylesheet for widget obtained from App stylesheet by Widget name, dinamic property, subcontrol, pseudoclass
/// @param a_widget Pointer to widget for wich will be setted stylesheet or wich name will be used for searching.
/// @param a_dinamicProperty Dinamic property.
/// @param a_subcontrol Subcontrol for e.g.: "text","image:hover", etc...
/// @param a_pseudoClass Pseudoclass for e.g.: "hover","clicked", etc...
/// @return true - if stilesheet was found, otherwise - false
bool AppStyleSheetHandler::setStyleSheetForWidget(QWidget *a_widget, QString a_dinamicProperty, QString a_subcontrol, QString a_pseudoClass)
{
    if (!a_widget)
        return false;

    StyleSheatSearchPar searchPar;
    searchPar.widgetName        = a_widget->objectName();
    searchPar.dinamicProperty   = a_dinamicProperty;
    searchPar.subcontrol        = a_subcontrol;
    searchPar.pseudoClass       = a_pseudoClass;
    QString widgetStyleSheet = getWidgetStyleSheet(searchPar);

    if (widgetStyleSheet.isEmpty())
        return false;

    a_widget->setStyleSheet(widgetStyleSheet);
    return true;
}

/// Get property value from a_stylesheet.
/// @param a_stylesheet Widget stylesheet without '{}'.
/// @param a_property Property name.
/// @return string value of a_property
QString AppStyleSheetHandler::getValueFromStylesheet(const QString a_styleSheet, const QString &a_property)
{
    QStringList strParametrs = a_styleSheet.split(";");
    strParametrs.removeAll("");

    QStringList tempParam;
    for(QString str : strParametrs)
    {
        tempParam = str.split(":");

        if (tempParam.at(0).simplified() == a_property){
            return tempParam.at(1).simplified();
        }
    }

    qWarning() << "styleSheet property not found: " << a_property;
    return "";
}

/// Stylesheet of current application
/// @return Stylesheet of current application
QString AppStyleSheetHandler::appStyleSheet()
{
    QApplication *app = appInstance();

    if (app)
        return app->styleSheet();

    else {
        qWarning() << "Can't get application styleSheet (Error while getting QApplication instance)";
        return QString();
    }
}

/// QCoreApplication::instance()
/// @return current QApplication
QApplication *AppStyleSheetHandler::appInstance()
{
    return qobject_cast<QApplication*>(QCoreApplication::instance());
}

QString AppStyleSheetHandler::convertPointsToPixels(const QString a_stylesheet)
{
    const QRegularExpression regExp("([\\d.])+((pt\\+)|(pt-)|(pt))");
    QMap<int, QString> matches;
    auto data = a_stylesheet;
    auto pos = 0;

    while ((pos = regExp.indexIn(data, pos)) != -1) {
        matches[pos] = regExp.cap(0);
        pos += regExp.matchedLength();
    }

    int displacement = 0; // diference between the begin strings length and the end strings length
//    float dpi = UiScaling::getNativDPI();
    float pointsvalue{};
    int pixelsValue{};
    for (auto index: matches.keys())
    {
        QString strInPoints = matches[index];
        if (strInPoints.contains("pt+"))
        {
            pointsvalue = matches[index].replace("pt+", "").toDouble();
            pixelsValue=std::ceil(UiScaling::pointsToPixels(pointsvalue, dpi));

        }
        else if (strInPoints.contains("pt-"))
        {
            pointsvalue = matches[index].replace("pt-", "").toDouble();
            pixelsValue=std::floor(UiScaling::pointsToPixels(pointsvalue, dpi));
        }
        else
        {
            pointsvalue = matches[index].replace("pt", "").toDouble();
            pixelsValue = qRound(UiScaling::pointsToPixels(pointsvalue, dpi));
            pixelsValue = (pixelsValue < 1 && pointsvalue > 0.f) ? 1 : pixelsValue;
        }


        QString strInPixels = QString::number(pixelsValue) + "px";
        data.replace(index + displacement, strInPoints.length(), strInPixels);

        displacement = displacement + strInPixels.length() - strInPoints.length();
    }

#ifdef QT_DEBUG
//Saving output css to build foller
    QFile file("./../outCSS.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&file);
    out << data;
#endif

    return data;
}

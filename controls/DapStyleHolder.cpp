#include "DapStyleHolder.h"
#include <qdebug.h>
#include <QApplication>
#include <QFileInfo>
#include <QWidget>

DapStyleHolder *DapStyleHolder::m_qAppCssStyleHolder = Q_NULLPTR;

/// Standard сonstructor.
DapStyleHolder::DapStyleHolder(QObject *parent)
{
    Q_UNUSED(parent)
}

/// Overloaded constructor.
/// @param file css file name.
DapStyleHolder::DapStyleHolder(const QString &filePath, bool loadStyleSheetNow, QObject *parent)
{
    Q_UNUSED(parent)
    if(!QFileInfo::exists(filePath)) {
        QString errorMsg("File " + filePath + " not exists");
        qCritical() << errorMsg;
        throw errorMsg;
    }

    m_fileStyleSheet.setFileName(filePath);

    if(loadStyleSheetNow)
        loadStyleSheet();
}

void DapStyleHolder::setStyleSheet(QWidget *a_widget, QString a_property,
                                   QString a_subcontrol, QString a_pseudoClass)
{
    a_widget->setStyleSheet(getWidgetStyleSheet(a_widget->objectName(),
                                                a_property, a_subcontrol, a_pseudoClass));
}

void DapStyleHolder::setStyleSheet(const QString& a_widgetNameCSS, QWidget *a_widget,
                                   QString a_property, QString a_subcontrol,
                                   QString a_pseudoClass)
{
    a_widget->setStyleSheet(getWidgetStyleSheet(a_widgetNameCSS, a_property, a_subcontrol, a_pseudoClass));
}

/// Get widget style by name and property and subcontrol(optional).
/// @param a_widgetName Widget name.
/// @param a_property Property.
/// @param a_subcontrol Subcontrol for e.g.: "text","image:hover"
/// @return css-style widget in string representation.
QString DapStyleHolder::getWidgetStyleSheet(const QString& a_widgetName,
                                            QString a_dinamicProperty /*= ""*/,
                                            QString a_subcontrol /*= ""*/,
                                            QString a_pseudoClass /*= ""*/) const
{
    // format subcontrol if exist:
    if (!a_subcontrol.isEmpty())
        a_subcontrol = "\\s*::\\s*" + a_subcontrol;

    // format property if exist:
    if (!a_dinamicProperty.isEmpty())
        a_dinamicProperty = "\\s*\\[" + a_dinamicProperty + "\\]";

    // format pseudoClass if exist:
    if (!a_pseudoClass.isEmpty())
        a_pseudoClass = ":" + a_pseudoClass;

    QRegExp regExp(QString("%1%2%3%4\\s*"
                           "\\{"
                                "([^\\}]*)"
                           "\\}")
                   .arg(a_widgetName).arg(a_subcontrol).arg(a_dinamicProperty).arg(a_pseudoClass));
    
    // If not found output and return empty
    if (regExp.lastIndexIn(m_appStyleSheet) == -1) {
        qWarning() << "styleSheet not found: " << regExp;
        return "";
    } 

    //If found return found capture 1 without spaces and new line symbols
    return regExp.cap(1).remove(QRegExp("(\\s|\\n)"));
}

/// Get property value from a_stylesheet.
/// @param a_stylesheet Widget stylesheet without '{}'.
/// @param a_property Property name.
/// @return string value of a_property
QString DapStyleHolder::getValueFromStylesheet(const QString a_styleSheet, const QString &a_property)
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

/// Get css-style application.
/// @return css-style application.
QString DapStyleHolder::getStyleSheet() const
{
    return m_appStyleSheet;
}

/// Update style sheet.
QFileDevice::FileError DapStyleHolder::loadStyleSheet()
{
    if(m_fileStyleSheet.open(QFile::ReadOnly))
    {
        m_appStyleSheet = m_fileStyleSheet.readAll();
        m_fileStyleSheet.close();
    } else {
        qCritical() << "Can't load style sheet. " << m_fileStyleSheet.errorString();
    }

    return m_fileStyleSheet.error();
}

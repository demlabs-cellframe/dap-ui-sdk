/****************************************************************************
**
** This file is part of the DapVpn project.
** 
** The class implements the interface for interacting with css files. Allows 
** you by the name of the widget and the property to load the style from 
** the css file.
** 
** Example: 
** c++ code:
** ...
** StyleHolder::getInstance().getWidgetStyleSheet("#Login #edPassword", "visible");
** ...
** 
** 
** css file:
** ...
** #Login #edPassword [visible]
** {
**      font: 24pt "Roboto";
**      color: rgb(255, 255, 255);
** }
** ...
**
****************************************************************************/

#ifndef APPSTYLESHEETHANDLER_H
#define APPSTYLESHEETHANDLER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>
#include <QApplication>

struct StyleSheatSearchPar{
     QString widgetName;
     QString dinamicProperty  = "";
     QString subcontrol       = "";
     QString pseudoClass      = "";
};

class AppStyleSheetHandler : public QObject
{
    Q_OBJECT
    AppStyleSheetHandler();             ///< private! Can't be created. Only for static methods

public:
    static QString readStyleSheetFromFile(const QString &filePath);

    static QString getWidgetStyleSheet(StyleSheatSearchPar a_searchPar);
    static QString getWidgetStyleSheet(const QString& a_widgetName, QString a_dinamicProperty = "", QString a_subcontrol = "", QString a_pseudoClass = "");
    static bool setStyleSheetForWidget(QWidget      * a_widget    , QString a_dinamicProperty = "", QString a_subcontrol = "", QString a_pseudoClass = "");

    static QString getValueFromStylesheet(const QString a_styleSheet, const QString &a_property);
    static QString appStyleSheet();

    static QApplication* appInstance();
    static QString convertPointsToPixels(const QString a_stylesheet);

};

#endif // APPSTYLESHEETHANDLER_H

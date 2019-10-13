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

#ifndef STYLEHOLDER_H
#define STYLEHOLDER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QRegExp>

struct StyleSheatSearchPar{
     QString widgetName;
     QString dinamicProperty  = "";
     QString subcontrol       = "";
     QString pseudoClass      = "";
};

class DapStyleHolder : public QObject
{
    Q_OBJECT
    
public:
    static DapStyleHolder *qAppCssStyleHolder() { return m_qAppCssStyleHolder; }

    static void setAppCssHolder(const QString &mainCssFilePath) {
        if(m_qAppCssStyleHolder != Q_NULLPTR)
            delete m_qAppCssStyleHolder;
        m_qAppCssStyleHolder = new DapStyleHolder(mainCssFilePath, true);
    }

    explicit DapStyleHolder(QObject *parent = nullptr);
    DapStyleHolder(const QString &filePath, bool loadStyleSheetNow = false, QObject *parent = nullptr);


    void setStyleSheet(const QString& a_widgetNameCSS, QWidget *a_widget, QString a_property = "",
                       QString a_subcontrol = "", QString a_pseudoClass = "");

    void setStyleSheet(QWidget *widget, QString a_property = "",
                       QString a_subcontrol = "", QString a_pseudoClass = "");

    QString getStyleSheet() const;

    QFileDevice::FileError loadStyleSheet(); // read css file to QString (m_appStyleSheet)

    QString getWidgetStyleSheet(const QString& a_widgetName, QString a_dinamicProperty = "",
                                QString a_subcontrol = "", QString a_pseudoClass = "") const;
    QString getWidgetStyleSheet(StyleSheatSearchPar a_searchPar) const;

    static QString getValueFromStylesheet(const QString a_styleSheet, const QString &a_property);

private:
    static DapStyleHolder * m_qAppCssStyleHolder;

    QFile   m_fileStyleSheet;       ///< Css-style file.
    QString m_appStyleSheet;        ///< Css-style file in string representation.
};

#endif // STYLEHOLDER_H

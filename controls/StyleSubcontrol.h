#ifndef STYLEDSUBCONTROL_H
#define STYLEDSUBCONTROL_H

#include <QObject>
#include <QLabel>
#include "DapStyleHolder.h"
#include <QDebug>

/** @brief Class that consist stylesheets for all object states
 *
 * @details
 * @todo Search style in comments also!
*/
class StyleSubcontrol: public QObject {
    Q_OBJECT

public:
    enum SubcontrolTypes {Image, Text};

    StyleSubcontrol(QString a_id, DapStyleHolder* holder, QWidget *a_parent);

    QString id();
    QLabel *widget();

    void updateStylesheets();
    void setStylesheet(bool a_hover, bool a_checked);

    QString styleSheet(bool a_hover, bool a_checked = false);

private:
    QString m_normalStylesheet;         ///< Subcontrol StyleSheet in normal  state of CustomPlacementButton
    QString m_hoverStylesheet;          ///< Subcontrol StyleSheet in hover   state of CustomPlacementButton
    QString m_checkedStylesheet;        ///< Subcontrol StyleSheet in checked state of CustomPlacementButton
    QString m_checkedHoverStylesheet;   ///< Subcontrol StyleSheet in checked and hover state of CustomPlacementButton

    DapStyleHolder *m_styleHolder;
    QLabel  *m_widget;
};


#endif // STYLEDSUBCONTROL_H

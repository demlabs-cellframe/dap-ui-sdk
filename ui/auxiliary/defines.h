#ifndef DEFINES_H
#define DEFINES_H


#include <QString>

namespace Properties
{
    static const QString TEXT    = "text";
    static const QString STATE   = "state";
    static const QString ACTIVE  = "active";
    static const QString FILLED  = "filled";
    static const QString WRONG   = "wrong";
    static const QString ENABLED = "enabled";
    static const QString HOVER   = "hover"  ;
    static const QString CHECKED = "checked";
    static const QString VISIBLE = "visible";
    static const QString VALID   = "valid";
    static const QString SELECTED= "selected";
    static const QString CURRENT_INDEX = "currentIndex";
    static const QString CURRENT_TEXT = "currentText";
}
enum class Language
{
    Undefined,
    En,
    Zh
};


#endif // DEFINES_H

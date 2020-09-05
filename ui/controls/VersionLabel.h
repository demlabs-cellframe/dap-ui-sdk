#ifndef VERSIONLABEL_H
#define VERSIONLABEL_H

#include <QLabel>
#include <QDateTime>
#include <QLocale>

class VersionLabel: public QLabel
{
public:
    VersionLabel(QWidget *a_parent = nullptr);
/// Set the text as follows:
/// Version %1 %2 1-specify the version number.
/// 2-specifies the build date
/// for line transfer use \n
    void setText(const QString &a_text);
    static QString getVersion();
};

#endif // VERSIONLABEL_H

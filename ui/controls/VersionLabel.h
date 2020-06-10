#ifndef VERSIONLABEL_H
#define VERSIONLABEL_H

#include <QLabel>

class VersionLabel: public QLabel
{
public:
    VersionLabel(QWidget *a_parent = nullptr);
/// Set the text as follows:
/// Version %1 %2 1-specify the version number.
/// 2-specifies the build date
/// for line transfer use \n
    void setText(const QString &a_text);
};

#endif // VERSIONLABEL_H

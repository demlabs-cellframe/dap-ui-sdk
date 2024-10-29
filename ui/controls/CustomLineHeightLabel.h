#ifndef CUSTOMLINEHEIGHTLABEL_H
#define CUSTOMLINEHEIGHTLABEL_H

#include <QLabel>

class CustomLineHeightLabel: public QLabel
{
public:
    explicit CustomLineHeightLabel(QWidget *a_parent = Q_NULLPTR);

    void setText(const QString &text);
    void setObjectName(const QString &name);

    void updateStyleSheets();

private:
    QString m_lineHeight;
};

#endif // CUSTOMLINEHEIGHTLABEL_H

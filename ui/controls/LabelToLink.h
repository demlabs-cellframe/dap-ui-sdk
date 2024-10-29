#ifndef LABELTOLINK_H
#define LABELTOLINK_H

#include <QLabel>
#include <QMouseEvent>
#include <QDesktopServices>

class LabelToLink: public QLabel
{
public:
    LabelToLink(QWidget *a_parent = nullptr);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // LABELTOLINK_H

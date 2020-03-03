#ifndef ABSTRACTWIDGETDELEGATE_H
#define ABSTRACTWIDGETDELEGATE_H

#include <QWidget>

class AbstractWidgetDelegate : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractWidgetDelegate(QWidget *parent = nullptr);

signals:

};

#endif // ABSTRACTWIDGETDELEGATE_H

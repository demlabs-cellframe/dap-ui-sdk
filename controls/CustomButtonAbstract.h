#ifndef CUSTOMBUTTONABSTRACT_H
#define CUSTOMBUTTONABSTRACT_H

#include <QWidget>
#include <QAbstractButton>
#include <QPushButton>

class CustomButtonAbstract: public QPushButton
{
public:
    CustomButtonAbstract(QWidget *a_parent = nullptr);

    virtual void setText(const QString& )=0;
    virtual QString text()=0;

};

#endif // CUSTOMBUTTONABSTRACT_H

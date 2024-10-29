#ifndef CUSTOMBUTTONABSTRACT_H
#define CUSTOMBUTTONABSTRACT_H

#include <QWidget>
#include <QAbstractButton>
#include <QPushButton>

enum class ImagePos {Left, Right};

class CustomButtonAbstract: public QPushButton
{
public:
    CustomButtonAbstract(QWidget *a_parent = nullptr);

    virtual void setText(const QString& )=0;
    virtual QString text()= 0;
    virtual void setProperty(const QString &a_property, const QVariant &a_value) = 0;
};

#endif // CUSTOMBUTTONABSTRACT_H

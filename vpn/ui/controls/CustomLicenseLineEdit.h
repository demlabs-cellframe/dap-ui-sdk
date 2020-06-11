#ifndef CUSTOMLICENSELINEEDIT_H
#define CUSTOMLICENSELINEEDIT_H

#include <QLineEdit>
#include <QClipboard>
#include <QApplication>
#include <QKeyEvent>
#include <QMenu>
#include <QDebug>


#include <QTapAndHoldGesture>
#include <QGestureEvent>

#define LICENSE_LENGTH 16

class CustomLicenseLineEdit : public QLineEdit
{
    Q_OBJECT

private:

public:
    explicit CustomLicenseLineEdit(QWidget *parent = nullptr);

    QString text() const;


signals:
    void sigPasteAllLicense(QString license);

    void onFocusLicense();

public slots:
    virtual void paste();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void pasteEvent();

#ifdef Q_OS_ANDROID
    virtual void inputMethodEvent(QInputMethodEvent *e);
    virtual void focusInEvent(QFocusEvent *e);

#endif
};

#endif // CUSTOMLICENSELINEEDIT_H

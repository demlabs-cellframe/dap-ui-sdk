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

public:
    explicit CustomLicenseLineEdit(QWidget *parent = nullptr);

    QString text() const;

#ifdef Q_OS_ANDROID
    QString getText(){return m_serial;}
#endif

signals:
    void sigPasteAllLicense(QString license);

    void onFocusLicense();
    //Indicates the need to switch focus to a widget such as a button. Made for Android because of the features of the input fields.
    void focusOutNeeded();
    //Called for any loss of focus.
    void focusOutLicense();
public slots:
    virtual void paste();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void pasteEvent();

#ifdef Q_OS_ANDROID
    virtual void inputMethodEvent(QInputMethodEvent *e);
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);

private:
    QString m_serial;

    ///A key indicating that the focus appeared at the click of the mouse (tap on the screen). Made for Android.
    bool m_focusFromMouse = false;
#endif
};

#endif // CUSTOMLICENSELINEEDIT_H

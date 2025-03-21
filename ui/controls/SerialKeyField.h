#ifndef SERIALKEYFIELD_H
#define SERIALKEYFIELD_H

#include <QLineEdit>
#include <QRegExpValidator>
#include <QDebug>
#include <QInputMethodEvent>
#include <QApplication>
#include <QClipboard>
#include <CustomLineEdit.h>

#ifndef VALIDATOR
#define VALIDATOR "A-Z0-9"
#endif

#ifndef MAX_COUNT_CHAR
#define MAX_COUNT_CHAR 16
#endif

class SerialKeyField:public CustomLineEdit
{
    Q_OBJECT
public:
    SerialKeyField(QWidget* parent = Q_NULLPTR);
    bool isFilledOut();
signals:
    void textEditedAndCleaned();
    void textEditedAndFilledOut(QString);

    void textChangedAndCleaned();
    void textChangedAndFilledOut(QString);

    void pasteTextCaused(QString);
public slots:
    void paste();
private:
    void replaceNextCharacter(const QString& text);
protected:
    virtual void inputMethodEvent(QInputMethodEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void pasteEvent(QString clipboardText);
private:
    QRegularExpression m_regExp{};
#ifdef Q_OS_ANDROID
    QVector <int> m_keys
    {
        Qt::Key_0, Qt::Key_1,
        Qt::Key_2, Qt::Key_3,
        Qt::Key_4, Qt::Key_5,
        Qt::Key_6, Qt::Key_7,
        Qt::Key_8, Qt::Key_9,
    };
#endif
};

#endif // SERIALKEYFIELD_H

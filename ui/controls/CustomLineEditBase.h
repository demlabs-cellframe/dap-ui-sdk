#ifndef CUSTOMLINEEDITBASE_H
#define CUSTOMLINEEDITBASE_H

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFocusEvent>

#include <QApplication>
/*
*
*thisWidget
*{
*    qproperty-visibleIcon: true;       for visible left icon
*    qproperty-visibleButton: true;     for visible right button
*
*   If you do not specify these 2 lines, the widget will be exactly the same as its parent,
*                                               and the settings blocks below will not be needed.
*   To offset the text field, use
*                   padding-left: (width image) + margin to text field
*                   padding-right: (width image) + margin to text field
*   .......... Other settings as usual
*}
*
*thisWidget #image (QLabel)
*{
*    min-height: 29pt;
*    max-height: 29pt;
*    min-width: 29pt;
*    max-width: 29pt;
*       ....... and etc
*
*    image: url(image path);
*}
*
*thisWidget #control (QPushButton)
*{
*    min-height: 20pt;
*    max-height: 20pt;
*    min-width: 20pt;
*    max-width: 20pt;
*       ....... and etc
*    image: url(:/pics/delete_icon.svg);
*}
*/

class ResizableIconLabel;
class ResizablePushButton;

class CustomLineEditBase : public QLineEdit
{
    Q_OBJECT

    Q_PROPERTY(bool visibleIcon WRITE setVisibleIcon DESIGNABLE true)
    Q_PROPERTY(bool visibleButton WRITE setVisibleButton DESIGNABLE true)
    Q_PROPERTY(bool useCustomPlaceholder READ useCustomPlaceholder WRITE setUseCustomPlaceholder)

public:
    CustomLineEditBase(QWidget* parent = Q_NULLPTR);
    CustomLineEditBase(const QString& contents,
                       QWidget* parent = Q_NULLPTR);

public:
    /// Setting the error signal.
    /// @param a_wrong
    void setWrongState(bool a_wrong);
    bool useCustomPlaceholder() const;
    void setUseCustomPlaceholder(bool useCustom);

protected:
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void focusInEvent(QFocusEvent *event);

protected:
    /// Sets the visibility of the icon.
    /// @param a_visible This parameter is taken from css settings.
    void setVisibleIcon(bool &a_visible);
    /// Sets the visibility of the button.
    /// @param a_visible This parameter is taken from css settings.
    void setVisibleButton(bool a_visible);

private:
    void recreateSubControls();
    void adjustPlaceholder();
    void adjustPlaceholderText();
    void showCustomPlaceholder();
    void hideCustomPlaceholder();

private slots:
    void adjustTextMargins();

private:
    bool    m_useCustomPlaceholder;

private:
    QHBoxLayout*    m_layoutCtrl;
    QLabel*         m_placeHolderCtrl;

private:
    ///Widget for icon
    ResizableIconLabel* m_lblIcon;

protected:
    ///Widget for button
    ResizablePushButton*    m_btnControl;

private:
    QSpacerItem*    m_spacer;
};

////////////////////////////////////////////////////////////////////

class ResizableIconLabel : public QLabel
{
    Q_OBJECT

    friend class CustomLineEditBase;

private:
    ResizableIconLabel(QWidget* parent = Q_NULLPTR);

signals:
    void resized();

protected:
    virtual void resizeEvent(QResizeEvent *event);
};

class ResizablePushButton : public QPushButton
{
    Q_OBJECT

    friend class CustomLineEditBase;

private:
    ResizablePushButton(QWidget* parent = Q_NULLPTR);

signals:
    void resized();

protected:
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // CUSTOMLINEEDITBASE_H

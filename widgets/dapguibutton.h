#ifndef DAPGUIBUTTON_H
#define DAPGUIBUTTON_H

/* INCLUDES */
#include <QWidget>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QUrl>
#include <QMap>
#include "style/dapguistylemanager.h"
#include "dapguilineedit.h"
#include <memory>

/* DEFS */
namespace Ui { class DapGuiButtonUI; };

/****************************************//**
 * @brief framed button with customiseable style
 * @ingroup groupDapGuiWidgets
 * @date 19.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiButton : public QWidget
{
  Q_OBJECT
  DAPGUI_ENABLECSS
  DAPGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum ButtonStyle
  {
    TopMainBottomSub,     ///< top:main, bottom:sub
    TopSubBottomMain,     ///< top:sub,  bottom:main
    LeftTopMainBottomSub, ///< left:left, top:main, bottom:sub
    IconMainSub,          ///< icon, main, sub
    EditTopMainBottomSub, ///< top:edit, bottom:sub
  };
  Q_ENUM (ButtonStyle)
  /// @}

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (ButtonStyle buttonStyle READ btnStyle     WRITE setBtnStyle)
  Q_PROPERTY (QString mainText        READ mainText     WRITE setMainText)
  Q_PROPERTY (QString subText         READ subText      WRITE setSubText)
  Q_PROPERTY (QString leftText        READ leftText     WRITE setLeftText)
  Q_PROPERTY (QString mainCssClass    READ mainCssClass WRITE setMainCssClass)
  Q_PROPERTY (QString subCssClass     READ subCssClass  WRITE setSubCssClass)
  Q_PROPERTY (QString leftCssClass    READ leftCssClass WRITE setLeftCssClass)
  Q_PROPERTY (QString iconCssClass    READ iconCssClass WRITE setIconCssClass)
  Q_PROPERTY (QString inputMask READ inputMask WRITE setInputMask)
  Q_PROPERTY (bool separator          READ separator    WRITE setSeparator)
  Q_PROPERTY (bool link               READ link         WRITE setLink)
  Q_PROPERTY (bool frame              READ frame        WRITE setFrame)
  Q_PROPERTY (int maxLength           READ maxLength    WRITE setMaxLength)

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::DapGuiButtonUI *ui;
  ButtonStyle m_btnStyle; ///< button style
  QString m_mainText;
  QString m_subText;
  QString m_leftText;
  QString m_mainCssClass;
  QString m_subCssClass;
  QString m_leftCssClass;
  QString m_iconCssClass;
  QString m_inputMask;
  bool m_separator;       ///< bottom separator
  bool m_link;            ///< link button
  bool m_frame;           ///< show frame

  QGraphicsDropShadowEffect *m_shadowEffect; ///< @note attached widget will take control. this means instance doesnt need to be deleted
  QMultiMap<QString, QWidget *> m_widgets; ///< categorized map for widget

  /// labels
  std::unique_ptr<QLabel> m_lLink;
//  std::unique_ptr<QLabel> m_lMain,
//      m_lSub,
//      m_lLeft,
//      m_lLink,
//      m_lIcon;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiButton (QWidget *parent = 0);
  ~DapGuiButton() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  ButtonStyle btnStyle() const;
  void setBtnStyle (const ButtonStyle &style);

  QString mainText() const;
  void setMainText (const QString &mainText);

  QString subText() const;
  void setSubText (const QString &subText);

  QString leftText() const;
  void setLeftText (const QString &leftText);

  QString mainCssClass() const;
  void setMainCssClass (const QString &mainStyle);

  QString subCssClass() const;
  void setSubCssClass (const QString &subStyle);

  QString leftCssClass() const;
  void setLeftCssClass (const QString &leftStyle);

  QString iconCssClass() const;
  void setIconCssClass (const QString &iconCssClass);

  QString inputMask() const;
  void setInputMask(const QString &inputMask);

  DapGuiLineEdit *edit() const;
  /// @warning this is permanent. ask Mikhail Shilenko for more info
  void setEdit (QWidget *newEdit) const;

  DapGuiLineEdit::cbTextEdit inputCallback() const;
  void setInputCallback (const DapGuiLineEdit::cbTextEdit &cb);

  DapGuiLineEdit::cbFocusEvent inputFocusCallback() const;
  void setInputFocusCallback (const DapGuiLineEdit::cbFocusEvent &cb);

  bool link() const;
  void setLink (bool link);

  bool frame() const;
  void setFrame (bool frame);

  bool showIcon() const;
  void setShowIcon (bool showIcon);

  bool separator() const;
  void setSeparator (bool separator);

  int maxLength() const;
  void setMaxLength (const int &max);

  void setupStyle();
  void setupLabels();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  //void paintEvent (QPaintEvent *) override;
  void mousePressEvent (QMouseEvent *) override;
  void resizeEvent (QResizeEvent *) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void clicked();         ///< button is clicked
  void textChanged(const QString &);
  void textEdited(const QString &);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
private slots:
  /// copy info into clipboard when alt hold and mouse press
  void _slotDebugInfo();
  void _slotTextEdited(const QString &a_text);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIBUTTON_H

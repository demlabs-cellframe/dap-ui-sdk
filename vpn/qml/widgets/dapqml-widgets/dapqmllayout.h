#ifndef DAPQMLLAYOUT_H
#define DAPQMLLAYOUT_H

/* INCLUDES */
#include <QQuickItem>

/****************************************//**
 * @brief Layout Widget Class
 * @ingroup groupDapGuiStyle
 * @date 27.09.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlLayout : public QQuickItem
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (qreal spacing       READ spacing        WRITE setSpacing        NOTIFY sigSpacingChanged)
  Q_PROPERTY (qreal contentWidth  READ contentWidth   WRITE setContentWidth   NOTIFY sigContentWidthChanged)
  Q_PROPERTY (qreal contentHeight READ contentHeight  WRITE setContentHeight  NOTIFY sigContentHeightChanged)
  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  qreal m_spacing;
  qreal m_contentWidth;
  qreal m_contentHeight;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlLayout (QQuickItem *a_parent = nullptr);
  ~DapQmlLayout() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  qreal spacing() const;
  void setSpacing (qreal a_value);

  qreal contentWidth() const;
  void setContentWidth (qreal a_value);

  qreal contentHeight() const;
  void setContentHeight (qreal a_value);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  QSGNode *updatePaintNode (QSGNode *, QQuickItem::UpdatePaintNodeData *) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigSpacingChanged();
  void sigContentWidthChanged();
  void sigContentHeightChanged();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLLAYOUT_H

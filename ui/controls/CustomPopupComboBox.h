#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H


#include <QComboBox>

class CustomComboBoxPopup;

enum PositionPopup{
    defaultPosition, //bottom combobox
    overlappingPosition,
    usingOffsetLeftEdge
};
///
/// \brief The CustomPopupComboBox class
class CustomPopupComboBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QString caption WRITE setCaption DESIGNABLE true)
    Q_PROPERTY(QString offsetLeftEdge WRITE setOffsetLeftEdge DESIGNABLE true)
public:


    CustomPopupComboBox(QWidget *parent = Q_NULLPTR);

    virtual void showPopup() override;

    CustomComboBoxPopup *popup() const;
    void setPopup(CustomComboBoxPopup *popup);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *a_model);

    virtual void setCaption(const QString &a_text);
    QString caption() const;

    void setPositionPopup(PositionPopup a_positon);
    //Sets the offset to the left of the styles
    void setOffsetLeftEdge(QString &a_offeset);
private:

    CustomComboBoxPopup *m_popup = nullptr;
    QString m_caption;
    int m_offsetLeftEdge;
    PositionPopup m_positionPopup = PositionPopup::defaultPosition;
};
#endif // CUSTOMCOMBOBOX_H

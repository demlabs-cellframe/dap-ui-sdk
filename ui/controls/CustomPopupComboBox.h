#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H


#include <QComboBox>

class CustomComboBoxPopup;

///
/// \brief The CustomPopupComboBox class
///
/// \details set this->popup()->sizePolicy().horizontalPolicy() to QSizePolicy::Fixed if you want set up concreate popup width
/// otherwise it will be seted up the same width as comboBox
class CustomPopupComboBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QString caption WRITE setCaption DESIGNABLE true)
public:
    CustomPopupComboBox(QWidget *parent = Q_NULLPTR);

    virtual void showPopup() override;

    CustomComboBoxPopup *popup() const;
    void setPopup(CustomComboBoxPopup *popup);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *a_model);

    virtual void setCaption(const QString &a_text);
    QString caption() const;


private:

    CustomComboBoxPopup *m_popup = nullptr;
    QString m_caption;
};
#endif // CUSTOMCOMBOBOX_H

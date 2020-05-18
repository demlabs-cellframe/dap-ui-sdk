#ifndef FAQWIDGET_H
#define FAQWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QStyle>

#include "CustomLineHeightLabel.h"

struct FAQDataModel
{
    QString text;
    QString aboutText;
};

class FAQWidget: public QWidget
{
    Q_OBJECT
public:
    /// Widget constructor.
    /// @param model Data from model with text.
    /// @param parent
    FAQWidget(const FAQDataModel &model, QWidget *parent = nullptr);
protected:
    /// Expands and closes the list, changes the settings for the style.
    /// @param event
    void mousePressEvent(QMouseEvent *event);
private:
    //Right arrow icon
    QLabel *m_lblIcon;
    //Reference Information Description
    CustomLineHeightLabel *m_lblHeader;
    //Description of the problem
    CustomLineHeightLabel *m_lblText;
    //Line under the description
    QLabel *m_lblBottomLine;
    QHBoxLayout *m_topLayout;
    QVBoxLayout *m_mainLayout;
};

#endif // FAQWIDGET_H

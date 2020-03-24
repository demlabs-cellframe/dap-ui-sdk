#include "FAQScreen.h"

const QString FAQScreen::SCREEN_NAME = "FAQ";

FAQScreen::FAQScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::FAQScreen>();

}

QString FAQScreen::screenName()
{
    return FAQScreen::SCREEN_NAME;
}

void FAQScreen::setState(ConnectionStates a_state)
{
    Q_UNUSED(a_state)


}

void FAQScreen::initVariantUi(QWidget *a_widget)
{

    QScrollArea *scaFAQ = a_widget->findChild<QScrollArea*>(SCA_FAQ);

    Q_ASSERT(scaFAQ);

    m_dataModel = new QList<FAQDataModel>();
    //set model
    FAQDataModel tmpModel;
    tmpModel.text = "Why KelVPN is best?";
    tmpModel.aboutText = "The widget is the atom of the user interface: it receives mouse, keyboard and other events from the window system, and paints a representation of itself on the screen. Every widget is rectangular, and they are sorted in a Z-order. A widget is clipped by its parent and by the widgets in front of it.";
    m_dataModel->append(tmpModel);

    tmpModel.text = "Can you provide information on any parking policies in your car parks?";
    tmpModel.aboutText = "QTextEdit works on paragraphs and characters. A paragraph is a formatted string which is word-wrapped to fit into the width of the widget. By default when reading plain text, one newline signifies a paragraph. A document consists of zero or more paragraphs. The words in the paragraph are aligned in accordance with the paragraph's alignment. Paragraphs are separated by hard line breaks. Each character within a paragraph has its own attributes, for example, font and color.";
    m_dataModel->append(tmpModel);
    tmpModel.text = "Can you provide information on any parking policies in your car parks?";
    tmpModel.aboutText = "QAbstractScrollArea is a low-level abstraction of a scrolling area. The area provides a central widget called the viewport, in which the contents of the area is to be scrolled (i.e, the visible parts of the contents are rendered in the viewport).";
    m_dataModel->append(tmpModel);
    tmpModel.text = "Why KelVPN is best?";
    tmpModel.aboutText = "This signal is emitted when all properties have been assigned their final value. If the state assigns a value to one or more properties for which an animation exists (either set on the transition or as a default animation on the state machine), then the signal will not be emitted until all such animations have finished playing.";
    m_dataModel->append(tmpModel);


    m_wgtScroll = new QWidget() ;
    m_VericalLayout = new QVBoxLayout();
    m_listWgt = new QList<QWidget *>();
    m_wgtScroll->setObjectName("wgtScrollArea");


    for(int i = 0; i < m_dataModel->size();i++)
    {
        m_listWgt ->append(new FAQWidget(m_dataModel->at(i)));
        m_VericalLayout->addWidget(m_listWgt->at(i));

    }
    QWidget *bottom = new QWidget();
    m_VericalLayout->addWidget(bottom);

    m_VericalLayout->setSpacing(0);
    m_VericalLayout->setMargin(0);
    m_wgtScroll->setLayout(m_VericalLayout);

    scaFAQ->setWidget(m_wgtScroll);

}


#include <QtDebug>
#include <QState>

#include "DapUiMainWindow.h"
#include "DapUiVpnStateIndicator.h"

/**
 * @brief DapUiVpnStateIndicator::DapUiVpnStateIndicator
 * @param a_statesParent
 * @param a_uiComboboxName
 * @param a_uiLabelName
 */
DapUiVpnStateIndicator::DapUiVpnStateIndicator(QState * a_statesParent,  const QString& a_uiComboboxName, const QString& a_uiLabelName, const QString &a_uiIndicator)
    :QObject()
{
    tmrBlinkOn=false;
    m_current = IndicatorState::False;
    m_states = new QState(a_statesParent);
    m_uiComboboxName = a_uiComboboxName;
    m_uiLabelName = a_uiLabelName;
    m_uiIndicator = a_uiIndicator;
    m_state[IndicatorState::True] = new QState(states());
    m_state[IndicatorState::False] = new QState(states());
    m_state[IndicatorState::TrueToFalse] = new QState(states());
    m_state[IndicatorState::FalseToTrue] = new QState(states());

    for (auto const &i: m_state.keys() ) {
         connect( state(i), &QState::entered, [=]{
                QString text = uiLabelTextState(i);
                QString style = uiIndicatorState(i);
                m_current = i;
                if(text.size()>0)
                {
                    DapUiMainWindow::getInstance()->setUiProp(uiLabelName(),"text",text);
                    DapUiMainWindow::getInstance()->setUiProp(uiIndicator(),"styleSheet",style);
                }
                update();
           } );
    }
    tmrBlink.setInterval(500);
    connect(&tmrBlink, &QTimer::timeout,[=]{
        DapUiMainWindow::getInstance()->setUiProp(uiComboboxName(),"checked", (tmrBlinkOn = (!tmrBlinkOn)) );
    });
    m_states->setInitialState(m_state[IndicatorState::False]);
}

void DapUiVpnStateIndicator::update()
{
    switch( current() ){
    case IndicatorState::True: tmrBlink.stop(); DapUiMainWindow::getInstance()->setUiProp(uiComboboxName(),"checked",true); break;
    case IndicatorState::False: tmrBlink.stop(); DapUiMainWindow::getInstance()->setUiProp(uiComboboxName(),"checked",false); break;
    case IndicatorState::TrueToFalse: tmrBlink.start(); break;
    case IndicatorState::FalseToTrue: tmrBlink.start(); break;
    default:
        qWarning() << "No have handler for" << current() << "state";
    }

}

/**
 * @brief DapUiVpnStateIndicator::~DapUiVpnStateIndicator
 */
DapUiVpnStateIndicator::~DapUiVpnStateIndicator()
{
    for(auto a:m_state)
        delete a;
    delete m_states;
}


#ifndef DAPUIVPNSTATEINDICATOR_H
#define DAPUIVPNSTATEINDICATOR_H

#include <QObject>
#include <QState>
#include <QSharedPointer>
#include <QMap>
#include <QTimer>
#include "DapIndicator.h"

class DapUiVpnStateIndicator: public QObject
{
    Q_OBJECT
public:
    using IndicatorState = DapIndicator::State;
private:
    IndicatorState m_current;

    QMap<IndicatorState,QState*> m_state;
    QState * m_states; // Group of states o

    QString m_uiComboboxName;
    QString m_uiLabelName;
    QString m_uiIndicator;

    QTimer tmrBlink;
    bool tmrBlinkOn;

    QMap<IndicatorState, QString> m_uiLabelTextState;
    QMap<IndicatorState, QString> m_uiIndicatorState;
public:
    DapUiVpnStateIndicator(QState * a_statesParent,  const QString& a_uiComboboxName, const QString& a_uiLabelName, const QString& a_uiIndicator);
    ~DapUiVpnStateIndicator();

    IndicatorState current(){ return m_current; }
    bool isTrue(){ return state(IndicatorState::True)->active(); }
    const QString& uiComboboxName() { return m_uiComboboxName; }
    const QString& uiLabelName() { return m_uiLabelName; }
    const QString& uiIndicator() { return m_uiIndicator; }

    const QString& uiLabelTextState(IndicatorState a_is) { return m_uiLabelTextState[a_is]; }
    void setUiLabelTextState(IndicatorState a_is, const QString& a_txt) { m_uiLabelTextState[a_is] = a_txt; }
    
    const QString& uiIndicatorState(IndicatorState a_is) { return m_uiIndicatorState[a_is]; }
    void setUiIndicatorState(IndicatorState a_is, const QString& a_style) { m_uiIndicatorState[a_is] = a_style; }

    QState * state(IndicatorState a_is){ return m_state[a_is]; }
    QState * states() { return m_states; }
    void update();
};
#endif // DAPUIVPNDSTATEINDICATOR_H

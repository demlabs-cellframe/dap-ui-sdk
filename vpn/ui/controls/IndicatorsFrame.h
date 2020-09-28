#ifndef INDICATORSFRAME_H
#define INDICATORSFRAME_H

#include <QFrame>

namespace  Ui{
class IndicatorsFrame;
}
class IndicatorsFrame : public QFrame
{
    Q_OBJECT
public:
    IndicatorsFrame(QWidget* a_parent = nullptr);
    ~IndicatorsFrame();

public slots:
    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);
private:
    Ui::IndicatorsFrame *m_ui;
};

#endif // INDICATORSFRAME_H

#ifndef NAVIGATIONPANEL_H
#define NAVIGATIONPANEL_H

#ifdef ANDROID
    #include "DefaultMultiScreen.h"
    typedef DefaultMultiScreen PanelParentClass;
#else
    #include "NavigationPanelMain.h"
    typedef NavigationPanelMain PanelParentClass;
#endif

class NavigationPanel : public PanelParentClass
{
public:
    NavigationPanel(QWidget *a_parent);

public slots:
    void setBackState(bool a_backState);
    bool backState();

    bool m_backState = true;
};

#endif // NAVIGATIONPANEL_H

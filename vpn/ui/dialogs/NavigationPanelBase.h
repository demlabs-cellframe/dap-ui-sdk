#ifndef NAVIGATIONPANELBASE_H
#define NAVIGATIONPANELBASE_H

#ifdef ANDROID
    #include "DefaultMultiScreen.h"
    typedef DefaultMultiScreen PanelParentClass;
#else
    #include "NavigationPanelMain.h"
    typedef NavigationPanelMain PanelParentClass;
#endif

class NavigationPanelBase : public PanelParentClass
{
public:
    NavigationPanelBase(QWidget *a_parent);

public slots:
    void setBackState(bool a_backState);
    bool backState();

    bool m_backState = true;
};

#endif // NAVIGATIONPANELBASE_H


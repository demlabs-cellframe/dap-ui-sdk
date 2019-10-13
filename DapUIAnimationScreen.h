#ifndef DAPUIANIMATIONSCREEN_H
#define DAPUIANIMATIONSCREEN_H

#include "DapUIAnimationScreenAbstract.h"

/**
 * @brief Class wich hold full screen AnimationChangingWidget and can hold child screens
 */
class DapUIAnimationScreen : public DapUIAnimationScreenAbstract
{
    Q_OBJECT

public:
    DapUIAnimationScreen(QObject *parent, QStackedWidget *a_sw);

protected:
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation);

};



//=======================================================================
//                            Default Form
//=======================================================================

class Ui_AnimationChangingScreen
{
public:
    AnimationChangingWidget *changingScreen;
    QHBoxLayout *horizontalLayout;

    void setupUi(QWidget *a_wgScreen)
    {
        if (a_wgScreen->objectName().isEmpty())
            a_wgScreen->setObjectName(QStringLiteral("AnimationScreenFullScreen"));
 //       a_wgScreen->resize(1280, 804);
        horizontalLayout = new QHBoxLayout(a_wgScreen);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("centralLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        a_wgScreen->setLayout(horizontalLayout);

        changingScreen = new AnimationChangingWidget(a_wgScreen);
        changingScreen->setObjectName(QStringLiteral("changingScreen"));
        changingScreen->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        changingScreen->setStyleSheet("#changingScreen{}");
        horizontalLayout->addWidget(changingScreen);
    } // setupUi

};


#endif // DAPUIANIMATIONSCREEN_H

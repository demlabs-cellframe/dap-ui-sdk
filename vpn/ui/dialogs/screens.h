#ifndef SCREENS_H
#define SCREENS_H

#include "BrandAppProperties.h"
#include "brandScreens.h"

//helper header file for including all scrreens

#ifdef USE_TWO_LAYOUTS
    #include "LoginScreenLayout.h"
    #include "LoginNavigationPanel.h"
#endif

#include "MainScreenLayout.h"

#include "StartScreen.h"
#include "SignUpScreen.h"
#include "SignInScreenSerialNumberBase.h"
#include "NavigationPanelBase.h"
#ifndef USING_SEPARATE_STATISTIC_SCREEN
    #include "MainScreen.h"
#else
    #include "ConnectionScreenBase.h"
    #include "StatisticScreenBase.h"
#endif
#include "SettingsScreen.h"
#include "BugReportWithEmailScreen.h"
#include "BugReportWithoutEmailScreen.h"
#include "BugReportResultScreen.h"
#include "BugReportLoadingScreen.h"
#include "NavigationPanelBack.h"
#include "NavigationPanelMain.h"
#include "NavigationPanelBackBase.h"
#include "MainScreenLayout.h"
#ifdef USING_SEPARATE_ACCOUNT_SCREEN
//#include "AccountScreenBase.h"
#include "AccountScreen.h"
#endif
#include "PasswordRecoveryScreen.h"
#include "PrivacyPolicyScreen.h"
#include "TermsOfUseScreen.h"
#include "ScreenWithCustomPopupsAbstract.h"
#include "ScreenWithScreenPopupsAbstract.h"
#include "ComboBoxPopupScreen.h"
#include "FAQScreen.h"

#ifdef USE_BOTTOM_PANEL
#include "NavigationPanelBottom.h"
#endif


#endif // SCREENS_H

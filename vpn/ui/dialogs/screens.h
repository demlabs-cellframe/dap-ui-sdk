#ifndef SCREENS_H
#define SCREENS_H

#include "BrandAppProperties.h"
#include "brandScreens.h"

//helper header file for including all scrreens

#ifdef ANDROID
#else
    #include "LoginScreenLayout.h"
    #include "LoginNavigationPanel.h"
#endif

#include "MainScreenLayout.h"

#include "StartScreen.h"
#include "SignUpScreen.h"
#include "SignInScreen.h"
#include "NavigationPanelBase.h"
#if !USING_SEPARATE_STATISTIC_SCREEN
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
#include "MainScreenLayout.h"
#include "AccountScreen.h"
#include "PasswordRecoveryScreen.h"
#include "PrivacyPolicyScreen.h"
#include "TermsOfUseScreen.h"
#include "ScreenWithCustomPopupsAbstract.h"
#include "ScreenWithScreenPopupsAbstract.h"
#include "ComboBoxPopupScreen.h"



#endif // SCREENS_H

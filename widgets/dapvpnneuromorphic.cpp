/* INCLUDES */
#include "dapguibuttonplugin.h"
#include "dapguipushbuttonplugin.h"
#include "dapguiradioplugin.h"
#include "dapguicheckboxplugin.h"
#include "dapguigraphicsviewplugin.h"
#include "dapguilabelplugin.h"
#include "dapguilineeditplugin.h"
#include "dapguiplaintexteditplugin.h"
#include "dapguiseparatorplugin.h"
#include "dapguiswitchplugin.h"
#include "dapguitexteditplugin.h"
#include "dapguiwidgetplugin.h"
#include "dapvpnneuromorphic.h"

#include "style/dapguistylemanager.h"
#include "dapguicommon.h"

#include "widgetsdebugsettings.h"
#include <QFontDatabase>

/* VARS */
static WidgetsDebugSettings *s_settings = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapVpnNeuromorphic::DapVpnNeuromorphic (QObject *parent)
  : QObject (parent)
{
  m_widgets.append (new DapGuiButtonPlugin (this));
  m_widgets.append (new DapGuiPushButtonPlugin (this));
  m_widgets.append (new DapGuiRadioPlugin (this));
  m_widgets.append (new DapGuiCheckBoxPlugin (this));
  m_widgets.append (new DapGuiPlainTextEditPlugin (this));
  m_widgets.append (new DapGuiSeparatorPlugin (this));
  m_widgets.append (new DapGuiSwitchPlugin (this));
  m_widgets.append (new DapGuiLineEditPlugin (this));
  m_widgets.append (new DapGuiTextEditPlugin (this));
  m_widgets.append (new DapGuiLabelPlugin (this));
  m_widgets.append (new DapGuiWidgetPlugin (this));
  m_widgets.append (new DapGuiGraphicsViewPlugin (this));

  QFontDatabase::addApplicationFont ("://fonts/Lato-Bold.ttf");
  QFontDatabase::addApplicationFont ("://fonts/Lato-Regular.ttf");
  QFontDatabase::addApplicationFont ("://fonts/Roboto-Bold.ttf");

  auto style = Common::fromFile ("://" DAP_BRAND "/style.css");
  style.replace ("pt", "px");
  DapGuiStyleManager::setupGlobalStyleSheet (style);

  s_settings  = new WidgetsDebugSettings;
  s_settings->show();
}

/********************************************
 * METHODS
 *******************************************/

QList<QDesignerCustomWidgetInterface *> DapVpnNeuromorphic::customWidgets() const
{
  return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2 (kelvpnneuromorphicplugin, KelVpnNeuromorphic)
#endif // QT_VERSION < 0x050000

/*-----------------------------------------*/

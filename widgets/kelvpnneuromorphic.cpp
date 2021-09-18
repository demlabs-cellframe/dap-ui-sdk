/* INCLUDES */
#include "kelguibuttonplugin.h"
#include "kelguipushbuttonplugin.h"
#include "kelguiradioplugin.h"
#include "kelguicheckboxplugin.h"
#include "kelguigraphicsviewplugin.h"
#include "kelguilabelplugin.h"
#include "kelguilineeditplugin.h"
#include "kelguiplaintexteditplugin.h"
#include "kelguiseparatorplugin.h"
#include "kelguitexteditplugin.h"
#include "kelguiwidgetplugin.h"
#include "kelvpnneuromorphic.h"

#include "style/kelguistylemanager.h"
#include "kelguicommon.h"

#include "widgetsdebugsettings.h"

/* VARS */
static WidgetsDebugSettings *s_settings = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelVpnNeuromorphic::KelVpnNeuromorphic (QObject *parent)
  : QObject (parent)
{
  m_widgets.append (new KelGuiButtonPlugin (this));
  m_widgets.append (new KelGuiPushButtonPlugin (this));
  m_widgets.append (new KelGuiRadioPlugin (this));
  m_widgets.append (new KelGuiCheckBoxPlugin (this));
  m_widgets.append (new KelGuiPlainTextEditPlugin (this));
  m_widgets.append (new KelGuiSeparatorPlugin (this));
  m_widgets.append (new KelGuiLineEditPlugin (this));
  m_widgets.append (new KelGuiTextEditPlugin (this));
  m_widgets.append (new KelGuiLabelPlugin (this));
  m_widgets.append (new KelGuiWidgetPlugin (this));
  m_widgets.append (new KelGuiGraphicsViewPlugin (this));

  QFontDatabase::addApplicationFont ("://fonts/Lato-Bold.ttf");
  QFontDatabase::addApplicationFont ("://fonts/Lato-Regular.ttf");
  QFontDatabase::addApplicationFont ("://fonts/Roboto-Bold.ttf");

  auto style = Common::fromFile ("://style/globalstyle.css");
  style.replace ("pt", "px");
  KelGuiStyleManager::setupGlobalStyleSheet (style);

  s_settings  = new WidgetsDebugSettings;
  s_settings->show();
}

/********************************************
 * METHODS
 *******************************************/

QList<QDesignerCustomWidgetInterface *> KelVpnNeuromorphic::customWidgets() const
{
  return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2 (kelvpnneuromorphicplugin, KelVpnNeuromorphic)
#endif // QT_VERSION < 0x050000

/*-----------------------------------------*/

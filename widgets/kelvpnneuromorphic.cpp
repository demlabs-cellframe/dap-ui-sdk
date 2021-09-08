#include "kelguibuttonplugin.h"
#include "kelguipushbuttonplugin.h"
#include "kelguiradioplugin.h"
#include "kelguicheckboxplugin.h"
#include "kelguilabelplugin.h"
#include "kelguiplaintexteditplugin.h"
#include "kelguiseparatorplugin.h"
#include "kelvpnneuromorphic.h"

#include "style/kelguistylemanager.h"
#include "kelguicommon.h"

KelVpnNeuromorphic::KelVpnNeuromorphic (QObject *parent)
  : QObject (parent)
{
  m_widgets.append (new KelGuiButtonPlugin (this));
  m_widgets.append (new KelGuiPushButtonPlugin (this));
  m_widgets.append (new KelGuiRadioPlugin (this));
  m_widgets.append (new KelGuiCheckBoxPlugin (this));
  m_widgets.append (new KelGuiPlainTextEditPlugin (this));
  m_widgets.append (new KelGuiSeparatorPlugin (this));
  m_widgets.append (new KelGuiLabelPlugin (this));

  QFontDatabase::addApplicationFont ("://fonts/Lato-Bold.ttf");
  QFontDatabase::addApplicationFont ("://fonts/Lato-Regular.ttf");
  QFontDatabase::addApplicationFont ("://fonts/Roboto-Bold.ttf");

  auto style = Common::fromFile ("://style/globalstyle.css");
  KelGuiStyleManager::setupGlobalStyleSheet (style, 69.8);
}

QList<QDesignerCustomWidgetInterface *> KelVpnNeuromorphic::customWidgets() const
{
  return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2 (kelvpnneuromorphicplugin, KelVpnNeuromorphic)
#endif // QT_VERSION < 0x050000

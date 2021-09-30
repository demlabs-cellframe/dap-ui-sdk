#include "kelguiswitch.h"
#include "kelguiswitchplugin.h"

#include <QtPlugin>

KelGuiSwitchPlugin::KelGuiSwitchPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiSwitchPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiSwitchPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiSwitchPlugin::createWidget(QWidget *parent)
{
  return new KelGuiSwitch(parent);
}

QString KelGuiSwitchPlugin::name() const
{
  return QLatin1String("KelGuiSwitch");
}

QString KelGuiSwitchPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiSwitchPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/switch.png"));
}

QString KelGuiSwitchPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiSwitchPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiSwitchPlugin::isContainer() const
{
  return false;
}

QString KelGuiSwitchPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiSwitch\" name=\"kelGuiSwitch\">\n</widget>\n");
}

QString KelGuiSwitchPlugin::includeFile() const
{
  return QLatin1String("kelguiswitch.h");
}


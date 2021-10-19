#include "dapguiswitch.h"
#include "dapguiswitchplugin.h"

#include <QtPlugin>

DapGuiSwitchPlugin::DapGuiSwitchPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiSwitchPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiSwitchPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiSwitchPlugin::createWidget(QWidget *parent)
{
  return new DapGuiSwitch(parent);
}

QString DapGuiSwitchPlugin::name() const
{
  return QLatin1String("DapGuiSwitch");
}

QString DapGuiSwitchPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon DapGuiSwitchPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/switch.png"));
}

QString DapGuiSwitchPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiSwitchPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiSwitchPlugin::isContainer() const
{
  return false;
}

QString DapGuiSwitchPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiSwitch\" name=\"kelGuiSwitch\">\n</widget>\n");
}

QString DapGuiSwitchPlugin::includeFile() const
{
  return QLatin1String("dapguiswitch.h");
}


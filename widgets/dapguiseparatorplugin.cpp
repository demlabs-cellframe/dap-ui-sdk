#include "dapguiseparator.h"
#include "dapguiseparatorplugin.h"

#include <QtPlugin>

DapGuiSeparatorPlugin::DapGuiSeparatorPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiSeparatorPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiSeparatorPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiSeparatorPlugin::createWidget(QWidget *parent)
{
  return new DapGuiSeparator(parent);
}

QString DapGuiSeparatorPlugin::name() const
{
  return QLatin1String("DapGuiSeparator");
}

QString DapGuiSeparatorPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon DapGuiSeparatorPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/separator.png"));
}

QString DapGuiSeparatorPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiSeparatorPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiSeparatorPlugin::isContainer() const
{
  return false;
}

QString DapGuiSeparatorPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiSeparator\" name=\"kelGuiSeparator\">\n</widget>\n");
}

QString DapGuiSeparatorPlugin::includeFile() const
{
  return QLatin1String("dapguiseparator.h");
}


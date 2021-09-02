#include "kelguiseparator.h"
#include "kelguiseparatorplugin.h"

#include <QtPlugin>

KelGuiSeparatorPlugin::KelGuiSeparatorPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiSeparatorPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiSeparatorPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiSeparatorPlugin::createWidget(QWidget *parent)
{
  return new KelGuiSeparator(parent);
}

QString KelGuiSeparatorPlugin::name() const
{
  return QLatin1String("KelGuiSeparator");
}

QString KelGuiSeparatorPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiSeparatorPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/separator.png"));
}

QString KelGuiSeparatorPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiSeparatorPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiSeparatorPlugin::isContainer() const
{
  return false;
}

QString KelGuiSeparatorPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiSeparator\" name=\"kelGuiSeparator\">\n</widget>\n");
}

QString KelGuiSeparatorPlugin::includeFile() const
{
  return QLatin1String("kelguiseparator.h");
}


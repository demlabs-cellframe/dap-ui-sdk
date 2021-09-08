#include "kelguiwidget.h"
#include "kelguiwidgetplugin.h"

#include <QtPlugin>

KelGuiWidgetPlugin::KelGuiWidgetPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void KelGuiWidgetPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiWidgetPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiWidgetPlugin::createWidget (QWidget *parent)
{
  return new KelGuiWidget (parent);
}

QString KelGuiWidgetPlugin::name() const
{
  return QLatin1String ("KelGuiWidget");
}

QString KelGuiWidgetPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon KelGuiWidgetPlugin::icon() const
{
  return QIcon();
}

QString KelGuiWidgetPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiWidgetPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiWidgetPlugin::isContainer() const
{
  return false;
}

QString KelGuiWidgetPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"KelGuiWidget\" name=\"kelGuiWidget\">\n</widget>\n");
}

QString KelGuiWidgetPlugin::includeFile() const
{
  return QLatin1String ("kelguiwidget.h");
}


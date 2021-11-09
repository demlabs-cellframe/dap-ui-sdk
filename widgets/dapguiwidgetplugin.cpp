#include "dapguiwidget.h"
#include "dapguiwidgetplugin.h"

#include <QtPlugin>

DapGuiWidgetPlugin::DapGuiWidgetPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void DapGuiWidgetPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiWidgetPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiWidgetPlugin::createWidget (QWidget *parent)
{
  return new DapGuiWidget (parent);
}

QString DapGuiWidgetPlugin::name() const
{
  return QLatin1String ("DapGuiWidget");
}

QString DapGuiWidgetPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon DapGuiWidgetPlugin::icon() const
{
  return QIcon();
}

QString DapGuiWidgetPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiWidgetPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiWidgetPlugin::isContainer() const
{
  return true;
}

QString DapGuiWidgetPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"DapGuiWidget\" name=\"kelGuiWidget\">\n</widget>\n");
}

QString DapGuiWidgetPlugin::includeFile() const
{
  return QLatin1String ("dapguiwidget.h");
}


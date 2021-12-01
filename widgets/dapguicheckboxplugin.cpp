#include "dapguicheckbox.h"
#include "dapguicheckboxplugin.h"

#include <QtPlugin>

DapGuiCheckBoxPlugin::DapGuiCheckBoxPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiCheckBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiCheckBoxPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiCheckBoxPlugin::createWidget(QWidget *parent)
{
  return new DapGuiCheckBox(parent);
}

QString DapGuiCheckBoxPlugin::name() const
{
  return QLatin1String("DapGuiCheckBox");
}

QString DapGuiCheckBoxPlugin::group() const
{
  return QLatin1String(DAP_BRAND "-Neuromorphic");
}

QIcon DapGuiCheckBoxPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/check.png"));
}

QString DapGuiCheckBoxPlugin::toolTip() const
{
  return QLatin1String(DAP_BRAND "-Neuromorphic GUI Element");
}

QString DapGuiCheckBoxPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for " DAP_BRAND "-Neuromorphic");
}

bool DapGuiCheckBoxPlugin::isContainer() const
{
  return false;
}

QString DapGuiCheckBoxPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiCheckBox\" name=\"kelGuiCheckBox\">\n</widget>\n");
}

QString DapGuiCheckBoxPlugin::includeFile() const
{
  return QLatin1String("dapguicheckbox.h");
}


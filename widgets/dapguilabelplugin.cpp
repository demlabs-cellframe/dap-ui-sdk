#include "dapguilabel.h"
#include "dapguilabelplugin.h"

#include <QtPlugin>

DapGuiLabelPlugin::DapGuiLabelPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void DapGuiLabelPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiLabelPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiLabelPlugin::createWidget (QWidget *parent)
{
  return new DapGuiLabel (parent);
}

QString DapGuiLabelPlugin::name() const
{
  return QLatin1String ("DapGuiLabel");
}

QString DapGuiLabelPlugin::group() const
{
  return QLatin1String (DAP_BRAND "-Neuromorphic");
}

QIcon DapGuiLabelPlugin::icon() const
{
  return QIcon();
}

QString DapGuiLabelPlugin::toolTip() const
{
  return QLatin1String (DAP_BRAND "-Neuromorphic GUI Element");
}

QString DapGuiLabelPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for " DAP_BRAND "-Neuromorphic");
}

bool DapGuiLabelPlugin::isContainer() const
{
  return false;
}

QString DapGuiLabelPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"DapGuiLabel\" name=\"kelGuiLabel\">\n</widget>\n");
}

QString DapGuiLabelPlugin::includeFile() const
{
  return QLatin1String ("dapguilabel.h");
}


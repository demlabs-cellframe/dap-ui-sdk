#include "dapguilineedit.h"
#include "dapguilineeditplugin.h"

#include <QtPlugin>

DapGuiLineEditPlugin::DapGuiLineEditPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void DapGuiLineEditPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiLineEditPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiLineEditPlugin::createWidget (QWidget *parent)
{
  return new DapGuiSerialKeyLineEdit (parent);
}

QString DapGuiLineEditPlugin::name() const
{
  return QLatin1String ("DapGuiLineEdit");
}

QString DapGuiLineEditPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon DapGuiLineEditPlugin::icon() const
{
  return QIcon();
}

QString DapGuiLineEditPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiLineEditPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiLineEditPlugin::isContainer() const
{
  return false;
}

QString DapGuiLineEditPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"DapGuiLineEdit\" name=\"kelGuiLineEdit\">\n</widget>\n");
}

QString DapGuiLineEditPlugin::includeFile() const
{
  return QLatin1String ("dapguilineedit.h");
}


#include "dapguitextedit.h"
#include "dapguitexteditplugin.h"

#include <QtPlugin>

DapGuiTextEditPlugin::DapGuiTextEditPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void DapGuiTextEditPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiTextEditPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiTextEditPlugin::createWidget (QWidget *parent)
{
  return new DapGuiTextEdit (parent);
}

QString DapGuiTextEditPlugin::name() const
{
  return QLatin1String ("DapGuiTextEdit");
}

QString DapGuiTextEditPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon DapGuiTextEditPlugin::icon() const
{
  return QIcon();
}

QString DapGuiTextEditPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiTextEditPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiTextEditPlugin::isContainer() const
{
  return false;
}

QString DapGuiTextEditPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"DapGuiTextEdit\" name=\"kelGuiTextEdit\">\n</widget>\n");
}

QString DapGuiTextEditPlugin::includeFile() const
{
  return QLatin1String ("dapguitextedit.h");
}


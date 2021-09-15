#include "kelguitextedit.h"
#include "kelguitexteditplugin.h"

#include <QtPlugin>

KelGuiTextEditPlugin::KelGuiTextEditPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void KelGuiTextEditPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiTextEditPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiTextEditPlugin::createWidget (QWidget *parent)
{
  return new KelGuiTextEdit (parent);
}

QString KelGuiTextEditPlugin::name() const
{
  return QLatin1String ("KelGuiTextEdit");
}

QString KelGuiTextEditPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon KelGuiTextEditPlugin::icon() const
{
  return QIcon();
}

QString KelGuiTextEditPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiTextEditPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiTextEditPlugin::isContainer() const
{
  return false;
}

QString KelGuiTextEditPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"KelGuiTextEdit\" name=\"kelGuiTextEdit\">\n</widget>\n");
}

QString KelGuiTextEditPlugin::includeFile() const
{
  return QLatin1String ("kelguitextedit.h");
}


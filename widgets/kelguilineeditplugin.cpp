#include "kelguilineedit.h"
#include "kelguilineeditplugin.h"

#include <QtPlugin>

KelGuiLineEditPlugin::KelGuiLineEditPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void KelGuiLineEditPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiLineEditPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiLineEditPlugin::createWidget (QWidget *parent)
{
  return new KelGuiLineEdit (parent);
}

QString KelGuiLineEditPlugin::name() const
{
  return QLatin1String ("KelGuiLineEdit");
}

QString KelGuiLineEditPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon KelGuiLineEditPlugin::icon() const
{
  return QIcon();
}

QString KelGuiLineEditPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiLineEditPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiLineEditPlugin::isContainer() const
{
  return false;
}

QString KelGuiLineEditPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"KelGuiLineEdit\" name=\"kelGuiLineEdit\">\n</widget>\n");
}

QString KelGuiLineEditPlugin::includeFile() const
{
  return QLatin1String ("kelguilineedit.h");
}


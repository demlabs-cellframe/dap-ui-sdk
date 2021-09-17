#include "kelguilabel.h"
#include "kelguilabelplugin.h"

#include <QtPlugin>

KelGuiLabelPlugin::KelGuiLabelPlugin (QObject *parent)
  : QObject (parent)
{
  m_initialized = false;
}

void KelGuiLabelPlugin::initialize (QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiLabelPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiLabelPlugin::createWidget (QWidget *parent)
{
  return new KelGuiLabel (parent);
}

QString KelGuiLabelPlugin::name() const
{
  return QLatin1String ("KelGuiLabel");
}

QString KelGuiLabelPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon KelGuiLabelPlugin::icon() const
{
  return QIcon();
}

QString KelGuiLabelPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiLabelPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiLabelPlugin::isContainer() const
{
  return false;
}

QString KelGuiLabelPlugin::domXml() const
{
  return QLatin1String ("<widget class=\"KelGuiLabel\" name=\"kelGuiLabel\">\n</widget>\n");
}

QString KelGuiLabelPlugin::includeFile() const
{
  return QLatin1String ("kelguilabel.h");
}


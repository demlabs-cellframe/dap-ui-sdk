#include "kelguicheckbox.h"
#include "kelguicheckboxplugin.h"

#include <QtPlugin>

KelGuiCheckBoxPlugin::KelGuiCheckBoxPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiCheckBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiCheckBoxPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiCheckBoxPlugin::createWidget(QWidget *parent)
{
  return new KelGuiCheckBox(parent);
}

QString KelGuiCheckBoxPlugin::name() const
{
  return QLatin1String("KelGuiCheckBox");
}

QString KelGuiCheckBoxPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiCheckBoxPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/check.png"));
}

QString KelGuiCheckBoxPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiCheckBoxPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiCheckBoxPlugin::isContainer() const
{
  return false;
}

QString KelGuiCheckBoxPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiCheckBox\" name=\"kelGuiCheckBox\">\n</widget>\n");
}

QString KelGuiCheckBoxPlugin::includeFile() const
{
  return QLatin1String("kelguicheckbox.h");
}


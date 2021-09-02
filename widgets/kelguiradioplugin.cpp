#include "kelguiradio.h"
#include "kelguiradioplugin.h"

#include <QtPlugin>

KelGuiRadioPlugin::KelGuiRadioPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiRadioPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiRadioPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiRadioPlugin::createWidget(QWidget *parent)
{
  return new KelGuiRadio(parent);
}

QString KelGuiRadioPlugin::name() const
{
  return QLatin1String("KelGuiRadio");
}

QString KelGuiRadioPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiRadioPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/radio.png"));
}

QString KelGuiRadioPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiRadioPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiRadioPlugin::isContainer() const
{
  return false;
}

QString KelGuiRadioPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiRadio\" name=\"kelGuiRadio\">\n</widget>\n");
}

QString KelGuiRadioPlugin::includeFile() const
{
  return QLatin1String("kelguiradio.h");
}


#include "dapguiradio.h"
#include "dapguiradioplugin.h"

#include <QtPlugin>

DapGuiRadioPlugin::DapGuiRadioPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiRadioPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiRadioPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiRadioPlugin::createWidget(QWidget *parent)
{
  return new DapGuiRadio(parent);
}

QString DapGuiRadioPlugin::name() const
{
  return QLatin1String("DapGuiRadio");
}

QString DapGuiRadioPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon DapGuiRadioPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/radio.png"));
}

QString DapGuiRadioPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiRadioPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiRadioPlugin::isContainer() const
{
  return false;
}

QString DapGuiRadioPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiRadio\" name=\"kelGuiRadio\">\n</widget>\n");
}

QString DapGuiRadioPlugin::includeFile() const
{
  return QLatin1String("dapguiradio.h");
}


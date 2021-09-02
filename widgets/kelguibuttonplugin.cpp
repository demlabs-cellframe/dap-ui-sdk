#include "kelguibutton.h"
#include "kelguibuttonplugin.h"

#include <QtPlugin>
#include <QFontDatabase>

KelGuiButtonPlugin::KelGuiButtonPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiButtonPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiButtonPlugin::createWidget(QWidget *parent)
{
  return new KelGuiButton(parent);
}

QString KelGuiButtonPlugin::name() const
{
  return QLatin1String("KelGuiButton");
}

QString KelGuiButtonPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiButtonPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/link.png"));
}

QString KelGuiButtonPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiButtonPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiButtonPlugin::isContainer() const
{
  return false;
}

QString KelGuiButtonPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiButton\" name=\"kelGuiButton\">\n</widget>\n");
}

QString KelGuiButtonPlugin::includeFile() const
{
  return QLatin1String("kelguibutton.h");
}


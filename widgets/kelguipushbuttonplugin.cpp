#include "kelguipushbutton.h"
#include "kelguipushbuttonplugin.h"

#include <QtPlugin>
#include <QFontDatabase>

KelGuiPushButtonPlugin::KelGuiPushButtonPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiPushButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiPushButtonPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiPushButtonPlugin::createWidget(QWidget *parent)
{
  return new KelGuiPushButton(parent);
}

QString KelGuiPushButtonPlugin::name() const
{
  return QLatin1String("KelGuiPushButton");
}

QString KelGuiPushButtonPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiPushButtonPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/button.png"));
}

QString KelGuiPushButtonPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiPushButtonPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiPushButtonPlugin::isContainer() const
{
  return false;
}

QString KelGuiPushButtonPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiPushButton\" name=\"kelGuiPushButton\">\n</widget>\n");
}

QString KelGuiPushButtonPlugin::includeFile() const
{
  return QLatin1String("kelguipushbutton.h");
}


#include "dapguipushbutton.h"
#include "dapguipushbuttonplugin.h"

#include <QtPlugin>
#include <QFontDatabase>

DapGuiPushButtonPlugin::DapGuiPushButtonPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiPushButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiPushButtonPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiPushButtonPlugin::createWidget(QWidget *parent)
{
  return new DapGuiPushButton(parent);
}

QString DapGuiPushButtonPlugin::name() const
{
  return QLatin1String("DapGuiPushButton");
}

QString DapGuiPushButtonPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon DapGuiPushButtonPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/button.png"));
}

QString DapGuiPushButtonPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiPushButtonPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiPushButtonPlugin::isContainer() const
{
  return false;
}

QString DapGuiPushButtonPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiPushButton\" name=\"kelGuiPushButton\">\n</widget>\n");
}

QString DapGuiPushButtonPlugin::includeFile() const
{
  return QLatin1String("dapguipushbutton.h");
}


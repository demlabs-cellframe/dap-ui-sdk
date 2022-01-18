#include "dapguibutton.h"
#include "dapguibuttonplugin.h"

#include <QtPlugin>
#include <QFontDatabase>

DapGuiButtonPlugin::DapGuiButtonPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiButtonPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiButtonPlugin::createWidget(QWidget *parent)
{
  return new DapGuiButton(parent);
}

QString DapGuiButtonPlugin::name() const
{
  return QLatin1String("DapGuiButton");
}

QString DapGuiButtonPlugin::group() const
{
  return QLatin1String ("KelVPN-Neuromorphic");
}

QIcon DapGuiButtonPlugin::icon() const
{
  return QIcon(QLatin1String("://icons/link.png"));
}

QString DapGuiButtonPlugin::toolTip() const
{
  return QLatin1String ("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiButtonPlugin::whatsThis() const
{
  return QLatin1String ("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiButtonPlugin::isContainer() const
{
  return false;
}

QString DapGuiButtonPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiButton\" name=\"kelGuiButton\">\n</widget>\n");
}

QString DapGuiButtonPlugin::includeFile() const
{
  return QLatin1String("dapguibutton.h");
}


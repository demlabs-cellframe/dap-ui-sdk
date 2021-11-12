#include "dapguigraphicsview.h"
#include "dapguigraphicsviewplugin.h"

#include <QtPlugin>

DapGuiGraphicsViewPlugin::DapGuiGraphicsViewPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiGraphicsViewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiGraphicsViewPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiGraphicsViewPlugin::createWidget(QWidget *parent)
{
  return new DapGuiGraphicsView(parent);
}

QString DapGuiGraphicsViewPlugin::name() const
{
  return QLatin1String("DapGuiGraphicsView");
}

QString DapGuiGraphicsViewPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon DapGuiGraphicsViewPlugin::icon() const
{
  return QIcon();
}

QString DapGuiGraphicsViewPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiGraphicsViewPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiGraphicsViewPlugin::isContainer() const
{
  return false;
}

QString DapGuiGraphicsViewPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiGraphicsView\" name=\"kelGuiGraphicsView\">\n</widget>\n");
}

QString DapGuiGraphicsViewPlugin::includeFile() const
{
  return QLatin1String("dapguigraphicsview.h");
}


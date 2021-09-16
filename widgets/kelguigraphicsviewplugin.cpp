#include "kelguigraphicsview.h"
#include "kelguigraphicsviewplugin.h"

#include <QtPlugin>

KelGuiGraphicsViewPlugin::KelGuiGraphicsViewPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiGraphicsViewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiGraphicsViewPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiGraphicsViewPlugin::createWidget(QWidget *parent)
{
  return new KelGuiGraphicsView(parent);
}

QString KelGuiGraphicsViewPlugin::name() const
{
  return QLatin1String("KelGuiGraphicsView");
}

QString KelGuiGraphicsViewPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiGraphicsViewPlugin::icon() const
{
  return QIcon();
}

QString KelGuiGraphicsViewPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiGraphicsViewPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiGraphicsViewPlugin::isContainer() const
{
  return false;
}

QString KelGuiGraphicsViewPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiGraphicsView\" name=\"kelGuiGraphicsView\">\n</widget>\n");
}

QString KelGuiGraphicsViewPlugin::includeFile() const
{
  return QLatin1String("kelguigraphicsview.h");
}


#include "kelguiplaintextedit.h"
#include "kelguiplaintexteditplugin.h"

#include <QtPlugin>

KelGuiPlainTextEditPlugin::KelGuiPlainTextEditPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void KelGuiPlainTextEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool KelGuiPlainTextEditPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *KelGuiPlainTextEditPlugin::createWidget(QWidget *parent)
{
  return new KelGuiPlainTextEdit(parent);
}

QString KelGuiPlainTextEditPlugin::name() const
{
  return QLatin1String("KelGuiPlainTextEdit");
}

QString KelGuiPlainTextEditPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon KelGuiPlainTextEditPlugin::icon() const
{
  return QIcon();
}

QString KelGuiPlainTextEditPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString KelGuiPlainTextEditPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool KelGuiPlainTextEditPlugin::isContainer() const
{
  return false;
}

QString KelGuiPlainTextEditPlugin::domXml() const
{
  return QLatin1String("<widget class=\"KelGuiPlainTextEdit\" name=\"kelGuiPlainTextEdit\">\n</widget>\n");
}

QString KelGuiPlainTextEditPlugin::includeFile() const
{
  return QLatin1String("kelguiplaintextedit.h");
}


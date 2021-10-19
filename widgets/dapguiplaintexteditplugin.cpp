#include "dapguiplaintextedit.h"
#include "dapguiplaintexteditplugin.h"

#include <QtPlugin>

DapGuiPlainTextEditPlugin::DapGuiPlainTextEditPlugin(QObject *parent)
  : QObject(parent)
{
  m_initialized = false;
}

void DapGuiPlainTextEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (m_initialized)
    return;

  // Add extension registrations, etc. here

  m_initialized = true;
}

bool DapGuiPlainTextEditPlugin::isInitialized() const
{
  return m_initialized;
}

QWidget *DapGuiPlainTextEditPlugin::createWidget(QWidget *parent)
{
  return new DapGuiPlainTextEdit(parent);
}

QString DapGuiPlainTextEditPlugin::name() const
{
  return QLatin1String("DapGuiPlainTextEdit");
}

QString DapGuiPlainTextEditPlugin::group() const
{
  return QLatin1String("KelVPN-Neuromorphic");
}

QIcon DapGuiPlainTextEditPlugin::icon() const
{
  return QIcon();
}

QString DapGuiPlainTextEditPlugin::toolTip() const
{
  return QLatin1String("KelVPN-Neuromorphic GUI Element");
}

QString DapGuiPlainTextEditPlugin::whatsThis() const
{
  return QLatin1String("Custom QWidget for KelVPN-Neuromorphic");
}

bool DapGuiPlainTextEditPlugin::isContainer() const
{
  return false;
}

QString DapGuiPlainTextEditPlugin::domXml() const
{
  return QLatin1String("<widget class=\"DapGuiPlainTextEdit\" name=\"kelGuiPlainTextEdit\">\n</widget>\n");
}

QString DapGuiPlainTextEditPlugin::includeFile() const
{
  return QLatin1String("dapguiplaintextedit.h");
}


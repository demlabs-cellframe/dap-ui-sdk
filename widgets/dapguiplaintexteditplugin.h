#ifndef DAPGUIPLAINTEXTEDITPLUGIN_H
#define DAPGUIPLAINTEXTEDITPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class DapGuiPlainTextEditPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)


public:
  DapGuiPlainTextEditPlugin(QObject *parent = 0);

  bool isContainer() const;
  bool isInitialized() const;
  QIcon icon() const;
  QString domXml() const;
  QString group() const;
  QString includeFile() const;
  QString name() const;
  QString toolTip() const;
  QString whatsThis() const;
  QWidget *createWidget(QWidget *parent);
  void initialize(QDesignerFormEditorInterface *core);

private:
  bool m_initialized;
};

#endif // DAPGUIPLAINTEXTEDITPLUGIN_H

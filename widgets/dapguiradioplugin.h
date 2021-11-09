#ifndef DAPGUIRADIOPLUGIN_H
#define DAPGUIRADIOPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class DapGuiRadioPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)


public:
  DapGuiRadioPlugin(QObject *parent = 0);

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

#endif // DAPGUIRADIOPLUGIN_H

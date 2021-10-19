/* INCLUDES */
#include "widgetsdebugsettings.h"
#include "ui_widgetsdebugsettings.h"
#include "style/dapguistylemanager.h"
#include "dapguicommon.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include <QApplication>
#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QCloseEvent>

QMainWindow *getMainWindow()
{
  foreach (QWidget *w, qApp->topLevelWidgets())
    if (QMainWindow *mainWin = qobject_cast<QMainWindow *> (w))
      return mainWin;
  return nullptr;
}

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

WidgetsDebugSettings::WidgetsDebugSettings (QWidget *parent) :
  QWidget (parent),
  ui (new Ui::WidgetsDebugSettings)
{
  /* setup ui */
  ui->setupUi (this);
  setWindowFlags (windowFlags() & (~Qt::WindowMaximizeButtonHint));

  /* read last settings */
  QSettings s ("/tmp/KelVpnNeuromorphic.ini", QSettings::IniFormat);
  m_globalStyleFilename = s.value ("globalstyle").toString();
  ui->editGlobalStyle->setText (m_globalStyleFilename);

  /* get main window and menu */
  auto mw   = getMainWindow();
  auto menu = mw->menuBar()->addMenu ("..::DapGui Custom Widgets::..");

  /* create docker */
  QDockWidget *dock = new QDockWidget ("..::DapGui Custom Widgets Style Settings::..", mw);
  dock->setWidget (this);

  /* create menu */
  QAction *action = dock->toggleViewAction();
  action->setText ("Show Style Settings");
  menu->addAction (action);

  /* attach to main window */
  mw->addDockWidget (Qt::BottomDockWidgetArea, dock);
}

WidgetsDebugSettings::~WidgetsDebugSettings()
{
  delete ui;
}

/********************************************
 * OVERRIDE
 *******************************************/

void WidgetsDebugSettings::closeEvent (QCloseEvent *event)
{
  showMinimized();
  event->ignore();
}

/********************************************
 * SLOTS
 *******************************************/

void WidgetsDebugSettings::on_btnChooseGlobalStyle_clicked()
{
  auto filename =
    QFileDialog::getOpenFileName (
      this, tr ("Open Global Style CSS"),
      m_globalStyleFilename.isEmpty() ? "/home" : m_globalStyleFilename,
      tr ("CSS Style (*.css)"));

  if (filename.isEmpty())
    return;

  m_globalStyleFilename = filename;
  ui->editGlobalStyle->setText (filename);

  QSettings s ("/tmp/KelVpnNeuromorphic.ini", QSettings::IniFormat);
  s.setValue ("globalstyle", filename);
}

void WidgetsDebugSettings::on_btnUpdateGlobalStyle_clicked()
{
  auto style = Common::fromFile (m_globalStyleFilename);

  if (style.isEmpty())
    {
      QMessageBox::warning (this, __PRETTY_FUNCTION__, "Unable to load file!");
      return;
    }

  style.replace ("pt", "px");
  DapGuiStyleManager::setupGlobalStyleSheet (style);
}

/*-----------------------------------------*/

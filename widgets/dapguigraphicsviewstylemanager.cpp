/* INCLUDES */
#include "dapguigraphicsviewstylemanager.h"
#include "dapguigraphicsview.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiGraphicsViewStyleManager::DapGuiGraphicsViewStyleManager (QWidget *parent)
  : DapGuiStyleManager (parent)
{
  /* connect to additional slot */
  if (this != &_signal())
    {
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiGraphicsViewStyleManager::forcedGraphicsViewStyleUpdate);
    }
}

DapGuiGraphicsViewStyleManager::DapGuiGraphicsViewStyleManager (DapGuiGraphicsViewStyleManager &&src)
  : DapGuiStyleManager ((DapGuiStyleManager &&)src)
{
  /* connect to additional slot */
  if (this != &_signal())
    {
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiGraphicsViewStyleManager::forcedGraphicsViewStyleUpdate);
    }
}

DapGuiGraphicsViewStyleManager::~DapGuiGraphicsViewStyleManager()
{

}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void DapGuiGraphicsViewStyleManager::forcedGraphicsViewStyleUpdate()
{
  /* get push button ptr */
  DapGuiGraphicsView *gv  = dynamic_cast<DapGuiGraphicsView *> (m_widget);
  QGraphicsScene *scene   = gv->scene();
  if (!gv || !scene)
    return;

  /* color */
  struct ColorStruct
  {
    const char
      *background,
      *download,
      *upload;
  };
  static QMap<QString, ColorStruct> mapColor =
  {
    { "light", {"#f7f8fa", "#D61F5D", "#2D1FD6"} },
    { "dark",  {"#28264d", "#FF22A2", "#9F83FF"} },
  };
  auto dir    = themeDir();
  auto color  = mapColor.value (dir, mapColor.first());
  gv->form()->setProperty ("colorBackground",     color.background);
  gv->form()->setProperty ("colorChartDownload",  color.download);
  gv->form()->setProperty ("colorChartUpload",    color.upload);

  auto size   = gv->size();
  auto rect   = scene->sceneRect();
  rect.setSize (size);
  scene->setSceneRect (rect);
  scene->addRect (rect, QPen(Qt::transparent), QBrush (color.background));
}

/*-----------------------------------------*/

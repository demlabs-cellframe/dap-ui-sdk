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

  auto size   = gv->size();
  auto rect   = scene->sceneRect();
  rect.setSize (size);
  scene->setSceneRect (rect);
}

/*-----------------------------------------*/

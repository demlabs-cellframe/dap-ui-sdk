/* INCLUDES */
#include "kelguigraphicsviewstylemanager.h"
#include "kelguigraphicsview.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiGraphicsViewStyleManager::KelGuiGraphicsViewStyleManager (QWidget *parent)
  : KelGuiStyleManager (parent)
{
  /* connect to additional slot */
  if (this != &_signal())
    {
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiGraphicsViewStyleManager::forcedGraphicsViewStyleUpdate);
    }
}

KelGuiGraphicsViewStyleManager::KelGuiGraphicsViewStyleManager (KelGuiGraphicsViewStyleManager &&src)
  : KelGuiStyleManager ((KelGuiStyleManager &&)src)
{
  /* connect to additional slot */
  if (this != &_signal())
    {
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiGraphicsViewStyleManager::forcedGraphicsViewStyleUpdate);
    }
}

KelGuiGraphicsViewStyleManager::~KelGuiGraphicsViewStyleManager()
{

}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void KelGuiGraphicsViewStyleManager::forcedGraphicsViewStyleUpdate()
{
  /* get push button ptr */
  KelGuiGraphicsView *gv  = dynamic_cast<KelGuiGraphicsView *> (m_widget);
  QGraphicsScene *scene   = gv->scene();
  if (!gv || !scene)
    return;

  auto size   = gv->size();
  auto rect   = scene->sceneRect();
  rect.setSize (size);
  scene->setSceneRect (rect);
}

/*-----------------------------------------*/

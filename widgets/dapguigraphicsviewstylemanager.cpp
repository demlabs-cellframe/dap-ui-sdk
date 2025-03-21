/* INCLUDES */
#include "dapguigraphicsviewstylemanager.h"
#include "dapguigraphicsview.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

QString fetchColor(const QString &style, const QString key)
{
    QString a_style(style);
    a_style = a_style.remove(" ");
    QString regExp(key + ":([#0-9a-fA-F]+);");
    int a = a_style.indexOf(QRegularExpression(regExp));
    if (a < 0)
        return nullptr;
    int b = a_style.indexOf(QRegularExpression(":"), a) + 1;
    int c = a_style.indexOf(QRegularExpression(";"), a);
    return a_style.mid(b, c-b);
}

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

  auto style = styleByClassName ("graphics");

  /* color */
  struct ColorStruct
  {
    QString
      background,
      download,
      upload;
  };
  static QMap<QString, ColorStruct> mapColor =
  // default value
  {
    { "light", {"#f7f8fa", "#D61F5D", "#2D1FD6"} },
    { "dark",  {"#2f2e42", "#FF22A2", "#9F83FF"} },
  };
  auto dir    = themeDir();
  auto color  = mapColor.value (dir, mapColor.first());
  if (style.contains("colorBackground"))
      color.background = fetchColor(style, "colorBackground");
  if (style.contains("colorChartDownload"))
      color.download = fetchColor(style, "colorChartDownload");
  if (style.contains("colorChartUpload"))
      color.upload = fetchColor(style, "colorChartUpload");
  gv->form()->setProperty ("colorBackground",     color.background);
  gv->form()->setProperty ("colorChartDownload",  color.download);
  gv->form()->setProperty ("colorChartUpload",    color.upload);

  auto size   = gv->size();
  auto rect   = scene->sceneRect();
  rect.setSize (size);
  scene->setSceneRect (rect);
  scene->addRect (rect, QPen(Qt::transparent), QBrush (qPrintable(color.background)));
}

/*-----------------------------------------*/

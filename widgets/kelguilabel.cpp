/* INCLUDES */
#include "kelguilabel.h"
#include "kelguicommon.h"

#include <QMutex>
#include <QMap>
#include <QMouseEvent>

/* DEFS */

/****************************************//**
 * @brief class handles css styles as map
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class _LabelStyleManager
{
  friend class Operation;

  /* VARS */
private:
  QMutex m_mutex;
  QMap<QString, QString> m_styleMap;

  /* DEFS */
public:
  /// mutex locker for map
  class Operation : public QMutexLocker
  {
  public:
    Operation()
      : QMutexLocker (&_LabelStyleManager::instance()->m_mutex)
    {}

    QString operator[] (const QString &className)
    {
      QString value = _LabelStyleManager::instance()->m_styleMap.value (className);
      return value.replace ('{', "").replace ('}', "").replace ('\n', "").replace ('\t', "");
    }

    /// split classes by 'space' and collect all styles as one string
    QString compile (const QString &classNameList)
    {
      QString result;
      QStringList list = classNameList.split (' ');
      foreach (auto &cs, list)
        result += operator[] (cs);
      return result;
    }
  };

  /* CONSTRUCT */
private:
  _LabelStyleManager()
  {
    auto style = Common::fromFile ("://styles/buttonlabel.css");
    auto list  = style.split ('.');
    //auto inst  = _LabelStyleManager::instance();
    foreach (auto &item, list)
      {
        auto index  = item.indexOf ('\n');
        if (index == -1)
          continue;
        auto name   = item.mid (0, index);
        auto body   = item.mid (index);
        m_styleMap.insert (name, body);
      }
  }

  /* METHODS */
private:
  static _LabelStyleManager *instance()
  {
    static _LabelStyleManager *lsm;
    if (lsm == nullptr)
      lsm = new _LabelStyleManager;
    return lsm;
  }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiLabel::KelGuiLabel (QWidget *parent)
  : QLabel (parent)
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiLabel::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();
}

/*-----------------------------------------*/

/* INCLUDES */
#include "dapqmlmodelfaq.h"

/* DEFS */
enum Role
{
  Question  = Qt::UserRole,
  Answer,
};

struct Item
{
  const char *q;
  const char *a;
};

/* VARS */
static DapQmlModelFaq *__inst = nullptr;
static const QVector<Item> s_items =
{
  {
    "Question",
    "Is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived"
  },
  {
    "Question",
    "Is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived"
  },
  {
    "Question",
    "Is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived"
  },
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelFaq::DapQmlModelFaq()
  : QAbstractListModel (nullptr)
{
  /* vars */
  __inst  = this;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelFaq *DapQmlModelFaq::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelFaq;
  return __inst;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelFaq::rowCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent)
  return s_items.size();
}

QVariant DapQmlModelFaq::data (const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= s_items.size())
    return QVariant();

  switch (role)
    {
    case Role::Question:  return QString (s_items.at (index.row()).q);
    case Role::Answer:    return QString (s_items.at (index.row()).a);
    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelFaq::roleNames() const
{
  static QHash<int, QByteArray> names;

  if (names.isEmpty())
    {
      names.insert (Role::Question, "question");
      names.insert (Role::Answer,   "answer");
    }

  return names;
}

/*-----------------------------------------*/

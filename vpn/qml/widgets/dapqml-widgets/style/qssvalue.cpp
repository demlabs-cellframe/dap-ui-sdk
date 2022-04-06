/* INCLUDES */
#include "qssvalue.h"
#include "qsslink.h"
#include "scaled.h"
#include <QColor>
#include <QUrl>
#include <QFont>
#include <QMetaEnum>

/* NAMESPACE */
namespace Style
{

/* VARS */
static QMap<QString, int> s_fontWeigthMap =
{
  {"Thin", 0},
  {"ExtraLight", 12},
  {"Light", 25},
  {"Normal", 50},
  {"Medium", 57},
  {"DemiBold", 63},
  {"Bold", 75},
  {"ExtraBold", 81},
  {"Black", 87},
};

/* LINKS */
void removeMinuses (QString &a_value);
static void removeQproperty (QString &a_value);
static void removeBeginSpaces (QString &a_value);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

QssValue::QssValue (const QString &a_src)
{
  /* split to field and value */
  auto sep    = a_src.indexOf(':');
  auto parts  = QStringList {a_src.mid (0, sep), a_src.mid (sep + 1)};

  /* skip invalid */
  if (parts.size() != 2)
    return;

  /* store */
  m_field = parts.at (0);
  m_value = parts.at (1);

  /* process value */
  removeBeginSpaces (m_field);
  removeBeginSpaces (m_value);
  removeQproperty (m_field);
  removeMinuses (m_field);
}

/********************************************
 * STATIC METHODS
 *******************************************/

void removeMinuses (QString &a_value)
{
  for (int i = 0; i < a_value.size(); i++)
    {
      if (a_value[i] == '-' && i + 1 < a_value.size())
        {
          a_value.remove (i, 1);
          a_value[i]  = a_value[i].toUpper();
        }
    }
}

void removeQproperty (QString &a_value)
{
  a_value = a_value.remove("qproperty-");
}

void removeBeginSpaces (QString &a_value)
{
  int counter = 0;
  for (auto i = a_value.cbegin(), e = a_value.cend(); i != e ; i++)
    if (*i == ' ')
      counter++;
  a_value = a_value.remove (0, counter);
}

static QColor rgba (const QString &a_value)
{
  int firstBracket  = a_value.indexOf ('(') + 1,
      secondBracket = a_value.indexOf (')');

  auto data         = a_value.mid (firstBracket, secondBracket - firstBracket);
  auto valuesStr    = data.split (',');

  if (valuesStr.size() != 4)
    return QColor();

  double values[4] =
  {
    valuesStr.at (0).toDouble(),
    valuesStr.at (1).toDouble(),
    valuesStr.at (2).toDouble(),
    valuesStr.at (3).toDouble(),
  };

  return QColor::fromRgba (
           qRgba (
             qRed (values[0]),
             qGreen (values[1]),
             qBlue (values[2]),
             qAlpha (values[3])));
}

static QUrl url (const QString &a_value)
{
  int firstBracket  = a_value.indexOf ('(') + 1,
      secondBracket = a_value.indexOf (')');

  auto value        = a_value.mid (firstBracket, secondBracket - firstBracket);

  if (value.at(0) == '\"' && value.at(value.size()-1) == '\"')
    value           = value.mid (1, value.size() - 2);

  return QUrl (value);
}

static int font (const QString &a_value)
{
  auto value  = a_value.mid (5);
  auto weight = s_fontWeigthMap.value(value, 50);
  return weight;
}

static int alignment (const QString &a_value)
{
  auto value      = a_value.mid (5);
  auto metaEnum   = QMetaEnum::fromType<Qt::Alignment>();
  bool ok;
  auto result     = metaEnum.keyToValue (value.toStdString().c_str(), &ok);

  if (ok)
    return result;

  return 0;
}

static Scaled scaled (const QString &a_value)
{
  /* vars */
  Scaled result;
  Scaled::Type type = Scaled::Invalid;

  /* parse data */
  int firstBracket  = a_value.indexOf ('(') + 1,
      secondBracket = a_value.indexOf (')');

  auto data         = a_value.mid (firstBracket, secondBracket - firstBracket);
  auto valuesStr    = data.split (',');

  /* ------------------------------------------------------------ */
  /* lambda's */

  auto parseFont  = [ = ](Scaled &result, QStringList &valuesStr)
    {
      /* store font size */
      result.setFontSize (valuesStr.at (0).toInt());
    };

  auto parseRect  = [ = ](Scaled &result, QStringList &valuesStr)
    {
      /* get all rect values */
      double values[4] =
      {
        valuesStr.at(0) == "nan" ? -32000 : valuesStr.at (0).toDouble(),
        valuesStr.at(1) == "nan" ? -32000 : valuesStr.at (1).toDouble(),
        valuesStr.at(2) == "nan" ? -32000 : valuesStr.at (2).toDouble(),
        valuesStr.at(3) == "nan" ? -32000 : valuesStr.at (3).toDouble(),
      };
      bool aspect = valuesStr.at(4) == "true";

      /* store */
      result.setX (values[0]);
      result.setY (values[1]);
      result.setW (values[2]);
      result.setH (values[3]);
      result.setAspect (aspect);
    };

  auto parseAll   = [ = ](Scaled &result, QStringList &valuesStr)
    {
      /* store rect */
      parseRect (result, valuesStr);

      /* move font size to first */
      auto customList = QStringList {valuesStr.at (5)};

      /* store font size */
      parseFont (result, customList);
    };

  /* ------------------------------------------------------------ */

  /* set proper type */
  if (valuesStr.size() == 1)
    type  = Scaled::FontOnly;
  else if (valuesStr.size() == 5)
    type  = Scaled::RectOnly;
  else if (valuesStr.size() == 6)
    type  = Scaled::All;

  /* finish, if invalid */
  if (type == Scaled::Invalid)
    return (result = Scaled());

  /* parse by type */
  switch (type)
    {
    case Scaled::Invalid:   break;
    case Scaled::FontOnly:  parseFont (result, valuesStr); break;
    case Scaled::RectOnly:  parseRect (result, valuesStr); break;
    case Scaled::All:       parseAll (result, valuesStr); break;
    }

  /* store type and return */
  result.setType (type);
  return result;
}

/********************************************
 * METHODS
 *******************************************/

QList<QssValue> QssValue::parse (const QString &a_src)
{
  /* vars */
  QList<QssValue> result;

  /* check empty */
  if (!a_src.contains (';'))
    return result;

  /* split to lines */
  auto lines  = a_src.split (';');

  /* cycle thru lines, separating them to QssLine instances */
  for (auto i = lines.cbegin(), e = lines.cend(); i != e; i++)
    if (i->contains(':') && !i->contains("/*"))
      result += QssValue (*i);

  return result;
}



const QString &QssValue::field() const
{
  return m_field;
}

void QssValue::setField (const QString &newField)
{
  m_field = newField;
}



const QString &QssValue::value() const
{
  return m_value;
}

void QssValue::setValue (const QString &newValue)
{
  m_value = newValue;
}



QVariant QssValue::asVariant() const
{
  /* check issue */
  if (m_field.isEmpty() || m_value.isEmpty())
    return m_value;

  /* link */
  if (m_value.at (0) == '@'
      || (m_value.size() > 1 && m_value.at (1) == '@'))
    return QssLink::get (QString (m_value).replace ('\"',""));

  /* color */
  if (m_value.at (0) == '#'
      || (m_value.size() > 1 && m_value.at (1) == '#'))
    return QColor (QString (m_value).replace ('\"',""));

  /* rgba */
  if ((m_value.size() > 12) && (m_value.mid (0, 4) == "rgba"))
    return rgba (m_value);

  /* url */
  if ((m_value.size() > 4) && (m_value.mid (0, 3) == "url"))
    return url (m_value);

  /* url */
  if ((m_value.size() > 4) && (m_value.mid (0, 6) == "scaled"))
    return QVariant::fromValue (scaled (m_value));

  /* font-weight */
  if ((m_value.size() > 6) && (m_value.mid(0, 5) == "Font."))
    return font (m_value);

  /* font-weight */
  if ((m_value.size() > 6) && (m_value.mid(0, 5) == "Text."))
    return alignment (m_value);

  /* string */
  if ((m_value.startsWith('\'') && m_value.endsWith('\''))
      || (m_value.startsWith('\"') && m_value.endsWith('\"')))
    return m_value.mid (1, m_value.size() - 2);

  /* integer */
  bool success;
  int value = m_value.toInt(&success);
  if (success)
    return value;

  return m_value;
}
/*-----------------------------------------*/
}

/*-----------------------------------------*/

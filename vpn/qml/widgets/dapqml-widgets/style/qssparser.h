#ifndef QSSPARSER_H
#define QSSPARSER_H

/* INCLUDES */
#include <QString>

/* NAMESPACE */
namespace Style
{

class QssParser
{
public:
  static void perform(const QString &styleSheet);
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // QSSPARSER_H

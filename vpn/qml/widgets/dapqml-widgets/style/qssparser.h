#ifndef QSSPARSER_H
#define QSSPARSER_H

/* INCLUDES */
#include <QString>

/* NAMESPACE */
namespace DapStyle
{

/****************************************//**
 * @brief QSS style text parser
 *
 * Used to parse stylesheet text and store
 * result into QssMap
 *
 * @ingroup groupDapGuiStyleParser
 * @date 05.06.2022
 * @author Mikhail Shilenko
 *******************************************/

class QssParser
{
public:
  static void perform(const QString &styleSheet);
};

/*-----------------------------------------*/
}

/*-----------------------------------------*/
#endif // QSSPARSER_H

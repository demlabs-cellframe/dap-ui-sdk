#include <QtDebug>
#include <QProcess>
#include "DapUtils.h"

#ifdef DAP_OS_UNIX
const QString cShellPath = "sh";
const QString cShellPathArgs = "-c";
#define DAP_SHELL_PRESENT
#elif defined(DAP_OS_WINDOWS)
const QString cShellPath = "cmd.exe";
const QString cShellPathArgs = "/c";
#define DAP_SHELL_PRESENT
#else
//warning cause we still want to build it on android!
#warning "Not defined shell path on your platform"

#endif


#ifdef DAP_SHELL_PRESENT
/**
 * @brief DapUtun::runShellCmd
 * @param cmd
 * @return
 */

QString DapUtils::shellCmd(const QString& cmd, int waitMsecs)
{
    QProcess process;
    process.start(cShellPath, QStringList() << cShellPathArgs << cmd);
    process.waitForFinished( waitMsecs );
    QString result = process.readAllStandardOutput();

    if(!result.isEmpty())
        result.chop(1); // delete \n symbol
    else
        qWarning() << "Result shell cmd " << cmd << "is empty";

    return result;
}
#endif

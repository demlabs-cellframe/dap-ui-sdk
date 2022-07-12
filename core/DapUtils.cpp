#include <QtDebug>
#include <QProcess>
#include "DapUtils.h"

#ifdef DAP_OS_UNIX
const QString cShellPath = "sh";
const QString cShellPathArgs = "-c";
#elif defined(DAP_OS_WINDOWS)
const QString shellPath = "cmd.exe";
const QString shellPathArgs = "/c";
#else
#error "Not defined shell path on your platform"
#endif


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

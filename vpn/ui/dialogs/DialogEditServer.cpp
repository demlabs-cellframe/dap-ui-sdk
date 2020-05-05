#include "DialogEditServer.h"

/** @brief constructor
 *  @param a_parent object parent
 */
DialogEditServer::DialogEditServer(QWidget *a_parent,
                                   DialogType a_type):
    DapUiDialog(a_parent),
    m_stmMode(),
    m_stEdit(new QState(&m_stmMode)),
    m_stCreate(new QState(&m_stmMode))
{
    qDebug()<<"DialogEditServer created";
    createDialog<Ui::EditServer>();

    QLabel *lbCap = findChild<QLabel*>("lbCaption");
    Q_ASSERT(lbCap);
    a_type == ADD ? lbCap->setText("Add server") : lbCap->setText("Edit server");
}

/** @brief Load server data from a_Server
 *  @param a_Server server info
 */
void DialogEditServer::loadServer(const DapServerInfo *a_server)
{
    m_edTittle ->setText(a_server->name);
    m_edAddress->setText(a_server->address);
    m_edPort   ->setText(QString::number(a_server->port));
}

void DialogEditServer::clearAllFields()
{
    m_edTittle->clear();
    m_edAddress->clear();
    m_edPort->clear();
}

/** @brief Save server data
 */
void DialogEditServer::save()
{
    qDebug()<<"save server";
    clearAllFields();
    hide();
    deleteLater();
}

void DialogEditServer::hide()
{
    clearAllFields();
    DapUiDialog::hide();
    deleteLater();
}

/** @brief init user interface
 */
void DialogEditServer::initUi()
{
    // Initializing pointers to window widgets
    m_edTittle  = findChild<AnimatedLineEdit*>("edTittle" );    Q_ASSERT(m_edTittle );
    m_edAddress = findChild<AnimatedLineEdit*>("edAddress");    Q_ASSERT(m_edAddress);
    m_edPort    = findChild<AnimatedLineEdit*>("edPort"   );    Q_ASSERT(m_edPort   );

    m_btCancel  = findChild<QPushButton*>("btCancel");          Q_ASSERT(m_btCancel);
    m_btSave    = findChild<QPushButton*>("btSave");            Q_ASSERT(m_btSave);

    connect(m_btCancel, SIGNAL(clicked()), this, SLOT(hide()));
    connect(m_btSave,   SIGNAL(clicked()), this, SLOT(save()));
}

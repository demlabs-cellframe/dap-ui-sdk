#ifndef DIALOGEDITSERVER_H
#define DIALOGEDITSERVER_H

#include <QStateMachine>

#include "DapUiDialog.h"
#include "ui_dlg_edit_server.h"
#include "controls/AnimatedLineEdit.h"
#include "DapServerInfo.h"

/** @brief Dialog for creating and editing server
*/
class DialogEditServer : public DapUiDialog
{
    Q_OBJECT
public:
    enum DialogType {EDIT, ADD};
    DialogEditServer(QWidget *a_parent, DialogType a_type = EDIT);
    ~DialogEditServer() override { qDebug() << "DialogEditServer destroyed";}
    void loadServer(const DapServerInfo *a_Server);

public slots:
    void save();
    void hide() override;

protected:
    void initUi() override;
    void clearAllFields();

private:
    //========= Controll pointers =========

    AnimatedLineEdit *m_edTittle    {Q_NULLPTR}; ///< Tittle line edit
    AnimatedLineEdit *m_edAddress   {Q_NULLPTR}; ///< Address line edit
    AnimatedLineEdit *m_edPort      {Q_NULLPTR}; ///< Port line edit

    QPushButton      *m_btCancel    {Q_NULLPTR}; ///< Cancel button
    QPushButton      *m_btSave      {Q_NULLPTR}; ///< Save button

    //=============== States ==============

    QStateMachine m_stmMode;
    QState *m_stEdit;
    QState *m_stCreate;
};


#endif // DIALOGEDITSERVER_H

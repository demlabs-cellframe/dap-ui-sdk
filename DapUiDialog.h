#ifndef DAPUIDIALOG_H
#define DAPUIDIALOG_H

#include <QtDebug>
#include <QAbstractButton>
#include <QStyle>

#include "DapUiScreen.h"
#include "controls/ScreenOverlaying.h"

/** @brief Dialog with form and screen overlaying
*/
class DapUiDialog : public QFrame
{
    Q_OBJECT
public:
    DapUiDialog(QWidget * a_parent);
    virtual ~DapUiDialog() { qDebug() << "DapUiDialog destroyed"; }

    template<class TUi>
    void createDialog();

    virtual void initUi() = 0; ///< Mast be reimplemented

public slots:
    void show();
    virtual void hide();

protected:
    virtual void resizeEvent(QResizeEvent *a_event);

    void updatePosition();

private:

    ScreenOverlaying m_screenOverlaying; ///<Screen Overlaying
};

//=================================================================================
//                               Interface
//=================================================================================
/** @brief
 *  @param TUi Type of UI form
 */
template<class TUi>
void DapUiDialog::createDialog()
{
    static TUi *ui;
    if ( ui != nullptr)
            delete ui;
    ui = new TUi;

    ui->setupUi(this);

    initUi();
}



#endif // DAPUIDIALOG_H

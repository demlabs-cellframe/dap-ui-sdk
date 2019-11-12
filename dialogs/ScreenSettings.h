#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H

#include <QWidget>
#include <QTabWidget>
#include <QDebug>
#include <QIcon>
#include <QPushButton>
#include <QToolButton>
#include "SettingsMoreMenu.h"
#include "DapDataLocal.h"

namespace Ui {
class ScreenSettings;
}

class ScreenSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenSettings(QWidget *parent = nullptr);
    ~ScreenSettings();

    enum {FLAG, NAME, FAVORITE, MORE};

    void fillUpstreams();

public slots:
    void changeFavorite();
    void sltAddNewServerClicked();

private:
    Ui::ScreenSettings *ui;

    QPushButton* m_btFavorite;
    QList<const DapServerInfo*> m_servers;
    SettingsMoreMenu *m_mnuMore;

    DialogEditServer *m_dlgEditServer {Q_NULLPTR};

    DapUiDialog *openedDialog;
};



#endif // SCREENSETTINGS_H

#include "ScreenSettings.h"
#include "ui_settings.h"

ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSettings),
    m_mnuMore(new SettingsMoreMenu(this))
{
    qDebug() << "ScreenSettings constructor.";
    ui->setupUi(this);

    QTableWidget* tblServers = this->findChild<QTableWidget*>("tblServers");
    Q_ASSERT(tblServers);

    QPushButton* btAddServer = this->findChild<QPushButton*>("btAddServer");
    Q_ASSERT(btAddServer);
    connect(btAddServer, &QPushButton::clicked, this, &ScreenSettings::sltAddNewServerClicked);

    // Setup table columns:
    tblServers->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tblServers->setColumnWidth(0, (50 + 8));
    tblServers->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tblServers->setColumnWidth(2, 48);
    tblServers->setColumnWidth(3, 48);

    tblServers->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //fill table:
    //fillUpstreams();


    // ===== Scrollbar =============================================
    //scrollbar placed outsite QTableWidget:
    QScrollBar* newScrollBar = this->findChild<QScrollBar*>("verticalScrollBar");
    QScrollBar* oldScrollBar = tblServers->verticalScrollBar();


    //scrollbars connectings:
    connect(newScrollBar, SIGNAL(valueChanged(int)),
            oldScrollBar, SLOT(setValue(int)));

    connect(oldScrollBar,SIGNAL(valueChanged(int)),
            newScrollBar,SLOT(setValue(int)));

    //set up new scrollbar on old scrollbar changed:
    connect(oldScrollBar, &QScrollBar::rangeChanged,[=](int min,  int max){
        Q_UNUSED(min);
        if (max == 0) newScrollBar->hide();
        else {
            newScrollBar->setRange(min, max);
            newScrollBar->setSingleStep(oldScrollBar->singleStep());
            newScrollBar->setPageStep(oldScrollBar->pageStep());
            newScrollBar->show();
        }
    });

    connect(m_mnuMore, &SettingsMoreMenu::editClicked, [this](const DapServerInfo* a_server){
        auto dlg = new DialogEditServer(const_cast<ScreenSettings*>(this));
        dlg->loadServer(a_server);
        dlg->show();
    });

}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}

void ScreenSettings::fillUpstreams()
{
    QTableWidget *tblServers = (this->findChild<QTableWidget*>("tblServers"));

    int i = 0;
    for(const DapServerInfo& curServer: DapDataLocal::me()->servers()){
        //save curServer to m_servers
        m_servers.append(&curServer);

        //insert row to table:
        tblServers->insertRow(i);

        //column 0 - "Flag"
        //-------------------------------------------------
        QTableWidgetItem *item = new QTableWidgetItem(QIcon(DapDataLocal::me()->locationToIconPath(curServer.location)), "");
        item->setFlags(Qt::ItemIsEnabled);
        tblServers->setItem(i, FLAG, item);

        //column 1 - "Name"
        //-------------------------------------------------
        item = new QTableWidgetItem(curServer.name);
        item->setFlags(Qt::NoItemFlags);               //disabled
        tblServers->setItem(i, NAME, item);

        //column 2 - "Favorite" (QPushButton)
        //-------------------------------------------------
        QPushButton *btFavorite = new QPushButton(tblServers);
        btFavorite->setCheckable(true);                 //checkable = true
        btFavorite->setCursor(Qt::PointingHandCursor);  //cursor = PointingHand
        connect(btFavorite, SIGNAL(clicked()), SLOT(changeFavorite()));

        tblServers->setCellWidget(i, FAVORITE, btFavorite);

        //if index = 0  set btFavorite checked
        if (!i){
            m_btFavorite = btFavorite;
            btFavorite->setChecked(true);
        }

        //column 3 - "More" (QPushButton)
        //-------------------------------------------------
        QToolButton *btMore = new QToolButton(tblServers);
        btMore->setCursor(Qt::PointingHandCursor);  //cursor = PointingHand
        tblServers->setCellWidget(i, MORE, btMore); //insert btMore into MORE column

        //on click - setup menu "More" and show
        connect(btMore, &QToolButton::clicked, [=]{
            qDebug()<<"[ScreenSettings] Button \"More\" pressed for server " << curServer.name;
            int MenuWidth = m_mnuMore->sizeHint().width();
            QPoint pos =  btMore->mapToGlobal(QPoint(btMore->width() - 14 - MenuWidth, btMore->height()));
            m_mnuMore->setParentButton(btMore);     //ParentButton
            m_mnuMore->setCurentServer(&curServer); //CurentServer
            m_mnuMore->exec(pos);                   //show
        });
        i++;
    }
    tblServers->sortItems(1);
}

void ScreenSettings::changeFavorite()
{
      QPushButton *btNewFavorite = qobject_cast<QPushButton*>(sender());
      //If clicked button is unchecked ...
      if (btNewFavorite != m_btFavorite) {
          //make old btgFavorite unchecked
          m_btFavorite->setChecked(false);
          m_btFavorite = btNewFavorite;
          //make new favorite server "The Best"
          const DapServerInfo* curServer = (m_servers.at(btNewFavorite->objectName().toInt()));
          qDebug()<<"[ScreenSettings] Action \"MakeFavorite\" pressed for server " << curServer->name;
          DapDataLocal::me()->setServerTheBest(*curServer);
      }
      //If clicked button is checked do nothing
      else {
          btNewFavorite->setChecked(true);
      }
}

void ScreenSettings::sltAddNewServerClicked()
{
    qDebug() << "Add new server btn clicked";
    auto dlg = new DialogEditServer(const_cast<ScreenSettings*>(this), DialogEditServer::ADD);
    dlg->show();
}

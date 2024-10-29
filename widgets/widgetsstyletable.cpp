/* INCLUDES */
#include "widgetsstyletable.h"
#include "ui_widgetsstyletable.h"
#include <QClipboard>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

WidgetsStyleTable::WidgetsStyleTable(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WidgetsStyleTable),
  m_tableModel(new StyleSheetTableModel)
{
  ui->setupUi(this);
  ui->tableView->setModel (m_tableModel);
  connect (ui->tableView, &QTableView::doubleClicked,
           this, & WidgetsStyleTable::slotDoubleClicked,
           Qt::QueuedConnection);
}

WidgetsStyleTable::~WidgetsStyleTable()
{
  delete ui;
}

/********************************************
 * SLOTS
 *******************************************/

void WidgetsStyleTable::slotDoubleClicked (const QModelIndex &index)
{
  QClipboard *clipboard = QGuiApplication::clipboard();
  auto data             = m_tableModel->data (index, Qt::DisplayRole).toString();
  clipboard->setText (data);
}

/*-----------------------------------------*/

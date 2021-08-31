#include "statisticscreen.h"
#include "ui_statisticscreen.h"

StatisticScreen::StatisticScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticScreen)
{
    ui->setupUi(this);
}

StatisticScreen::~StatisticScreen()
{
    delete ui;
}

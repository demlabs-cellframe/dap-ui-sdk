#ifndef STATISTICSCREEN_H
#define STATISTICSCREEN_H

#include <QWidget>

namespace Ui {
    class StatisticScreen;
    }

class StatisticScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticScreen(QWidget *parent = nullptr);
    ~StatisticScreen();

private:
    Ui::StatisticScreen *ui;
};

#endif // STATISTICSCREEN_H

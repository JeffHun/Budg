#ifndef DETAILWINDOW_H
#define DETAILWINDOW_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSettings>
#include "expense.h"

class DetailWindow : public QWidget{

public:
    explicit DetailWindow(QWidget*, QList<Expense>, QList<Expense>, QList<Expense>, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
    explicit DetailWindow(QWidget*, QList<Expense>, int);
    ~DetailWindow();

private:
    QWidget* categoryWidgetGeneration(QString, QString, QString, int, QWidget*);
    QString convertCategoryToString(const Expense::Category&);
    bool eventFilter(QObject*, QEvent*)override;
    QList<Expense> refundExpenses;
    QList<Expense> incomeExpenses;
    QList<Expense> inflowExpenses;
    QString getMonthName(int);
};

#endif // DETAILWINDOW_H

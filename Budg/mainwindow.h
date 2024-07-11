#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QLineEdit>
#include <QMessageBox>

#include "month.h"
#include "detailwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMonths(QList<Month*>);

private :
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QWidget *dropWidget;
    QVBoxLayout *scrollLayout;
    QWidget *scrollWidget;
    QPushButton *setBudgetsButton;
    QScrollArea *scrollArea;
    QList<QFile*> files;
    QList<Month*> months;
    QSettings* settings;
    QList<float> defaultValues;
    QList<float> budgetCategoryValues;
    QList<Month*> selectedMonths;
    int currentYear;
    QWidget* summaryWidget;
    QGridLayout* summaryLayout;
    DetailWindow *detailWindow;
    int monthAlreadyPrint;

    int fileNbr;
    void fileUploadPageGeneration();
    void dragEnterEvent(QDragEnterEvent*) override;
    void dragLeaveEvent(QDragLeaveEvent*) override;
    void dropEvent(QDropEvent*) override;
    void browseFile();
    void addFile(QFile*);
    bool isAllFilesAreCSV(const QList<QUrl> &urls) const;
    void budgetingPageGeneration();
    void getBudgetCategory();
    void setBudgetCategory();
    void expenseAnalysisPageGeneration();
    void monthButtonGeneration();
    void selectedThisMonth(Month*, QPushButton*);
    void loadPreviousYear();
    void loadNextYear();
    void categoriesSummaryGeneration();
    QWidget* categorySummaryGeneration(QString, QString);
    QWidget* monthSummaryCategoryValueGeneration(int, QString, int);
    QWidget* monthSummaryHeaderGeneration(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
    void monthSummaryGeneration();
    bool eventFilter(QObject*, QEvent*)override;
    QString convertCategoryToString(const Expense::Category&);
    void clearSummary();
    void showMonthSummaryRow(int);
    void hideMonthSummaryRow(int);
    void hideAllMonthSummary();
    QString getMonthName(int);

signals:
    void processTheseFiles(QList<QFile*>);
};
#endif // MAINWINDOW_H

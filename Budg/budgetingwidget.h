#ifndef BUDGETINGWIDGET_H
#define BUDGETINGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QGridLayout>
#include <QDebug>
#include <QPieSlice>

class BudgetingWidget : public QWidget {

public:
    explicit BudgetingWidget(const QString&, const QString&, const QString&, QWidget*, QPieSlice*, float, QLabel*);
    void updateSlice();
    void updatePercentageLabel();
    void setBudgetingWidgets(QList<BudgetingWidget*>);
    float getValue();
    void updateSlicesAndPercentageLabels();

protected:
    bool event(QEvent*) override;

private:
    QString imgPath;
    QString name;
    QString color;
    QPieSlice *slice;
    QWidget* parent;
    QLabel *percentageLabel;
    QLineEdit *lineEdit;
    float value;
    QList<BudgetingWidget*> budgetingWidgets;
    QLabel *expenseExempleLabel;

    void setUp();
    void setDefaultValue();
    void updateExpenseExempleLabel();
};

#endif // BUDGETINGWIDGET_H

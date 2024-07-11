#ifndef MONTH_H
#define MONTH_H

#include "expense.h"

class Month
{
public:

    Month(QList<Expense>, int, int);
    Month();

private:
    QList<Expense> expenses;
    int year;
    int monthIndex;
    int endRemaining;
    float totalFoodExpenses;
    float totalBankExpenses;
    float totalEducationExpenses;
    float totalTaxesExpenses;
    float totalLegalExpenses;
    float totalHousingExpenses;
    float totalLeisureExpenses;
    float totalHealthExpenses;
    float totalShoppingExpenses;
    float totalExcludedExpenses;
    float totalTransportsExpenses;
    float totalOutflowExpenses;
    float totalSavingExpenses;
    float totalIncomeExpenses;
    float totalInflowExpenses;
    float totalRefundExpenses;
    QList<Expense>foodExpenses;
    QList<Expense>bankExpenses;
    QList<Expense>educationExpenses;
    QList<Expense>taxesExpenses;
    QList<Expense>legalExpenses;
    QList<Expense>housingExpenses;
    QList<Expense>leisureExpenses;
    QList<Expense>healthExpenses;
    QList<Expense>shoppingExpenses;
    QList<Expense>excludedExpenses;
    QList<Expense>transportExpenses;
    QList<Expense>outflowExpenses;
    QList<Expense>savingExpenses;
    QList<Expense>incomeExpenses;
    QList<Expense>inflowExpenses;
    QList<Expense>refundExpenses;

public:
    int getMonthIndex();
    int getYear();
    QList<Expense> getExpenses();
    bool operator==(const Month &other) const;
    void setTotalExpenses();
    int getEndRemaining();
    int getTotalFoodExpenses();
    int getTotalBankExpenses();
    int getTotalEducationExpenses();
    int getTotalTaxesExpenses();
    int getTotalLegalExpenses();
    int getTotalHousingExpenses();
    int getTotalLeisureExpenses();
    int getTotalHealthExpenses();
    int getTotalShoppingExpenses();
    int getTotalExcludedExpenses();
    int getTotalTransportsExpenses();
    int getTotalOutflowExpenses();
    int getTotalSavingExpenses();
    int getTotalIncomeExpenses();
    int getTotalInflowExpenses();
    int getTotalRefundExpenses();
    QList<Expense>getFoodExpenses();
    QList<Expense>getBankExpenses();
    QList<Expense>getEducationExpenses();
    QList<Expense>getTaxesExpenses();
    QList<Expense>getLegalExpenses();
    QList<Expense>getHousingExpenses();
    QList<Expense>getLeisureExpenses();
    QList<Expense>getHealthExpenses();
    QList<Expense>getShoppingExpenses();
    QList<Expense>getExcludedExpenses();
    QList<Expense>getTransportExpenses();
    QList<Expense>getOutflowExpenses();
    QList<Expense>getSavingExpenses();
    QList<Expense>getIncomeExpenses();
    QList<Expense>getInflowExpenses();
    QList<Expense>getRefundExpenses();
};

#endif // MONTH_H

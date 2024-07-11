#include "month.h"

Month::Month(QList<Expense> aExpenses, int aMonthIndex, int aYear) {
    expenses = aExpenses;
    monthIndex = aMonthIndex;
    year = aYear;
    setTotalExpenses();
}

void Month::setTotalExpenses()
{
    totalFoodExpenses = 0;
    totalBankExpenses = 0;
    totalEducationExpenses = 0;
    totalTaxesExpenses = 0;
    totalLegalExpenses = 0;
    totalHousingExpenses = 0;
    totalLeisureExpenses = 0;
    totalHealthExpenses = 0;
    totalShoppingExpenses = 0;
    totalExcludedExpenses = 0;
    totalTransportsExpenses = 0;
    totalOutflowExpenses = 0;
    totalSavingExpenses = 0;
    totalIncomeExpenses = 0;
    totalInflowExpenses = 0;
    totalRefundExpenses = 0;
    for(int i = 0; i < expenses.count(); i++)
    {
        if(expenses[i].getCategory() == Expense::Category::Food)
        {
            foodExpenses.append(expenses[i]);
            totalFoodExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::BankAndInsurance)
        {
            bankExpenses.append(expenses[i]);
            totalBankExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::EducationAndFamily)
        {
            educationExpenses.append(expenses[i]);
            totalEducationExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::TaxesAndDuties)
        {
            taxesExpenses.append(expenses[i]);
            totalTaxesExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::LegalAndAdministrative)
        {
            legalExpenses.append(expenses[i]);
            totalLegalExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::HousingAndHome)
        {
            housingExpenses.append(expenses[i]);
            totalHousingExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::LeisureAndVacations)
        {
            leisureExpenses.append(expenses[i]);
            totalLeisureExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::Health)
        {
            healthExpenses.append(expenses[i]);
            totalHealthExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::ShoppingAndServices)
        {
            shoppingExpenses.append(expenses[i]);
            totalShoppingExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::ExcludedAndTransaction)
        {
            excludedExpenses.append(expenses[i]);
            totalExcludedExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::Transports)
        {
            transportExpenses.append(expenses[i]);
            totalTransportsExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::OutflowOfMoney)
        {
            outflowExpenses.append(expenses[i]);
            totalOutflowExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::Saving)
        {
            savingExpenses.append(expenses[i]);
            totalSavingExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::IncomeAndEarnings)
        {
            incomeExpenses.append(expenses[i]);
            totalIncomeExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::InflowOfMoney)
        {
            inflowExpenses.append(expenses[i]);
            totalInflowExpenses += expenses[i].getValue();
        }
        if(expenses[i].getCategory() == Expense::Category::Refund)
        {
            refundExpenses.append(expenses[i]);
            totalRefundExpenses += expenses[i].getValue();
        }
    }
    totalFoodExpenses = round(totalFoodExpenses);
    totalBankExpenses = round(totalBankExpenses);
    totalEducationExpenses = round(totalEducationExpenses);
    totalTaxesExpenses = round(totalTaxesExpenses);
    totalLegalExpenses = round(totalLegalExpenses);
    totalHousingExpenses = round(totalHousingExpenses);
    totalLeisureExpenses = round(totalLeisureExpenses);
    totalHealthExpenses = round(totalHealthExpenses);
    totalShoppingExpenses = round(totalShoppingExpenses);
    totalExcludedExpenses = round(totalExcludedExpenses);
    totalTransportsExpenses = round(totalTransportsExpenses);
    totalOutflowExpenses = round(totalOutflowExpenses);
    totalSavingExpenses = round(totalSavingExpenses);
    totalIncomeExpenses = round(totalIncomeExpenses);
    totalInflowExpenses = round(totalInflowExpenses);
    totalRefundExpenses = round(totalRefundExpenses);
    endRemaining = totalFoodExpenses + totalBankExpenses + totalEducationExpenses + totalTaxesExpenses + totalLegalExpenses + totalHousingExpenses + totalLeisureExpenses + totalHealthExpenses + totalShoppingExpenses + totalExcludedExpenses + totalTransportsExpenses + totalOutflowExpenses + totalSavingExpenses + totalIncomeExpenses + totalInflowExpenses + totalRefundExpenses;
}

Month::Month() {
}

int Month::getMonthIndex()
{
    return monthIndex;
}

int Month::getYear()
{
    return year;
}

QList<Expense> Month::getExpenses()
{
    return expenses;
}

QList<Expense> Month::getFoodExpenses()
{
    return foodExpenses;
}

int Month::getTotalFoodExpenses()
{
    return totalFoodExpenses;
}

int Month::getTotalBankExpenses()
{
    return totalBankExpenses;
}

int Month::getTotalEducationExpenses()
{
    return totalEducationExpenses;
}

int Month::getTotalTaxesExpenses()
{
    return totalTaxesExpenses;
}

int Month::getTotalLegalExpenses()
{
    return totalLegalExpenses;
}

int Month::getTotalHousingExpenses()
{
    return totalHousingExpenses;
}

int Month::getTotalLeisureExpenses()
{
    return totalLeisureExpenses;
}

int Month::getTotalHealthExpenses()
{
    return totalHealthExpenses;
}

int Month::getTotalShoppingExpenses()
{
    return totalShoppingExpenses;
}

int Month::getTotalExcludedExpenses()
{
    return totalExcludedExpenses;
}

int Month::getTotalTransportsExpenses()
{
    return totalTransportsExpenses;
}

int Month::getTotalOutflowExpenses()
{
    return totalOutflowExpenses;
}

int Month::getTotalSavingExpenses()
{
    return totalSavingExpenses;
}

int Month::getTotalIncomeExpenses()
{
    return totalIncomeExpenses;
}

int Month::getTotalInflowExpenses()
{
    return totalInflowExpenses;
}

int Month::getTotalRefundExpenses()
{
    return totalRefundExpenses;
}

int Month::getEndRemaining()
{
    return endRemaining;
}

bool Month::operator==(const Month &other) const {
    if (year == other.year && monthIndex == other.monthIndex)
        return true;
    else
        return false;
}

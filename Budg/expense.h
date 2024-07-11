#ifndef EXPENSE_H
#define EXPENSE_H

#include<QDate>

class Expense
{
public:
    enum class Category
    {
        Food,
        BankAndInsurance,
        EducationAndFamily,
        TaxesAndDuties,
        LegalAndAdministrative,
        HousingAndHome,
        LeisureAndVacations,
        Health,
        ShoppingAndServices,
        ExcludedAndTransaction,
        Transports,
        OutflowOfMoney,
        Saving,
        IncomeAndEarnings,
        InflowOfMoney,
        Refund
    };
    Expense(QString, float, QDate, Category);
    float getValue();
    Category getCategory();
    QDate getDate();
    QString getName();

private:
    float value;
    Category category;
    QDate date;
    QString name;
};

#endif // EXPENSE_H

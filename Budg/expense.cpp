#include "expense.h"

Expense::Expense(QString aName, float aValue, QDate aDate, Category aCategory) {
    name = aName;
    value = aValue;
    date = aDate;
    category = aCategory;
}

QString Expense::getName()
{
    return name;
}

float  Expense::getValue()
{
    return value;
}

QDate  Expense::getDate()
{
    return date;
}

Expense::Category Expense::getCategory()
{
    return category;
}

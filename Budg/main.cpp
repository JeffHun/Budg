#include "mainwindow.h"
#include "expense.h"
#include "month.h"

#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>

void processTheseFiles(const QList<QFile*>&, MainWindow*);
QList<Expense> setExpenses(const QList<QFile*>&, MainWindow*);
bool checkSlot(int slot, const QString&, const QString&, QWidget*);
Expense::Category setCategory(const QString&);
QList<Month*> setMonths(QList<Expense>);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Budg");
    window.resize(1271,901);
    window.show();
    QObject::connect(&window, &MainWindow::processTheseFiles, &app, [&](const QList<QFile*>& files) {
        processTheseFiles(files, &window);
    });
    return app.exec();
}

void processTheseFiles(const QList<QFile*>& files,MainWindow* window)
{
    QList<Expense> expenses = setExpenses(files, window);
    if(!expenses.empty())
    {
        QList<Month*> months;
        months = setMonths(expenses);
        window->setMonths(months);
    }
    else
        QMessageBox::critical(window, "Erreur", "There is no information to process, please try again with a valid file.");
}

QList<Month*> setMonths(QList<Expense> expenses)
{
    std::sort(expenses.begin(), expenses.end(),[](Expense& a, Expense& b) {return a.getDate() < b.getDate();});

    int currentMonth = expenses[0].getDate().month();
    int currentYear = expenses[0].getDate().year();
    QList<Expense> monthExpense;
    QList<Month*> months;

    //browse expenses and create a new month for each monthly overage
    for(int i = 0; i < expenses.count(); i++)
    {
        if(expenses[i].getDate().month() == currentMonth && expenses[i].getDate().year() == currentYear)
            monthExpense.append(expenses[i]);
        else
        {
            months.append(new Month(monthExpense, monthExpense[0].getDate().month(), monthExpense[0].getDate().year()));
            currentMonth = expenses[i].getDate().month();
            currentYear = expenses[i].getDate().year();
            monthExpense.clear();
            monthExpense.append(expenses[i]);
        }
    }
    months.append(new Month(monthExpense, monthExpense[0].getDate().month(), monthExpense[0].getDate().year()));

    return months;
}

QList<Expense> setExpenses(const QList<QFile*>& files, MainWindow* window)
{
    QList<Expense> expenses;
    foreach(QFile* file, files)
    {
        if(!file->open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::critical(window, "Erreur", "Unable to open the file\n"+file->fileName()+"\nThis file will be ignored");
            continue;
        }

        QTextStream in(file);
        const QStringList labels = in.readLine().split(';');
        const int dateSlot = labels.indexOf("Date de comptabilisation");
        const int nameSlot = labels.indexOf("Libelle operation");
        const int categorySlot = labels.indexOf("Categorie");
        const int debitSlot = labels.indexOf("Debit");
        const int creditSlot = labels.indexOf("Credit");

        if(!checkSlot(dateSlot, "date", file->fileName(), window) ||
            !checkSlot(nameSlot, "name", file->fileName(), window) ||
            !checkSlot(categorySlot, "category", file->fileName(), window) ||
            !checkSlot(debitSlot, "debit", file->fileName(), window) ||
            !checkSlot(creditSlot, "credit", file->fileName(), window))
        {
            file->close();
            continue;
        }

        // Ignore first line cause it is a line for header
        int lineNbr = 1;
        while(!in.atEnd())
        {
            lineNbr++;

            QStringList items = in.readLine().split(';');

            const float value = items[debitSlot].isEmpty() ? items[creditSlot].replace(",", ".").toFloat() : items[debitSlot].replace(",", ".").toFloat();
            if(value == 0)
            {
                QMessageBox::critical(window, "Erreur", "Credit or debit value is invalid in the file\n" +file->fileName()+"\non the line " + QString::number(lineNbr)+"\nThis line will be ignored.");
                continue;
            }

            const QDate date = QDate::fromString(items[dateSlot], "dd/MM/yyyy");
            if(!date.isValid())
            {
                QMessageBox::critical(window, "Erreur", "Date value is invalid in the file\n" +file->fileName()+"\non the line " + QString::number(lineNbr)+"\nThis line will be ignored.");
                continue;
            }

            Expense::Category category = setCategory(items[categorySlot]);

            if(value > 0 && category != Expense::Category::InflowOfMoney && category != Expense::Category::IncomeAndEarnings)
                category = Expense::Category::Refund;

            const QString name = items[nameSlot];

            Expense expense(name, value, date, category);
            expenses.append(expense);
        }
        file->close();
    }
    return expenses;
}

bool checkSlot(int slot, const QString& columnName, const QString& fileName, QWidget* window) {
    if (slot == -1) {
        QMessageBox::critical(window, "Erreur", QString("Operation %1 column were not found in the file.\n%2\nThis file will be ignored.").arg(columnName, fileName));
        return false;
    }
    return true;
}

Expense::Category setCategory(const QString& categoryStr)
{
    static const QMap<QString, Expense::Category> categoryMap = {
        {"Alimentation", Expense::Category::Food},
        {"Banque et assurances", Expense::Category::BankAndInsurance},
        {"Education et famille", Expense::Category::EducationAndFamily},
        {"Epargne", Expense::Category::Saving},
        {"Impots et taxes", Expense::Category::TaxesAndDuties},
        {"Juridique et administratif", Expense::Category::LegalAndAdministrative},
        {"Logement - maison", Expense::Category::HousingAndHome},
        {"Loisirs et vacances", Expense::Category::LeisureAndVacations},
        {"Revenus et rentrees d'argent", Expense::Category::IncomeAndEarnings},
        {"Sante", Expense::Category::Health},
        {"Shopping et services", Expense::Category::ShoppingAndServices},
        {"Transaction exclue", Expense::Category::ExcludedAndTransaction},
        {"Transports", Expense::Category::Transports},
        {"A categoriser - sortie d'argent", Expense::Category::OutflowOfMoney},
        {"A categoriser - rentree d'argent", Expense::Category::InflowOfMoney}
    };

    return categoryMap.value(categoryStr, Expense::Category::Refund);
}

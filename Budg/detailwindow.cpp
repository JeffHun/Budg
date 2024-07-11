#include "detailwindow.h"
#include <Qlabel>
#include <QVBoxLayout>

DetailWindow::DetailWindow(QWidget *parent, QList<Expense> aExpenses, int aBudget): QWidget(parent)
{
    this->setObjectName("detailWindow");
    this->setStyleSheet("#detailWindow{background-color : #241E38;}");

    QVBoxLayout *layout = new QVBoxLayout(this);

    if(aExpenses.count() > 0)
    {
        this->resize(500,480);
        QString dateString = getMonthName(aExpenses[0].getDate().month());
        dateString += " " + QString::number(aExpenses[0].getDate().year());
        QLabel *dateLabel = new QLabel(dateString, this);
        dateLabel->setObjectName("dateLabel");
        dateLabel->setStyleSheet("#dateLabel{"
                                 "font-size: 30px;"
                                 "font-family: Roboto Black;"
                                 "color : #D9D9D9;}");
        layout->addWidget(dateLabel);
        layout->setAlignment(dateLabel, Qt::AlignHCenter);

        QLabel *categoryLabel = new QLabel(convertCategoryToString(aExpenses[0].getCategory()), this);
        categoryLabel->setObjectName("categoryLabel");
        categoryLabel->setStyleSheet("#categoryLabel{"
                                     "font-size: 15px;"
                                     "font-family: Roboto Medium;"
                                     "color : #D9D9D9;}");
        layout->addWidget(categoryLabel);
        layout->setAlignment(categoryLabel, Qt::AlignHCenter);

        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setStyleSheet("#scrollArea{"
                                  "border: none;"
                                  "}"
                                  "QScrollBar:vertical{"
                                  "border: none;"
                                  "background: #3D3558;"
                                  "width: 10px;"
                                  "border-radius: 5px;"
                                  "}"
                                  "QScrollBar::handle:vertical{"
                                  "background: #6954D7;"
                                  "border-radius: 5px;"
                                  "}"
                                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical{"
                                  "background: none;"
                                  "border: none;"
                                  "}"
                                  "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                                  "background: none;"
                                  "}");
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);

        QWidget *scrollContent = new QWidget(scrollArea);
        scrollContent->setObjectName("scrollContent");
        scrollContent->setStyleSheet("#scrollContent{"
                                     "background-color: #241E38;}");
        QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

        float totalExpense = 0;
        int roundedTotalExpense;
        for(int i = 0; i < aExpenses.count(); i++)
        {
            totalExpense += aExpenses[i].getValue();

            QWidget *expenseWidget = new QWidget(scrollContent);
            expenseWidget->setFixedHeight(40);
            expenseWidget->setObjectName("expenseWidget");
            QHBoxLayout *expenseLayout = new QHBoxLayout(expenseWidget);
            QLabel *dateLabel = new QLabel(aExpenses[i].getDate().toString("dd/MM/yyyy"), expenseWidget);
            dateLabel->setObjectName("label");
            QLabel *nameLabel = new QLabel(aExpenses[i].getName(), expenseWidget);
            nameLabel->setObjectName("label");
            QLabel *valueLabel = new QLabel(QString::number(aExpenses[i].getValue()), expenseWidget);
            valueLabel->setObjectName("label");
            expenseLayout->addWidget(dateLabel);
            expenseLayout->addStretch();
            expenseLayout->addWidget(nameLabel);
            expenseLayout->addStretch();
            expenseLayout->addWidget(valueLabel);
            scrollLayout->addWidget(expenseWidget);
            expenseWidget->setStyleSheet("#expenseWidget{"
                                         "background-color: #3D3558;"
                                         "border-radius: 10px;}"
                                         "#fileWidget:hover{"
                                         "background-color: #6954D7;}");
            QString labelStyle = "#label {"
                                 "font-family: Roboto Light;"
                                 "font-size: 15px;"
                                 "color: white;}";
            dateLabel->setStyleSheet(labelStyle);
            nameLabel->setStyleSheet(labelStyle);
            valueLabel->setStyleSheet(labelStyle);
        }
        roundedTotalExpense = round(totalExpense);
        scrollArea->setWidget(scrollContent);

        QLabel *totalExpenseLabel;
        // if the budget is equal to -1 this means that the detail window will only display amounts received as refund, incom or inflow
        if(aBudget == -1)
            totalExpenseLabel = new QLabel("Earned : "+QString::number(roundedTotalExpense), this);
        else
            totalExpenseLabel = new QLabel("Expended : "+QString::number(roundedTotalExpense*-1), this);
        totalExpenseLabel->setObjectName("totalExpenseLabel");
        totalExpenseLabel->setStyleSheet("#totalExpenseLabel{"
                                         "font-size: 15px;"
                                         "font-family: Roboto Medium;"
                                         "color : #D9D9D9;}");

        QWidget *valueWidget = new QWidget(this);
        QHBoxLayout *valueLayout = new QHBoxLayout(valueWidget);
        valueLayout->addStretch();
        valueLayout->addWidget(totalExpenseLabel);
        valueLayout->addStretch();
        if(aBudget != -1)
        {
            QLabel *budgetLabel = new QLabel("Budget : " + QString::number(aBudget), this);
            budgetLabel->setObjectName("budgetLabel");
            budgetLabel->setStyleSheet("#budgetLabel{"
                                       "font-size: 15px;"
                                       "font-family: Roboto Medium;"
                                       "color : #D9D9D9;}");
            valueLayout->addWidget(budgetLabel);
            valueLayout->addStretch();
        }
        layout->addWidget(valueWidget);
        layout->addWidget(scrollArea);
    }
    else
    {
        QLabel *label = new QLabel("No expenses, the entire budget amount is respected", this);
        label->setObjectName("label");
        label->setStyleSheet("#label{"
                             "font-size: 15px;"
                             "font-family: Roboto Medium;"
                             "color : #D9D9D9;}");
        layout->addWidget(label);
    }


}

QString DetailWindow::getMonthName(int monthIndex) {
    static const QStringList monthNames = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    if (monthIndex < 1 || monthIndex > 12)
        return "Unknown";
    return monthNames.at(monthIndex - 1);
}

QString DetailWindow::convertCategoryToString(const Expense::Category& category)
{
    static const QMap<Expense::Category, QString> categoryMap = {
        {Expense::Category::Food, "Food"},
        {Expense::Category::BankAndInsurance, "Bank & insurance"},
        {Expense::Category::EducationAndFamily, "Education & family"},
        {Expense::Category::Saving, "Saving"},
        {Expense::Category::TaxesAndDuties, "Taxes & duties"},
        {Expense::Category::LegalAndAdministrative, "Legal & administrative"},
        {Expense::Category::HousingAndHome, "Housing"},
        {Expense::Category::LeisureAndVacations, "Leisure & holidays"},
        {Expense::Category::Health, "Health"},
        {Expense::Category::ShoppingAndServices, "Shopping & services"},
        {Expense::Category::ExcludedAndTransaction, "Excluded transaction"},
        {Expense::Category::Transports, "Transport"},
        {Expense::Category::OutflowOfMoney, "Outflow of money"}
    };
    return categoryMap.value(category);
}

DetailWindow::DetailWindow(QWidget *parent, QList<Expense> aRefundExpenses, QList<Expense> aIncomeExpenses, QList<Expense> aInflowExpenses, int aMonthIndex, int aYear, int aMonthEndRemaining, int aTotalFood, int aTotalBank, int aTotalEducation, int aTotalTaxes, int aTotalLegal, int aTotalHousing, int aTotalLeisure, int aTotalHealth, int aTotalShopping, int aTotalExcluded, int aTotalTransport, int aTotalOutflow, int aTotalSaving, int aTotalIncome, int aTotalInflow, int aTotalRefund) : QWidget(parent)
{
    refundExpenses = aRefundExpenses;
    incomeExpenses = aIncomeExpenses;
    inflowExpenses = aInflowExpenses;

    this->setObjectName("detailWindow");
    this->setStyleSheet("#detailWindow{background-color : #241E38;}");

    QLabel *dateLabel = new QLabel(getMonthName(aMonthIndex) + " " + QString::number(aYear), this);

    dateLabel->setObjectName("dateLabel");
    dateLabel->setStyleSheet("#dateLabel{"
                             "font-size: 30px;"
                             "font-family: Roboto Black;"
                             "color : #D9D9D9;}");
    int earn = aTotalInflow + aTotalIncome + aTotalRefund;
    QLabel *earnLabel = new QLabel("Earned : " + QString::number(earn), this);
    earnLabel->setObjectName("earnLabel");
    earnLabel->setStyleSheet("#earnLabel{"
                             "font-size: 15px;"
                             "font-family: Roboto Medium;"
                             "color : #D9D9D9;}");
    int totalExpense = aTotalFood + aTotalBank + aTotalEducation + aTotalTaxes + aTotalLegal + aTotalHousing + aTotalLeisure + aTotalHealth + aTotalShopping + aTotalExcluded + aTotalTransport + aTotalOutflow + aTotalSaving;
    totalExpense *= -1;
    QLabel *expenseLabel = new QLabel("Expended : " + QString::number(totalExpense), this);
    expenseLabel->setObjectName("expenseLabel");
    expenseLabel->setStyleSheet("#expenseLabel{"
                                "font-size: 15px;"
                                "font-family: Roboto Medium;"
                                "color : #D9D9D9;}");
    QLabel *remainingLabel = new QLabel("Remaining : "+QString::number(aMonthEndRemaining), this);
    remainingLabel->setObjectName("remainingLabel");
    remainingLabel->setStyleSheet("#remainingLabel{"
                                  "font-size: 15px;"
                                  "font-family: Roboto Medium;"
                                  "color : #D9D9D9;}");
    QWidget *summaryWidget = new QWidget(this);
    QHBoxLayout *summaryLayout = new QHBoxLayout(summaryWidget);
    summaryLayout->addStretch();
    summaryLayout->addWidget(earnLabel);
    summaryLayout->addStretch();
    summaryLayout->addWidget(expenseLabel);
    summaryLayout->addStretch();
    summaryLayout->addWidget(remainingLabel);
    summaryLayout->addStretch();
    QWidget* expensesCategoriesWidget = new QWidget(this);
    QWidget* earnedCategoriesWidget = new QWidget(this);
    QVBoxLayout* expensesCategoriesLayout = new QVBoxLayout(expensesCategoriesWidget);
    QVBoxLayout* earnedCategoriesLayout = new QVBoxLayout(earnedCategoriesWidget);
    expensesCategoriesLayout->addStretch();
    earnedCategoriesLayout->addStretch();
    if(aTotalInflow>0)
    {
        QWidget *widget = categoryWidgetGeneration(":/resources/img/inflow.png", "Inflow", "#A0A0A0", aTotalInflow, earnedCategoriesWidget);
        earnedCategoriesLayout->addWidget(widget);
        widget->setAttribute(Qt::WA_Hover, true);
        widget->installEventFilter(this);
        widget->setProperty("earnType", "inflow");
        widget->setProperty("baseColor", "#A0A0A0");
        widget->setProperty("hoverColor", "#C1BEBE");
        earnedCategoriesLayout->addStretch();
    }
    if(aTotalIncome>0)
    {
        QWidget *widget = categoryWidgetGeneration(":/resources/img/income.png", "Income", "#767676", aTotalIncome, earnedCategoriesWidget);
        earnedCategoriesLayout->addWidget(widget);
        widget->setAttribute(Qt::WA_Hover, true);
        widget->installEventFilter(this);
        widget->setProperty("earnType", "income");
        widget->setProperty("baseColor", "#767676");
        widget->setProperty("hoverColor", "#939393");
        earnedCategoriesLayout->addStretch();
    }
    if(aTotalRefund>0)
    {
        QWidget *widget = categoryWidgetGeneration(":/resources/img/refund.png", "Refund", "#535353", aTotalRefund, earnedCategoriesWidget);
        earnedCategoriesLayout->addWidget(widget);
        widget->setAttribute(Qt::WA_Hover, true);
        widget->installEventFilter(this);
        widget->setProperty("earnType", "refund");
        widget->setProperty("baseColor", "#535353");
        widget->setProperty("hoverColor", "#737373");
    }
    // Convert a negative value to positive with *-1 and check if there is an expense value
    if(aTotalHealth*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/health.png", "Health", "#B38952", aTotalHealth*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalHousing*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/housing.png", "Housing", "#B3A752", aTotalHousing*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalBank*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/bank.png", "Bank & insurance", "#9EB352", aTotalBank*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalLegal*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/legal.png", "Legal & administrative", "#71B352", aTotalLegal*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalOutflow*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/outflow.png", "Outflow money", "#52B363", aTotalOutflow*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalSaving*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/saving.png", "Saving", "#52B393", aTotalSaving*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalTransport*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/transport.png", "Transport", "#52A6B3", aTotalTransport*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalTaxes*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/taxes.png", "Taxes", "#527DB3", aTotalTaxes*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalShopping*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/shopping.png", "Shopping & services", "#5254B3", aTotalShopping*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalEducation*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/education.png", "Education & family", "#7B52B3", aTotalEducation*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalLeisure*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/leisure.png", "Leisure & holidays", "#B352AE", aTotalLeisure*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalExcluded*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/excluded.png", "Excluded transaction", "#B3527C", aTotalExcluded*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }
    if(aTotalFood*-1>0)
    {
        expensesCategoriesLayout->addWidget(categoryWidgetGeneration(":/resources/img/food.png", "Food", "#B35652", aTotalFood*-1, expensesCategoriesWidget));
        expensesCategoriesLayout->addStretch();
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->addWidget(dateLabel);
    layout->setAlignment(dateLabel, Qt::AlignHCenter);
    layout->addWidget(summaryWidget);

    QBarSet *foodBar = new QBarSet("Food");
    QBarSet *excludedBar = new QBarSet("Excluded");
    QBarSet *leisureBar = new QBarSet("Leisure");
    QBarSet *educationBar = new QBarSet("Education");
    QBarSet *shoppingBar = new QBarSet("Shopping");
    QBarSet *taxesBar = new QBarSet("Taxes");
    QBarSet *transportBar = new QBarSet("Transport");
    QBarSet *savingBar = new QBarSet("Saving");
    QBarSet *outflowBar = new QBarSet("Outflow");
    QBarSet *legalBar = new QBarSet("Legal");
    QBarSet *bankBar = new QBarSet("Bank");
    QBarSet *housingBar = new QBarSet("Housing");
    QBarSet *healthBar = new QBarSet("Health");
    QBarSet *refundBar = new QBarSet("Refund");
    QBarSet *incomeBar = new QBarSet("Income");
    QBarSet *inflowBar = new QBarSet("Inflow");

    foodBar->setColor("#B35652");
    excludedBar->setColor("#B3527C");
    leisureBar->setColor("#B352AE");
    educationBar->setColor("#7B52B3");
    shoppingBar->setColor("#5254B3");
    taxesBar->setColor("#527DB3");
    transportBar->setColor("#52A6B3");
    savingBar->setColor("#52B393");
    outflowBar->setColor("#52B363");
    legalBar->setColor("#71B352");
    bankBar->setColor("#9EB352");
    housingBar->setColor("#B3A752");
    healthBar->setColor("#B38952");
    refundBar->setColor("#535353");
    incomeBar->setColor("#767676");
    inflowBar->setColor("#A0A0A0");

    // Put expense in the left stacked bar and earn in the right stacked bar
    *foodBar<<aTotalFood*-1<<0;
    *excludedBar<<aTotalExcluded*-1<<0;
    *leisureBar<<aTotalLeisure*-1<<0;
    *educationBar<<aTotalEducation*-1<<0;
    *shoppingBar<<aTotalShopping*-1<<0;
    *taxesBar<<aTotalTaxes*-1<<0;
    *transportBar<<aTotalTransport*-1<<0;
    *savingBar<<aTotalSaving*-1<<0;
    *outflowBar<<aTotalOutflow*-1<<0;
    *legalBar<<aTotalLegal*-1<<0;
    *bankBar<<aTotalBank*-1<<0;
    *housingBar<<aTotalHousing*-1<<0;
    *healthBar<<aTotalHealth*-1<<0;
    *refundBar<<0<<aTotalRefund;
    *incomeBar<<0<<aTotalIncome;
    *inflowBar<<0<<aTotalInflow;

    QStackedBarSeries *series = new QStackedBarSeries();
    series->append(foodBar);
    series->append(excludedBar);
    series->append(leisureBar);
    series->append(educationBar);
    series->append(shoppingBar);
    series->append(taxesBar);
    series->append(transportBar);
    series->append(savingBar);
    series->append(outflowBar);
    series->append(legalBar);
    series->append(bankBar);
    series->append(housingBar);
    series->append(healthBar);
    series->append(refundBar);
    series->append(incomeBar);
    series->append(inflowBar);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundRoundness(0);
    chart->setBackgroundBrush(Qt::NoBrush);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setContentsMargins(0, 0, 0, 0);

    QStringList categories;
    categories<<"Expended"<<"Earned";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsBrush(QBrush(Qt::white));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelsBrush(QBrush(Qt::white));

    int max;
    int totalEarn = aTotalIncome + aTotalInflow + aTotalRefund;
    max = (totalEarn > totalExpense) ? totalEarn : totalExpense;

    axisY->setRange(0, max);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumWidth(330);
    chartView->setStyleSheet("background: transparent");
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* inforgraphicWidget = new QWidget(this);
    QHBoxLayout* infographicLayout = new QHBoxLayout(inforgraphicWidget);
    infographicLayout->addWidget(expensesCategoriesWidget);
    infographicLayout->addWidget(chartView);
    infographicLayout->addWidget(earnedCategoriesWidget);
    layout->addWidget(inforgraphicWidget);
}

bool DetailWindow::eventFilter(QObject *obj, QEvent *event)
{
    DetailWindow *detailWindow;
    QString baseColor = qobject_cast<QWidget*>(obj)->property("baseColor").toString();
    QString hoverColor = qobject_cast<QWidget*>(obj)->property("hoverColor").toString();
    switch (event->type()) {
    case QEvent::Enter:
        qobject_cast<QWidget*>(obj)->setStyleSheet(QString("background-color: %1;"
                                                            "border-radius: 10px;").arg(hoverColor));
        return true;
    case QEvent::Leave:
        qobject_cast<QWidget*>(obj)->setStyleSheet(QString("background-color: %1;"
                                                            "border-radius: 10px;").arg(baseColor));
        return true;
    case QEvent::MouseButtonPress:
        if(qobject_cast<QWidget*>(obj)->property("earnType") == "income")
            detailWindow = new DetailWindow(nullptr, incomeExpenses, -1);
        if(qobject_cast<QWidget*>(obj)->property("earnType") == "inflow")
            detailWindow = new DetailWindow(nullptr, inflowExpenses, -1);
        if(qobject_cast<QWidget*>(obj)->property("earnType") == "refund")
            detailWindow = new DetailWindow(nullptr, refundExpenses, -1);
        detailWindow->show();
        return true;
    default:
        break;
    }
}

QWidget* DetailWindow::categoryWidgetGeneration(QString aPathImg, QString aCategoryName, QString aColor, int aTotalFood, QWidget* aCategoriesWidget)
{
    QWidget* categoryWidget = new QWidget(aCategoriesWidget);
    categoryWidget->setObjectName("categoryWidget");
    QHBoxLayout* categoryLayout = new QHBoxLayout(categoryWidget);
    QLabel *categoryIcon = new QLabel(categoryWidget);
    QPixmap categoryImg(aPathImg);
    QSize size(20,20);
    categoryImg = categoryImg.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    categoryIcon->setPixmap(categoryImg);
    categoryIcon->setAlignment(Qt::AlignCenter);
    categoryLayout->addWidget(categoryIcon);
    QLabel* categoryLabel = new QLabel(aCategoryName);
    categoryLabel->setObjectName("categoryLabel");
    categoryLabel->setStyleSheet("#categoryLabel{"
                                 "color: #D9D9D9;"
                                 "font-family: Roboto Medium;"
                                 "font-size: 15px;}");
    categoryLayout->addWidget(categoryLabel);
    QLabel* valueLabel = new QLabel(QString::number(aTotalFood));
    valueLabel->setObjectName("valueLabel");
    valueLabel->setStyleSheet("#valueLabel{"
                                 "color: #D9D9D9;"
                                 "font-family: Roboto Medium;"
                                 "font-size: 15px;}");
    categoryLayout->addStretch();
    categoryWidget->setFixedHeight(40);
    categoryWidget->setStyleSheet(QString("#categoryWidget{"
                                          "border-radius: 10px;"
                                          "background-color : %1;}").arg(aColor));
    categoryLayout->addWidget(valueLabel);
    return categoryWidget;
}

DetailWindow::~DetailWindow()
{

}

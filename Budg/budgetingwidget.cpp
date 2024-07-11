#include "budgetingwidget.h"
#include <QDoubleValidator>
#include <QEvent>
#include <QWidget>
#include <QFile>
#include <QMessageBox>

BudgetingWidget::BudgetingWidget(const QString &aImgPath, const QString &aName, const QString &aColor, QWidget *aParent, QPieSlice *aSlice, float aValue, QLabel* aExpenseExempleLabel){
    imgPath = aImgPath;
    name = aName;
    color = aColor;
    parent = aParent;
    slice = aSlice;
    value = aValue;
    expenseExempleLabel = aExpenseExempleLabel;
    setMouseTracking(true);
    setUp();
}

void BudgetingWidget::setUp()
{
    QVBoxLayout *budgetingCategoryLayout = new QVBoxLayout(this);

    QLabel *iconLabel = new QLabel(this);
    QPixmap categoryImg(imgPath);
    QSize size(30,30);
    categoryImg = categoryImg.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel->setPixmap(categoryImg);
    iconLabel->setAlignment(Qt::AlignCenter);
    budgetingCategoryLayout->addWidget(iconLabel);

    QLabel *categoryLabel = new QLabel(name, this);
    categoryLabel->setAlignment(Qt::AlignCenter);
    budgetingCategoryLayout->addWidget(categoryLabel);

    lineEdit = new QLineEdit(this);
    lineEdit->setText(QString::number(value));
    slice->setValue(value);
    lineEdit->setAlignment(Qt::AlignCenter);
    budgetingCategoryLayout->addWidget(lineEdit);

    QDoubleValidator *doubleValidator = new QDoubleValidator(0.00, std::numeric_limits<double>::max(), 2, lineEdit);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    lineEdit->setValidator(doubleValidator);

    percentageLabel = new QLabel("0%", this);
    percentageLabel->setAlignment(Qt::AlignCenter);
    budgetingCategoryLayout->addWidget(percentageLabel);

    setFixedSize(120, 140);

    setObjectName("budgetingCategoryWidget");
    setStyleSheet(QString("#budgetingCategoryWidget{"
                          "background-color: %1;"
                          "border-radius: 10px; }").arg(color));
    categoryLabel->setStyleSheet("font-family: Roboto Thin;"
                                 "font-size: 15px;"
                                 "color: #FFFFFF;");
    lineEdit->setStyleSheet("background-color: #3D3558;"
                            "border-radius: 5px;"
                            "font-family: Roboto Medium;"
                            "color: #FFFFFF;"
                            "font-size: 15px;");
    percentageLabel->setStyleSheet("color: white;"
                                   "font-family: Roboto Light;"
                                   "font-size: 15px;");

    //Update pie chart slice and percentage each time value is edit
    connect(lineEdit, &QLineEdit::editingFinished, [this]() {
        for(int i=0; i<budgetingWidgets.count(); i++)
            budgetingWidgets[i]->updateSlice();
        for(int i=0; i<budgetingWidgets.count(); i++)
            budgetingWidgets[i]->updatePercentageLabel();
    });
}

//highlight pie chart slice when mouse hover
bool BudgetingWidget::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        if(value>0)
            slice->setExploded(true);
        updateExpenseExempleLabel();
        return false;
    case QEvent::Leave:
        slice->setExploded(false);
        expenseExempleLabel->setText("");
        return false;
    default:
        return QWidget::event(event);
    }
}

void BudgetingWidget::updateExpenseExempleLabel()
{
    if(name=="Food\n")
        expenseExempleLabel->setText("Supermarket, restaurant, bakery, ...");
    if(name=="Excluded\ntransaction")
        expenseExempleLabel->setText("Internal transfer, ...");
    if(name=="Leisure &\nholidays")
        expenseExempleLabel->setText("Bar, museum, sport");
    if(name=="Education &\nfamily")
        expenseExempleLabel->setText("Childcare, academic support, training, ...");
    if(name=="Shopping &\nservices")
        expenseExempleLabel->setText("Hairdresser, clothing, gift, ...");
    if(name=="Taxes &\nduties")
        expenseExempleLabel->setText("Social contribution, income tax, property tax, ...");
    if(name=="Transport\n")
        expenseExempleLabel->setText("Car insurance, toll, public transport, ...");
    if(name=="Saving\n")
        expenseExempleLabel->setText("Financial investment, saving account, housing savings, ...");
    if(name=="Outflow of\nmoney")
        expenseExempleLabel->setText("Transfer, cash withdrawal, ...");
    if(name=="Legal &\nadministrative")
        expenseExempleLabel->setText("Lawyers, bailiffs' fees, alimony, ...");
    if(name=="Bank &\ninsurance")
        expenseExempleLabel->setText("Credit, bank charges, securities, ...");
    if(name=="Housing\n")
        expenseExempleLabel->setText("Rent, internet subscription, electricity bills, ...");
    if(name=="Health\n")
        expenseExempleLabel->setText("Doctor, mutual insurance, pharmacy, ...");
}

void BudgetingWidget::updateSlicesAndPercentageLabels()
{
    for(int i=0; i<budgetingWidgets.count(); i++)
        budgetingWidgets[i]->updateSlice();
    for(int i=0; i<budgetingWidgets.count(); i++)
        budgetingWidgets[i]->updatePercentageLabel();
}

void BudgetingWidget::updateSlice()
{
    value = lineEdit->text().toFloat();
    slice->setValue(value);
}

void BudgetingWidget::updatePercentageLabel()
{
    percentageLabel->setText("~" + QString::number(round(slice->percentage()*1000)/10) + "%");
}

void BudgetingWidget::setBudgetingWidgets(QList<BudgetingWidget*>aBudgetingWidgets)
{
    budgetingWidgets = aBudgetingWidgets;
}

float BudgetingWidget::getValue()
{
    return value;
}

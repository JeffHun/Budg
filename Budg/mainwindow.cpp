#include "mainwindow.h"
#include "budgetingwidget.h"

#include <QTransform>
#include <QHoverEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainWidget = new QWidget(this);
    mainWidget->setObjectName("mainWidget");
    setCentralWidget(mainWidget);

    setAcceptDrops(true);

    settings = new QSettings("JeffHun", "Budg");

    fileUploadPageGeneration();
    fileNbr = 0;
}

void MainWindow::fileUploadPageGeneration()
{
    mainLayout = new QVBoxLayout(mainWidget);
    QLabel *titleLabel = new QLabel("Budg", mainWidget);
    titleLabel->setObjectName("titleLabel");
    QLabel *descriptionLabel = new QLabel("Facilitate managing your finances by automating the\nvisualization of your expenses. Understand where your\nmoney is being spent to track your budget.", mainWidget);
    descriptionLabel->setObjectName("descriptionLabel");

    QWidget *mainDropWidget = new QWidget(mainWidget);
    mainDropWidget->setObjectName("mainDropWidget");
    mainDropWidget->setFixedHeight(200);
    mainDropWidget->setFixedWidth(550);
    QHBoxLayout *mainDropLayout = new QHBoxLayout(mainDropWidget);
    mainDropWidget->setLayout(mainDropLayout);

    dropWidget = new QWidget(mainWidget);
    dropWidget->setObjectName("dropWidget");

    scrollArea = new QScrollArea(mainWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("scrollArea");
    scrollWidget = new QWidget(scrollArea);
    scrollWidget->setObjectName("scrollWidget");
    scrollLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);

    mainDropLayout->addWidget(dropWidget);
    mainDropLayout->addWidget(scrollArea);

    setBudgetsButton = new QPushButton("Set budgets", mainWidget);
    connect(setBudgetsButton, &QPushButton::clicked, this, [=](){
        emit processTheseFiles(files);
    });
    setBudgetsButton->setObjectName("setBudgetsButton");
    mainWidget->setLayout(mainLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->setAlignment(titleLabel, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(descriptionLabel);
    mainLayout->setAlignment(descriptionLabel, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(mainDropWidget);
    mainLayout->setAlignment(mainDropWidget, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(setBudgetsButton);
    mainLayout->setAlignment(setBudgetsButton, Qt::AlignHCenter);
    mainLayout->addStretch();

    QVBoxLayout *dropLayout = new QVBoxLayout(dropWidget);
    QLabel *dropLabel = new QLabel("Drag and drop to upload file", dropWidget);
    dropLabel->setObjectName("dropLabel");
    QLabel *dropLabel2 = new QLabel("or", dropWidget);
    dropLabel2->setObjectName("dropLabel2");
    QPushButton *dropButton = new QPushButton("Browse file",dropWidget);
    connect(dropButton, &QPushButton::clicked, this, &MainWindow::browseFile);
    dropButton->setObjectName("dropButton");
    dropWidget->setLayout(dropLayout);
    dropLayout->addWidget(dropLabel);
    dropLayout->setAlignment(dropLabel, Qt::AlignHCenter);
    dropLayout->addWidget(dropLabel2);
    dropLayout->setAlignment(dropLabel2, Qt::AlignHCenter);
    dropLayout->addWidget(dropButton);

    mainWidget->setStyleSheet("#mainWidget{background-color : #15131B;}");
    titleLabel->setStyleSheet("#titleLabel {"
                              "color: #D9D9D9;"
                              "font-family: Roboto Black;"
                              "font-size: 50px;}");
    descriptionLabel->setStyleSheet("#descriptionLabel{"
                                    "color: #A4A4A4;"
                                    "font-family: Roboto Light;"
                                    "font-size: 15px;}");
    dropWidget->setStyleSheet("#dropWidget{"
                              "border: 5px dashed #6954D7;"
                              "border-radius: 10px;}");
    scrollWidget->setStyleSheet("#scrollWidget{"
                                "background-color: #15131B;}");
    scrollArea->setStyleSheet("#scrollArea{"
                              "border: none;}"
                              "QScrollBar:vertical{"
                              "border: none;"
                              "background: #3D3558;"
                              "width: 10px;"
                              "border-radius: 5px;}"
                              "QScrollBar::handle:vertical{"
                              "background: #6954D7;"
                              "border-radius: 5px;}"
                              "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical{"
                              "background: none;"
                              "border: none;}"
                              "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                              "background: none;}");
    dropLabel->setStyleSheet("#dropLabel{"
                             "color: #D9D9D9;"
                             "font-family: Roboto Medium;"
                             "font-size: 20px;"
                             "margin: 20px 20px 0px 20px;}");
    dropLabel2->setStyleSheet("#dropLabel2 {"
                              "color: #D9D9D9;"
                              "font-family: Roboto Medium;"
                              "font-size: 20px;}");
    dropButton->setStyleSheet("#dropButton{"
                              "background-color: #6954D7;"
                              "border-radius: 10px;"
                              "font-family: Roboto Medium;"
                              "font-size: 20px;"
                              "color: #D9D9D9;"
                              "padding: 10px 30px;"
                              "margin: 20px;}"
                              "#dropButton:hover{"
                              "background-color: #5C44DD}");
    setBudgetsButton->setStyleSheet("#setBudgetsButton{"
                                    "background-color: #6954D7;"
                                    "border-radius: 10px;"
                                    "font-family: Roboto Medium;"
                                    "font-size: 20px;"
                                    "color: #D9D9D9;"
                                    "padding: 10px 100px;"
                                    "margin: 20px}"
                                    "#setBudgetsButton:hover{"
                                    "background-color: #5C44DD}");
    setBudgetsButton->setVisible(false);
    scrollArea->setVisible(false);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() && isAllFilesAreCSV(event->mimeData()->urls())) {
        event->acceptProposedAction();
        //change the visual to get feedback
        dropWidget->setStyleSheet("#dropWidget{"
                                  "border: 5px dashed #6954D7;"
                                  "background-color: #3D3558;"
                                  "border-radius: 10px;}");
    } else {
        event->ignore();
    }
}

bool MainWindow::isAllFilesAreCSV(const QList<QUrl> &urls) const
{
    int nbrCSVFile = 0;
    for (const QUrl &url : urls) {
        if (url.toLocalFile().endsWith(".csv", Qt::CaseInsensitive))
            nbrCSVFile++;
    }
    return nbrCSVFile == urls.length();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    //change the visual to get feedback
    dropWidget->setStyleSheet("#dropWidget{"
                              "border: 5px dashed #6954D7;"
                              "border-radius: 10px;}");
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        foreach(QUrl url, urls) {
            QString fileName = url.toLocalFile();
            QFile* aFile = new QFile(fileName);
            addFile(aFile);
        }
        event->acceptProposedAction();
    }

    //change the visual to get feedback
    dropWidget->setStyleSheet("#dropWidget{"
                              "border: 5px dashed #6954D7;"
                              "border-radius: 10px;}");
}

void MainWindow::browseFile()
{
    QString filter = "CSV file (*.csv)";
    QStringList file_names = QFileDialog::getOpenFileNames(this, "Open file", "C:/Users/user/Downloads", filter);
    foreach(const QString &file_name, file_names){
        if (!file_name.isEmpty()) {
            QFile* aFile = new QFile(file_name);
            addFile(aFile);
        }
    }
}

void MainWindow::addFile(QFile* aFile)
{
    bool isNewFile = true;
    foreach(QFile* file, files)
    {
        if(file->fileName() == aFile->fileName())
            isNewFile = false;
    }
    if(isNewFile)
    {
        fileNbr++;
        QWidget *fileWidget = new QWidget(scrollWidget);
        fileWidget->setObjectName("fileWidget");
        fileWidget->setFixedHeight(40);
        QHBoxLayout *fileLayout = new QHBoxLayout(fileWidget);
        fileWidget->setLayout(fileLayout);

        QPushButton *fileButton = new QPushButton("X", fileWidget);
        fileButton->setFixedWidth(20);
        fileButton->setObjectName("fileButton");
        connect(fileButton, &QPushButton::clicked, this, [=]() {
            delete fileWidget;
            fileNbr--;

            //remove the file from the list
            int index = files.indexOf(aFile);
            if (index != -1) {
                delete files.takeAt(index);
            }

            setBudgetsButton->setVisible(fileNbr>0);
            scrollArea->setVisible(fileNbr>0);
        });

        //remove the extension from the file name and reduce the number of characters to 18
        QString fileName = aFile->fileName().section('/', -1);
        QLabel *fileLabel = new QLabel(fileName, fileWidget);
        fileLabel->setObjectName("fileLabel");
        for(int i = 0; i<fileLabel->text().length(); i++){
            if(fileLabel->text()[i] == '.')
            {
                fileLabel->setText(fileLabel->text().left(i));
                break;
            }
        }
        if (fileLabel->text().length() > 18)
            fileLabel->setText(fileLabel->text().left(18));

        fileLayout->addWidget(fileButton);
        fileLayout->addWidget(fileLabel);
        scrollLayout->addWidget(fileWidget);
        fileWidget->setStyleSheet("#fileWidget{"
                                  "background-color: #3D3558;"
                                  "border-radius: 10px;}"
                                  "#fileWidget:hover{"
                                  "background-color: #6954D7;}");
        fileLabel->setStyleSheet("#fileLabel {"
                                 "font-family: Roboto Light;"
                                 "font-size: 15px;"
                                 "color: white;}");
        fileButton->setStyleSheet("#fileButton{"
                                  "width: 20px;"
                                  "height: 20px;"
                                  "font-family: Roboto Black;"
                                  "border: none;"
                                  "background-color: #FFFFFF;"
                                  "border-radius: 10px;"
                                  "padding: 0px;"
                                  "color: #DB7A76;}"
                                  "#fileButton:hover{"
                                  "background: #C2C2C2;}");

        setBudgetsButton->setVisible(fileNbr>0);
        scrollArea->setVisible(fileNbr>0);
        files.append(aFile);
    }
}

void MainWindow::setMonths(QList<Month*> aMonths)
{
    months = aMonths;
    getBudgetCategory();
    budgetingPageGeneration();
    currentYear = months[0]->getYear();
}





void MainWindow::budgetingPageGeneration()
{
    delete mainWidget;
    mainWidget = nullptr;
    mainWidget = new QWidget(this);
    mainWidget->setObjectName("mainWidget");
    setCentralWidget(mainWidget);
    mainWidget->setStyleSheet("#mainWidget{background-color : #15131B;}");

    mainLayout = new QVBoxLayout(mainWidget);
    QLabel *titleLabel = new QLabel("Budg", mainWidget);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setStyleSheet("#titleLabel{"
                              "font-family: Roboto Black;"
                              "font-size: 50px;"
                              "color: #D9D9D9;}");
    QLabel *descriptionLabel = new QLabel("Are these budget values right for you ?", mainWidget);
    descriptionLabel->setObjectName("descriptionLabel");
    descriptionLabel->setStyleSheet("#descriptionLabel{"
                                    "font-family: Roboto Light;"
                                    "font-size: 15px;"
                                    "color: #A4A4A4;}");
    mainWidget->setLayout(mainLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->setAlignment(titleLabel, Qt::AlignHCenter);
    mainLayout->addWidget(descriptionLabel);
    mainLayout->setAlignment(descriptionLabel, Qt::AlignHCenter);
    mainLayout->addStretch();

    QWidget *budgetingWidget = new QWidget(mainWidget);
    QHBoxLayout *budgetingLayout = new QHBoxLayout(budgetingWidget);
    budgetingWidget->setLayout(budgetingLayout);
    mainLayout->addWidget(budgetingWidget);
    mainLayout->addStretch();

    QWidget *budgetingCategoriesWidget = new QWidget(budgetingWidget);
    QGridLayout *budgetingCategoriesLayout = new QGridLayout(budgetingCategoriesWidget);

    QPieSeries *series = new QPieSeries();
    series->append("", 0)->setBrush(QColor(179, 86, 82));
    series->append("", 0)->setBrush(QColor(179, 82, 124));
    series->append("", 0)->setBrush(QColor(179, 82, 174));
    series->append("", 0)->setBrush(QColor(123, 82, 179));
    series->append("", 0)->setBrush(QColor(82, 84, 179));
    series->append("", 0)->setBrush(QColor(82, 125, 179));
    series->append("", 0)->setBrush(QColor(82, 166, 179));
    series->append("", 0)->setBrush(QColor(82, 179, 147));
    series->append("", 0)->setBrush(QColor(82, 179, 99));
    series->append("", 0)->setBrush(QColor(113, 179, 82));
    series->append("", 0)->setBrush(QColor(158, 179, 82));
    series->append("", 0)->setBrush(QColor(179, 167, 82));
    series->append("", 0)->setBrush(QColor(179, 137, 82));
    series->append("", 0)->setBrush(QColor(179, 108, 82));
    series->setLabelsVisible(false);
    series->setPieSize(0.8);

    QLabel *expenseExempleLabel = new QLabel("", mainWidget);
    expenseExempleLabel->setObjectName("expenseExempleLabel");
    expenseExempleLabel->setStyleSheet("#expenseExempleLabel{"
                                       "font-family: Roboto Light;"
                                       "font-size: 15px;"
                                       "color: #D9D9D9;}");

    BudgetingWidget *food = new BudgetingWidget(":/resources/img/food.png", "Food\n", "#B35652", budgetingCategoriesWidget, series->slices().at(0),budgetCategoryValues[0], expenseExempleLabel);
    BudgetingWidget *excluded = new BudgetingWidget(":/resources/img/excluded.png", "Excluded\ntransaction", "#B3527C", budgetingCategoriesWidget, series->slices().at(1),budgetCategoryValues[1], expenseExempleLabel);
    BudgetingWidget *leisure = new BudgetingWidget(":/resources/img/leisure.png", "Leisure &\nholidays", "#B352AE", budgetingCategoriesWidget, series->slices().at(2),budgetCategoryValues[2], expenseExempleLabel);
    BudgetingWidget *education = new BudgetingWidget(":/resources/img/education.png", "Education &\nfamily", "#7B52B3", budgetingCategoriesWidget, series->slices().at(3),budgetCategoryValues[3], expenseExempleLabel);
    BudgetingWidget *shopping = new BudgetingWidget(":/resources/img/shopping.png", "Shopping &\nservices", "#5254B3", budgetingCategoriesWidget, series->slices().at(4),budgetCategoryValues[4], expenseExempleLabel);
    BudgetingWidget *taxes = new BudgetingWidget(":/resources/img/taxes.png", "Taxes &\nduties", "#527DB3", budgetingCategoriesWidget, series->slices().at(5),budgetCategoryValues[5], expenseExempleLabel);
    BudgetingWidget *transport = new BudgetingWidget(":/resources/img/transport.png", "Transport\n", "#52A6B3", budgetingCategoriesWidget, series->slices().at(6),budgetCategoryValues[6], expenseExempleLabel);
    BudgetingWidget *saving = new BudgetingWidget(":/resources/img/saving.png", "Saving\n", "#52B393", budgetingCategoriesWidget, series->slices().at(7),budgetCategoryValues[7], expenseExempleLabel);
    BudgetingWidget *outflow = new BudgetingWidget(":/resources/img/outflow.png", "Outflow of\nmoney", "#52B363", budgetingCategoriesWidget, series->slices().at(8),budgetCategoryValues[8], expenseExempleLabel);
    BudgetingWidget *legal = new BudgetingWidget(":/resources/img/legal.png", "Legal &\nadministrative", "#71B352", budgetingCategoriesWidget, series->slices().at(9),budgetCategoryValues[9], expenseExempleLabel);
    BudgetingWidget *bank = new BudgetingWidget(":/resources/img/bank.png", "Bank &\ninsurance", "#9EB352", budgetingCategoriesWidget, series->slices().at(10),budgetCategoryValues[10], expenseExempleLabel);
    BudgetingWidget *housing = new BudgetingWidget(":/resources/img/housing.png", "Housing\n", "#B3A752", budgetingCategoriesWidget, series->slices().at(11),budgetCategoryValues[11], expenseExempleLabel);
    BudgetingWidget *health = new BudgetingWidget(":/resources/img/health.png", "Health\n", "#B38952", budgetingCategoriesWidget, series->slices().at(12),budgetCategoryValues[12], expenseExempleLabel);

    QList<BudgetingWidget*> budgetingWidgets;
    budgetingWidgets.append(food);
    budgetingWidgets.append(excluded);
    budgetingWidgets.append(leisure);
    budgetingWidgets.append(education);
    budgetingWidgets.append(shopping);
    budgetingWidgets.append(taxes);
    budgetingWidgets.append(transport);
    budgetingWidgets.append(saving);
    budgetingWidgets.append(outflow);
    budgetingWidgets.append(legal);
    budgetingWidgets.append(bank);
    budgetingWidgets.append(housing);
    budgetingWidgets.append(health);

    food->setBudgetingWidgets(budgetingWidgets);
    excluded->setBudgetingWidgets(budgetingWidgets);
    leisure->setBudgetingWidgets(budgetingWidgets);
    education->setBudgetingWidgets(budgetingWidgets);
    shopping->setBudgetingWidgets(budgetingWidgets);
    taxes->setBudgetingWidgets(budgetingWidgets);
    transport->setBudgetingWidgets(budgetingWidgets);
    saving->setBudgetingWidgets(budgetingWidgets);
    outflow->setBudgetingWidgets(budgetingWidgets);
    legal->setBudgetingWidgets(budgetingWidgets);
    bank->setBudgetingWidgets(budgetingWidgets);
    housing->setBudgetingWidgets(budgetingWidgets);
    health->setBudgetingWidgets(budgetingWidgets);

    food->updateSlicesAndPercentageLabels();

    budgetingCategoriesLayout->addWidget(food, 0, 0);
    budgetingCategoriesLayout->addWidget(excluded, 0, 1);
    budgetingCategoriesLayout->addWidget(leisure, 0, 2);
    budgetingCategoriesLayout->addWidget(education, 0, 3);
    budgetingCategoriesLayout->addWidget(shopping, 0, 4);
    budgetingCategoriesLayout->addWidget(taxes, 1, 0);
    budgetingCategoriesLayout->addWidget(transport, 1, 1);
    budgetingCategoriesLayout->addWidget(saving, 1, 2);
    budgetingCategoriesLayout->addWidget(outflow, 1, 3);
    budgetingCategoriesLayout->addWidget(legal, 1, 4);
    budgetingCategoriesLayout->addWidget(bank, 2, 0);
    budgetingCategoriesLayout->addWidget(housing, 2, 1);
    budgetingCategoriesLayout->addWidget(health, 2, 2);

    budgetingLayout->addStretch();
    budgetingLayout->addWidget(budgetingCategoriesWidget);
    budgetingLayout->addStretch();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);
    chart->setBackgroundBrush(Qt::NoBrush);

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setStyleSheet("background: transparent");
    chartview->setMinimumSize(430, 430);
    chartview->setContentsMargins(0, 0, 0, 0);
    chartview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *chartContainer = new QWidget(budgetingWidget);
    QVBoxLayout *chartContainerLayout = new QVBoxLayout(chartContainer);
    chartContainerLayout->setContentsMargins(0, 0, 0, 0);
    chartContainerLayout->setSpacing(2);
    chartContainerLayout->addWidget(chartview);
    chartContainer->setLayout(chartContainerLayout);

    budgetingLayout->addWidget(chartContainer);
    budgetingLayout->addStretch();

    QPushButton *analyzeExpensesButton = new QPushButton("Analyze my expenses", mainWidget);
    connect(analyzeExpensesButton, &QPushButton::clicked, this, [=](){
        for(int i = 0; i < budgetingWidgets.count(); i++)
            budgetCategoryValues[i] = budgetingWidgets[i]->getValue();
        setBudgetCategory();
        expenseAnalysisPageGeneration();
    });
    analyzeExpensesButton->setObjectName("analyzeExpensesButton");
    mainLayout->addWidget(expenseExempleLabel);
    mainLayout->setAlignment(expenseExempleLabel, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(analyzeExpensesButton);
    mainLayout->setAlignment(analyzeExpensesButton, Qt::AlignHCenter);
    analyzeExpensesButton->setStyleSheet("#analyzeExpensesButton{"
                                         "background-color: #6954D7;"
                                         "border-radius: 10px;"
                                         "font-family: Roboto Medium;"
                                         "font-size: 20px;"
                                         "color: #D9D9D9;"
                                         "padding: 10px 100px;"
                                         "margin: 20px}"
                                         "#analyzeExpensesButton:hover{"
                                         "background-color: #5C44DD;}");
}

void MainWindow::getBudgetCategory()
{
    QStringList categories = {"food", "excluded", "leisure", "education", "shopping", "taxes", "transport", "saving", "outflow", "legal", "bank", "housing", "health"};
    foreach (const QString &category, categories) {
        if(settings->contains(category))
            budgetCategoryValues.append(settings->value(category).toFloat());
        else {
            settings->setValue(category, "0");
            budgetCategoryValues.append(0.0f);
        }
    }
}

void MainWindow::setBudgetCategory()
{
    settings->setValue("food", QString::number(budgetCategoryValues[0]));
    settings->setValue("excluded", QString::number(budgetCategoryValues[1]));
    settings->setValue("leisure", QString::number(budgetCategoryValues[2]));
    settings->setValue("education", QString::number(budgetCategoryValues[3]));
    settings->setValue("shopping", QString::number(budgetCategoryValues[4]));
    settings->setValue("taxes", QString::number(budgetCategoryValues[5]));
    settings->setValue("transport", QString::number(budgetCategoryValues[6]));
    settings->setValue("saving", QString::number(budgetCategoryValues[7]));
    settings->setValue("outflow", QString::number(budgetCategoryValues[8]));
    settings->setValue("legal", QString::number(budgetCategoryValues[9]));
    settings->setValue("bank", QString::number(budgetCategoryValues[10]));
    settings->setValue("housing", QString::number(budgetCategoryValues[11]));
    settings->setValue("health", QString::number(budgetCategoryValues[12]));
}





void MainWindow::expenseAnalysisPageGeneration()
{
    delete mainWidget;
    mainWidget = nullptr;
    mainWidget = new QWidget(this);
    mainWidget->setObjectName("mainWidget");
    setCentralWidget(mainWidget);
    mainWidget->setStyleSheet("#mainWidget{background-color : #15131B;}");

    QWidget* yearWidget = new QWidget(mainWidget);
    mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->addWidget(yearWidget);
    mainLayout->setAlignment(yearWidget, Qt::AlignHCenter);
    QHBoxLayout* yearLayout = new QHBoxLayout(yearWidget);

    QPixmap narrow(":/resources/img/narrow.png");
    QPixmap narrowReversed = narrow.transformed(QTransform().scale(-1, 1).translate(-narrow.width(), 0));
    QIcon narrowIcon(narrow);

    bool isPreviousYearExist = false;
    bool isNextYearExist = false;

    for (int i = 0; i < months.count(); i++)
    {
        if (months[i]->getYear() < currentYear)
        {
            isPreviousYearExist = true;
            break;
        }
    }

    for (int i = 0; i < months.count(); i++)
    {
        if (months[i]->getYear() > currentYear)
        {
            isNextYearExist = true;
            break;
        }
    }

    QPushButton* previousYearButton = new QPushButton(yearWidget);
    previousYearButton->setObjectName("previousYearButton");
    previousYearButton->setIcon(narrowIcon);
    QLabel* yearLabel = new QLabel(QString::number(currentYear), yearWidget);
    yearLabel->setObjectName("yearLabel");
    QPushButton* nextYearButton = new QPushButton(yearWidget);
    nextYearButton->setObjectName("nextYearButton");
    QIcon narrowReversedIcon(narrowReversed);
    nextYearButton->setIcon(narrowReversedIcon);

    yearLayout->addWidget(previousYearButton);
    yearLayout->addWidget(yearLabel);
    yearLayout->addWidget(nextYearButton);

    previousYearButton->setFixedSize(40, 40);
    nextYearButton->setFixedSize(40, 40);

    if(isPreviousYearExist)
    {
        previousYearButton->setStyleSheet("#previousYearButton{"
                                          "background-color: #D9D9D9;"
                                          "border-radius: 20px;}"
                                          "#previousYearButton:hover{"
                                          "background-color: #A4A4A4}");
        connect(previousYearButton, &QPushButton::clicked, this, &MainWindow::loadPreviousYear);
    }
    else
    {
        previousYearButton->setStyleSheet("#previousYearButton{"
                                          "background-color: #747474;"
                                          "border-radius: 20px;}");
    }

    if(isNextYearExist)
    {
        nextYearButton->setStyleSheet("#nextYearButton{"
                                          "background-color: #D9D9D9;"
                                          "border-radius: 20px;}"
                                          "#nextYearButton:hover{"
                                          "background-color: #A4A4A4}");
        connect(nextYearButton, &QPushButton::clicked, this, &MainWindow::loadNextYear);
    }
    else
    {
        nextYearButton->setStyleSheet("#nextYearButton{"
                                          "background-color: #747474;"
                                          "border-radius: 20px;}");
    }

    yearLabel->setStyleSheet("#yearLabel{"
                             "color: #D9D9D9;"
                             "font-family: Roboto Black;"
                             "font-size: 30px;"
                             "margin: 0 20px;}");
    monthButtonGeneration();
}

void MainWindow::loadNextYear()
{
    currentYear++;
    selectedMonths.clear();
    clearSummary();
    expenseAnalysisPageGeneration();
}

void MainWindow::loadPreviousYear()
{
    currentYear--;
    selectedMonths.clear();
    clearSummary();
    expenseAnalysisPageGeneration();
}

void MainWindow::clearSummary()
{
    //Remove all widget from summaryLayout exept first line (category widget)
    for (int i = summaryLayout->count() - 1; i >= 0; --i) {
        int row, col, rowSpan, colSpan;
        summaryLayout->getItemPosition(i, &row, &col, &rowSpan, &colSpan);

        if (row != 0) {
            QLayoutItem* item = summaryLayout->takeAt(i);
            if (item) {
                QWidget* widget = item->widget();
                if (widget) {
                    widget->setParent(nullptr);
                    delete widget;
                }
                delete item;
            }
        }
    }
}

void MainWindow::monthButtonGeneration()
{
    QWidget* monthButtonWidget = new QWidget(mainWidget);
    mainLayout->setAlignment(monthButtonWidget, Qt::AlignHCenter);
    monthButtonWidget->setFixedHeight(150);
    QGridLayout* monthLayout = new QGridLayout(monthButtonWidget);
    QList<QString> monthNames;
    monthNames<<""<<"January"<<"February"<<"March"<<"April"<<"May"<<"June"<<"July"<<"August"<<"September"<<"October"<<"November"<<"December";
    int row = 0;
    int collumn = 0;

    //Create clickable buttons if months match the study button
    for(int i = 1; i <13; i++)
    {
        QPushButton* monthButton = new QPushButton(monthNames[i], monthButtonWidget);
        monthButton->setFixedSize(125,40);
        monthButton->setObjectName("monthButton");
        monthButton->setStyleSheet("#monthButton{"
                                   "background-color: #241E38;"
                                   "color: #838383;"
                                   "font-family: Roboto Regular;"
                                   "font-size: 15px;"
                                   "margin: 5px;"
                                   "border-radius: 10px;}");
        for(int j = 0; j < months.count(); j++)
        {
            if(months[j]->getYear()>currentYear)
                break;
            if(months[j]->getYear() == currentYear)
            {
                if(months[j]->getMonthIndex() == i)
                {
                    monthButton->setStyleSheet("#monthButton{"
                                               "background-color: #3D3558;"
                                               "color: #D9D9D9;"
                                               "font-family: Roboto Regular;"
                                               "font-size: 15px;"
                                               "margin: 5px;"
                                               "border-radius: 10px;}"
                                               "#monthButton:hover{"
                                               "background-color: #4C3B88;}");
                    connect(monthButton, &QPushButton::clicked, this, [this, j, monthButton](){
                        selectedThisMonth(months[j], monthButton);
                    });
                }
            }

        }
        if(i==7)
        {
            collumn = 0;
            row = 1;
        }
        monthLayout->addWidget(monthButton, row, collumn);
        collumn++;
    }
    mainLayout->addWidget(monthButtonWidget);
    categoriesSummaryGeneration();
}

void MainWindow::selectedThisMonth(Month* aMonth, QPushButton* aMonthButton)
{
    int index = selectedMonths.indexOf(aMonth);
    if (index == -1) {
        selectedMonths.append(aMonth);
        aMonthButton->setStyleSheet("#monthButton{"
                                    "background-color: #6954D7;"
                                    "color: #D9D9D9;"
                                    "font-family: Roboto Regular;"
                                    "font-size: 15px;"
                                    "margin: 5px;"
                                    "border-radius: 10px;}");
        showMonthSummaryRow(aMonth->getMonthIndex());
    }
    //avoid duplicates
    else
    {
        aMonthButton->setStyleSheet("#monthButton{"
                                    "background-color: #3D3558;"
                                    "color: #D9D9D9;"
                                    "font-family: Roboto Regular;"
                                    "font-size: 15px;"
                                    "margin: 5px;"
                                    "border-radius: 10px;}"
                                    "#monthButton:hover{"
                                    "background-color: #4C3B88;}");
        hideMonthSummaryRow(aMonth->getMonthIndex());
        selectedMonths.takeAt(index);
    }
}

void MainWindow::categoriesSummaryGeneration()
{
    summaryWidget = new QWidget(mainWidget);
    mainLayout->addWidget(summaryWidget);
    summaryLayout = new QGridLayout(summaryWidget);
    summaryLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* spaceTakerWidget = new QWidget(summaryWidget);
    spaceTakerWidget->setFixedWidth(150);

    // a phantom space the size of the months ummary header widget
    summaryLayout->addWidget(spaceTakerWidget, 0,0);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/food.png", "Food"), 0,1);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/excluded.png", "Excluded\ntransaction"), 0,2);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/leisure.png", "Leisure &\nholidays"), 0,3);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/education.png", "Education &\nfamily"), 0,4);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/shopping.png", "Shopping &\nservices"), 0,5);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/taxes.png", "Taxes"), 0,6);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/transport.png", "Transport"), 0,7);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/saving.png", "Saving"), 0,8);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/outflow.png", "Outflow\nmoney"), 0,9);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/legal.png", "Legal a\nadministravite"), 0,10);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/bank.png", "Bank &\ninsurance"), 0,11);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/housing.png", "Housing"), 0,12);
    summaryLayout->addWidget(categorySummaryGeneration(":/resources/img/health.png", "Health"), 0,13);

    mainLayout->addStretch();
    monthSummaryGeneration();
}

QWidget* MainWindow::categorySummaryGeneration(QString aImgPath, QString aCategoryName)
{
    QWidget* categoryWidget = new QWidget(summaryWidget);
    QVBoxLayout* categoryLayout = new QVBoxLayout(categoryWidget);
    QLabel *categoryIcon = new QLabel(categoryWidget);
    QPixmap categoryImg(aImgPath);
    QSize size(20,20);
    categoryImg = categoryImg.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    categoryIcon->setPixmap(categoryImg);
    categoryIcon->setAlignment(Qt::AlignCenter);
    categoryLayout->addWidget(categoryIcon);
    QLabel *categoryLabel = new QLabel(aCategoryName, categoryWidget);
    categoryLayout->setContentsMargins(0, 0, 0, 0);
    categoryLayout->setSpacing(0);
    categoryLabel->setObjectName("categoryLabel");
    categoryLabel->setStyleSheet("#categoryLabel{"
                                 "color: #A4A4A4;"
                                 "font-family: Roboto Light;"
                                 "font-size: 10px;}");
    categoryLabel->setAlignment(Qt::AlignHCenter);
    categoryLayout->addWidget(categoryLabel);

    return categoryWidget;
}

void MainWindow::monthSummaryGeneration()
{
    for(int i = 1; i < 13; i++)
    {
        for(int j = 0; j < months.count(); j++)
        {
            if(months[j]->getMonthIndex() == i && months[j]->getYear() == currentYear)
            {
                summaryLayout->addWidget(monthSummaryHeaderGeneration(months[j]->getMonthIndex(), months[j]->getEndRemaining(), months[j]->getTotalFoodExpenses(), months[j]->getTotalBankExpenses(), months[j]->getTotalEducationExpenses(), months[j]->getTotalTaxesExpenses(), months[j]->getTotalLegalExpenses(), months[j]->getTotalHousingExpenses(), months[j]->getTotalLeisureExpenses(), months[j]->getTotalHealthExpenses(), months[j]->getTotalShoppingExpenses(), months[j]->getTotalExcludedExpenses(), months[j]->getTotalTransportsExpenses(), months[j]->getTotalOutflowExpenses(), months[j]->getTotalSavingExpenses(), months[j]->getTotalIncomeExpenses(), months[j]->getTotalInflowExpenses(), months[j]->getTotalRefundExpenses()), i,0);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalFoodExpenses(), "food", months[j]->getMonthIndex()), i,1);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalExcludedExpenses(), "excluded", months[j]->getMonthIndex()), i,2);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalLeisureExpenses(), "leisure", months[j]->getMonthIndex()), i,3);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalEducationExpenses(), "education", months[j]->getMonthIndex()), i,4);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalShoppingExpenses(), "shopping", months[j]->getMonthIndex()), i,5);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalTaxesExpenses(), "taxes", months[j]->getMonthIndex()), i,6);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalTransportsExpenses(), "transport", months[j]->getMonthIndex()), i,7);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalSavingExpenses(), "saving", months[j]->getMonthIndex()), i,8);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalOutflowExpenses(), "outflow", months[j]->getMonthIndex()), i,9);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalLegalExpenses(), "legal", months[j]->getMonthIndex()), i,10);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalBankExpenses(), "bank", months[j]->getMonthIndex()), i,11);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalHousingExpenses(), "housing", months[j]->getMonthIndex()), i,12);
                summaryLayout->addWidget(monthSummaryCategoryValueGeneration(months[j]->getTotalHealthExpenses(), "health", months[j]->getMonthIndex()), i,13);
                break;
            }
        }
    }
    hideAllMonthSummary();
}

void MainWindow::hideAllMonthSummary()
{
    int rowCount = summaryLayout->rowCount();
    int columnCount = summaryLayout->columnCount();

    // Hide all line from summaryLayout exept first line (category widget)
    for (int row = 1; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            QLayoutItem* item = summaryLayout->itemAtPosition(row, col);
            if (item != nullptr) {
                QWidget* widget = item->widget();
                if (widget != nullptr) {
                    widget->hide();
                }
            }
        }
    }
}

void MainWindow::hideMonthSummaryRow(int row)
{
    // Hide a specific line
    int columns = summaryLayout->columnCount();
    for (int col = 0; col < columns; ++col) {
        QLayoutItem *item = summaryLayout->itemAtPosition(row, col);
        if (item) {
            QWidget *widget = item->widget();
            if (widget) {
                widget->hide();
            }
        }
    }
}

void MainWindow::showMonthSummaryRow(int row)
{
    // Show a specific line
    int columns = summaryLayout->columnCount();
    for (int col = 0; col < columns; ++col) {
        QLayoutItem *item = summaryLayout->itemAtPosition(row, col);
        if (item) {
            QWidget *widget = item->widget();
            if (widget) {
                widget->show();
            }
        }
    }
}

QString MainWindow::getMonthName(int monthIndex) {
    static const QStringList monthNames = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    if (monthIndex < 1 || monthIndex > 12)
        return "Unknown";
    return monthNames.at(monthIndex - 1);
}

QWidget* MainWindow::monthSummaryHeaderGeneration(int monthIndex, int monthEndRemaining, int aTotalFood, int aTotalBank, int aTotalEducation, int aTotalTaxes, int aTotalLegal, int aTotalHousing, int aTotalLeisure, int aTotalHealth, int aTotalShopping, int aTotalExcluded, int aTotalTransport, int aTotalOutflow, int aTotalSaving, int aTotalIncome, int aTotalInflow, int aTotalRefund)
{
    QWidget* monthHeaderWidget = new QWidget(summaryWidget);
    monthHeaderWidget->setObjectName("monthHeaderWidget");
    monthHeaderWidget->setProperty("month index", monthIndex);
    monthHeaderWidget->setProperty("remaining", monthEndRemaining);
    monthHeaderWidget->setProperty("food", aTotalFood);
    monthHeaderWidget->setProperty("bank", aTotalBank);
    monthHeaderWidget->setProperty("education", aTotalEducation);
    monthHeaderWidget->setProperty("taxes", aTotalTaxes);
    monthHeaderWidget->setProperty("legal", aTotalLegal);
    monthHeaderWidget->setProperty("housing", aTotalHousing);
    monthHeaderWidget->setProperty("leisure", aTotalLeisure);
    monthHeaderWidget->setProperty("health", aTotalHealth);
    monthHeaderWidget->setProperty("shopping", aTotalShopping);
    monthHeaderWidget->setProperty("excluded", aTotalExcluded);
    monthHeaderWidget->setProperty("transport", aTotalTransport);
    monthHeaderWidget->setProperty("outflow", aTotalOutflow);
    monthHeaderWidget->setProperty("saving", aTotalSaving);
    monthHeaderWidget->setProperty("income", aTotalIncome);
    monthHeaderWidget->setProperty("inflow", aTotalInflow);
    monthHeaderWidget->setProperty("refund", aTotalRefund);
    monthHeaderWidget->installEventFilter(this);
    monthHeaderWidget->setAttribute(Qt::WA_Hover, true);
    monthHeaderWidget->setFixedWidth(150);
    QString monthName = getMonthName(monthIndex);

    QLabel* monthLabel = new QLabel(monthName, monthHeaderWidget);
    monthLabel->setObjectName("monthLabel");
    monthLabel->setStyleSheet("#monthLabel{"
                              "font-family: Roboto Bold;"
                              "font-size: 15px;"
                              "color: #D9D9D9}");
    QString digitalSymbol;
    QLabel* monthEndRemainingLabel = new QLabel(monthHeaderWidget);
    monthEndRemainingLabel->setObjectName("monthEndRemainingLabel");
    monthEndRemainingLabel->setStyleSheet("#monthEndRemainingLabel{"
                                          "font-family: Roboto Black;"
                                          "font-size: 15px;"
                                          "color: #D9D9D9}");
    if(monthEndRemaining>0)
    {
        digitalSymbol = "+";
        monthEndRemainingLabel->setText(digitalSymbol + QString::number(monthEndRemaining));
        monthEndRemainingLabel->setStyleSheet("#monthEndRemainingLabel{"
                                              "background-color: #76DB83;"
                                              "border-radius: 10px;"
                                              "font-family: Roboto Black;"
                                              "font-size: 15px;"
                                              "color: #D9D9D9;"
                                              "padding: 2px}");
    }
    else
    {
        digitalSymbol = "";
        monthEndRemainingLabel->setText(digitalSymbol + QString::number(monthEndRemaining));
        monthEndRemainingLabel->setStyleSheet("#monthEndRemainingLabel{"
                                              "background-color: #DB7A76;"
                                              "border-radius: 10px;"
                                              "font-family: Roboto Black;"
                                              "font-size: 15px;"
                                              "color: #D9D9D9;"
                                              "padding: 2px}");
    }
    QHBoxLayout* monthEndRemainingLayout = new QHBoxLayout(monthHeaderWidget);
    monthEndRemainingLayout->addWidget(monthLabel);
    monthEndRemainingLayout->addStretch();
    monthEndRemainingLayout->addWidget(monthEndRemainingLabel);

    return monthHeaderWidget;
}

QWidget* MainWindow::monthSummaryCategoryValueGeneration(int aValue, QString aCategory, int aMonthIndex)
{
    QWidget* categoryValueWidget = new QWidget(summaryWidget);
    // If there are no expenses and no budget, return an empty widget
    if(settings->value(aCategory).toFloat() == 0 && aValue == 0)
        return categoryValueWidget;
    if(aValue != 0)
        categoryValueWidget->setAttribute(Qt::WA_Hover, true);
    aValue += settings->value(aCategory).toFloat();
    categoryValueWidget->setObjectName("categoryValueWidget");
    categoryValueWidget->setProperty("category", aCategory);
    categoryValueWidget->setProperty("month index", aMonthIndex);
    categoryValueWidget->installEventFilter(this);
    QString digitalSymbol;
    QColor color;
    QImage img(":/resources/img/arrow.png");
    if(aValue >0)
    {
        color = QColor(118, 219, 131);
        img = img.mirrored(false, true);
        digitalSymbol = "+";
    }
    if(aValue == 0)
    {
        img.load(":/resources/img/egal.png");
        color = QColor(131, 131, 131);
    }
    if(aValue < 0)
    {
        color = QColor(219, 122, 118);
    }
    QLabel* valueLabel = new QLabel(digitalSymbol + QString::number(aValue), categoryValueWidget);
    valueLabel->setObjectName("valueLabel");
    valueLabel->setStyleSheet("#valueLabel{"
                              "font-family: Roboto Light;"
                              "color: #D9D9D9;"
                              "font-size: 15px;}");

    QLabel *valueIcon = new QLabel(categoryValueWidget);
    QSize size(20,20);
    for (int y = 0; y < img.height(); ++y)
    {
        for (int x = 0; x < img.width(); ++x)
        {
            QColor pixelColor = img.pixelColor(x, y);
            if (pixelColor.alpha() > 0)
            {
                pixelColor.setRed(color.red());
                pixelColor.setGreen(color.green());
                pixelColor.setBlue(color.blue());
                img.setPixelColor(x, y, pixelColor);
            }
        }
    }
    QPixmap pixmap = QPixmap::fromImage(img);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    valueIcon->setPixmap(pixmap);
    valueIcon->setAlignment(Qt::AlignCenter);
    QHBoxLayout* categoryValueLayout = new QHBoxLayout(categoryValueWidget);
    categoryValueLayout->addStretch();
    categoryValueLayout->addWidget(valueLabel);
    categoryValueLayout->addWidget(valueIcon);
    categoryValueLayout->addStretch();
    categoryValueWidget->setFixedWidth(80);
    return categoryValueWidget;
}

QString MainWindow::convertCategoryToString(const Expense::Category& category)
{
    static const QMap<Expense::Category, QString> categoryMap = {
        {Expense::Category::Food, "food"},
        {Expense::Category::BankAndInsurance, "bank"},
        {Expense::Category::EducationAndFamily, "education"},
        {Expense::Category::Saving, "saving"},
        {Expense::Category::TaxesAndDuties, "taxes"},
        {Expense::Category::LegalAndAdministrative, "legal"},
        {Expense::Category::HousingAndHome, "housing"},
        {Expense::Category::LeisureAndVacations, "leisure"},
        {Expense::Category::Health, "health"},
        {Expense::Category::ShoppingAndServices, "shopping"},
        {Expense::Category::ExcludedAndTransaction, "excluded"},
        {Expense::Category::Transports, "transport"},
        {Expense::Category::OutflowOfMoney, "outflow"}
    };
    return categoryMap.value(category);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->objectName() == "categoryValueWidget") {
        QList<Expense> monthCategoryExpenses;
        switch (event->type()) {
        case QEvent::Enter:
            qobject_cast<QWidget*>(obj)->setStyleSheet("background-color: #3D3558;"
                                                        "border-radius: 10px;");
            return true;
        case QEvent::Leave:
            qobject_cast<QWidget*>(obj)->setStyleSheet("");
            return true;
        case QEvent::MouseButtonPress:
            for(int i = 0; i < selectedMonths.count(); i++)
            {
                if(selectedMonths[i]->getMonthIndex() == obj->property("month index"))
                {
                    for(int j = 0; j < selectedMonths[i]->getExpenses().count(); j++)
                    {
                        if(convertCategoryToString(selectedMonths[i]->getExpenses()[j].getCategory()) == obj->property("category"))
                            monthCategoryExpenses.append(selectedMonths[i]->getExpenses()[j]);
                    }
                    break;
                }
            }
            detailWindow= new DetailWindow(nullptr, monthCategoryExpenses, settings->value(obj->property("category").toString().toStdString().c_str()).toInt());
            detailWindow->show();
            return true;
        default:
            break;
        }
    }

    if (obj->objectName() == "monthHeaderWidget") {
        QList<Expense> refundExpenses;
        QList<Expense> incomeExpenses;
        QList<Expense> inflowExpenses;
        switch (event->type()) {
        case QEvent::Enter:
            qobject_cast<QWidget*>(obj)->setStyleSheet("#monthHeaderWidget{"
                                                        "background-color: #3D3558;"
                                                        "border-radius: 10px;}");
            return true;
        case QEvent::Leave:
            qobject_cast<QWidget*>(obj)->setStyleSheet("");
            return true;
        case QEvent::MouseButtonPress:
            for(int i = 0; i < selectedMonths.count(); i++)
            {
                if(selectedMonths[i]->getMonthIndex() == obj->property("month index"))
                {
                    for(int j = 0; j < selectedMonths[i]->getExpenses().count(); j++)
                    {
                        if(selectedMonths[i]->getExpenses()[j].getCategory() == Expense::Category::Refund)
                            refundExpenses.append(selectedMonths[i]->getExpenses()[j]);
                        if(selectedMonths[i]->getExpenses()[j].getCategory() == Expense::Category::IncomeAndEarnings)
                            incomeExpenses.append(selectedMonths[i]->getExpenses()[j]);
                        if(selectedMonths[i]->getExpenses()[j].getCategory() == Expense::Category::InflowOfMoney)
                            inflowExpenses.append(selectedMonths[i]->getExpenses()[j]);
                    }
                    break;
                }
            }
            detailWindow = new DetailWindow(nullptr,
                                            refundExpenses,
                                            incomeExpenses,
                                            inflowExpenses,
                                            obj->property("month index").toInt(),
                                            currentYear,
                                            obj->property("remaining").toInt(),
                                            obj->property("food").toInt(),
                                            obj->property("bank").toInt(),
                                            obj->property("education").toInt(),
                                            obj->property("taxes").toInt(),
                                            obj->property("legal").toInt(),
                                            obj->property("housing").toInt(),
                                            obj->property("leisure").toInt(),
                                            obj->property("health").toInt(),
                                            obj->property("shopping").toInt(),
                                            obj->property("excluded").toInt(),
                                            obj->property("transport").toInt(),
                                            obj->property("outflow").toInt(),
                                            obj->property("saving").toInt(),
                                            obj->property("income").toInt(),
                                            obj->property("inflow").toInt(),
                                            obj->property("refund").toInt());
            detailWindow->show();
            return true;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

MainWindow::~MainWindow()
{
    qDeleteAll(files);
    files.clear();
}

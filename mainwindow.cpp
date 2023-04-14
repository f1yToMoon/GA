#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#include "mainwindow.h"
#include "ui_mainwindow.h"

float f(float x1, float x2) {
    float res = 4 * pow(x1 - 5, 2) + pow(x2 - 6, 2);
    return res;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand (static_cast <unsigned> (time(0)));

    ui->setupUi(this);

    ui->spinBox->setValue(0);
    ui->spinBox->setMinimum(-10);

    ui->spinBox_2->setValue(10);
    ui->spinBox_2->setMaximum(20);

    ui->spinBox_3->setValue(5);
    ui->spinBox_3->setMaximum(10);
    ui->spinBox_3->setMinimum(0);
    ui->spinBox_3->setSingleStep(1);

    ui->spinBox_5->setMinimum(200);
    ui->spinBox_5->setMaximum(1000);
    ui->spinBox_5->setValue(200);
    ui->spinBox_5->setSingleStep(200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

TestModel::TestModel(QObject *parent) : QAbstractTableModel(parent)
{
}

// Create a method to populate the model with data:
void TestModel::populateData(const QList<QString> &fun, const QList<QString> &x1, const QList<QString> &x2, const QList<QString> &id)
{
    tm_fun.clear();
    tm_fun = fun;
    tm_x1.clear();
    tm_x1 = x1;
    tm_x2.clear();
    tm_x2 = x2;
    tm_id.clear();
    tm_id = id;
    return;
}

int TestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tm_fun.length();
}

int TestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 1) {
        return tm_fun[index.row()];
    } else if (index.column() == 2) {
        return tm_x1[index.row()];
    } else if (index.column() == 3) {
        return tm_x2[index.row()];
    } else if (index.column() == 0) {
        return tm_id[index.row()];
    }
    return QVariant();
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("n");
        } else if (section == 1) {
            return QString("fun");
        } else if (section == 2) {
            return QString("x1");
        } else if (section == 3) {
            return QString("x2");
        }
    }
    return QVariant();
}

void MainWindow::on_pushButton_clicked()
{
    fun.clear();
    x1.clear();
    x2.clear();
    id.clear();

    ui->textBrowser->clear();

    minFun = 1000;
    bestx1 = 0;
    bestx2 = 0;
    vector<pair<pair<float, float>, float>> init;

    //initialization
    for (int i = 0; i < chromosomes; ++i) {
        init.push_back(make_pair(make_pair(0, 0), 0));
        init[i].first.first = t_left + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(t_right - t_left)));
        init[i].first.second = t_left + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(t_right - t_left)));
    }

    for (int i = 0; i < chromosomes; ++i) {
        init[i].second = f(init[i].first.first, init[i].first.second);
    }

    sort(init.begin(), init.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });

    //_________________________________________________________________________________________________
    for (int k = 0; k < gen; ++k) {

        //crossover
        for (int i = 0; i < chromosomes / 4; ++i) {
            init[i + chromosomes / 2].first.first = (init[i].first.first + init[chromosomes / 2 - 1 - i].first.first) / 2;
            init[i + chromosomes / 2].first.second = (init[i].first.second + init[chromosomes / 2 - 1 - i].first.second) / 2;
            init[chromosomes - 1 - i].first.first = (init[i].first.first + init[chromosomes / 2 - 1 - i].first.first) / 2;
            init[chromosomes - 1 - i].first.second = (init[i].first.second + init[chromosomes / 2 - 1 - i].first.second) / 2;
        }

        //mutation
        for (int i = 0; i < chromosomes; ++i) {
            double val = (double) rand() / RAND_MAX;
            if (val < mutationPercentage) {
                init[i].first.first = init[i].first.first + -0.25 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5)));
            }
        }

        for (int i = 0; i < chromosomes; ++i) {
            init[i].second = f(init[i].first.first, init[i].first.second);
        }

        sort(init.begin(), init.end(), [](auto &left, auto &right) {
            return left.second < right.second;
        });

        if (init[0].second < minFun) {
            minFun = init[0].second;
            bestx1 = init[0].first.first;
            bestx2 = init[0].first.second;
        }

        fun.append(QString::fromStdString(to_string(init[0].second)));
        x1.append(QString::fromStdString(to_string(init[0].first.first)));
        x2.append(QString::fromStdString(to_string(init[0].first.second)));
        id.append(QString::fromStdString(to_string(k + 1)));
    }

    ui->textBrowser->append(QString::fromStdString("x[1] = " + to_string(bestx1)));
    ui->textBrowser->append(QString::fromStdString("x[2] = " + to_string(bestx2)));
    ui->textBrowser->append("\n");
    ui->textBrowser->append(QString::fromStdString("f(x[1] ,x[2]) = " + to_string(minFun)));

    TestModel *PhoneBookModel = new TestModel(this);
    PhoneBookModel->populateData(fun, x1, x2, id);

    // Connect model to table view:
    ui->tableView->setModel(PhoneBookModel);

    // Make table header visible and display table:
    ui->tableView->horizontalHeader()->setVisible(true);
    ui->tableView->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    gen = 1;
    ui->label_5->setText(QString(QString::fromStdString("Количество поколений: " + to_string(gen))));
}

void MainWindow::on_pushButton_2_clicked()
{
    gen = 10;
    ui->label_5->setText(QString(QString::fromStdString("Количество поколений: " + to_string(gen))));
}

void MainWindow::on_pushButton_3_clicked()
{
    gen = 100;
    ui->label_5->setText(QString(QString::fromStdString("Количество поколений: " + to_string(gen))));
}

void MainWindow::on_pushButton_4_clicked()
{
    gen = 1000;
    ui->label_5->setText(QString(QString::fromStdString("Количество поколений: " + to_string(gen))));
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    t_left = arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg2)
{
    t_right = arg2;
}

void MainWindow::on_spinBox_3_valueChanged(int arg3)
{
    mut = arg3;
    mutationPercentage = mut/100;
}

void MainWindow::on_spinBox_5_valueChanged(int arg4)
{
    chromosomes = arg4;
}


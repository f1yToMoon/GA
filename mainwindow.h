#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QAbstractTableModel>
#include <QPushButton>
#include <QSpinBox>
#include <qlabel.h>
#include <QTextBrowser>

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

namespace Ui {
    class MainWindow;
}

class TestModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TestModel(QObject *parent = 0);

    void populateData(const QList<QString> &fun, const QList<QString> &x1, const QList<QString> &x2, const QList<QString> &id);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    float f(float x1, float x2);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<QString> tm_fun, tm_x1, tm_x2, tm_id;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg2);

    void on_spinBox_3_valueChanged(int arg3);

    void on_spinBox_5_valueChanged(int arg4);

private:
    Ui::MainWindow *ui;

    QPushButton pushButton;
    QSpinBox spinBox, spinBox_2, spinBox_3, spinBox_5;
    QLabel label_5;
    QTextBrowser textBrowser;

    float minFun = 1000, bestx1 = 0, bestx2 = 0;
    int chromosomes = 0;
    double mutationPercentage = 0.0, mut = 0.0;
    int gen = 0;
    int t_left = 0, t_right = 0;
    QList<QString> fun, x1, x2, id;
};

#endif // MAINWINDOW_H

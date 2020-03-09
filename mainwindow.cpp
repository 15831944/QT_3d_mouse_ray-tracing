#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <opengl.h>
#include <variable.h>
#include <iostream>
using namespace std;

#define MyQLineEdit() << QLineEdit() << fixed << qSetRealNumberPrecision(3)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timerId = startTimer(20);

    xscale=ui->doubleSpinBox->value();
    yscale=ui->doubleSpinBox->value();
    zscale=ui->doubleSpinBox->value();
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *)
{
    ui->lineEdit->setText(QString::number(ui_mx));
    ui->lineEdit_2->setText(QString::number(ui_my));
    ui->lineEdit_3->setText(QString::number(dist));
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    xscale=arg1;
    yscale=arg1;
    zscale=arg1;
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    xmove=double(arg1);
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    ymove=double(arg1);
}

void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
    zmove=double(arg1);
}

void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    xrotate=arg1;
}

void MainWindow::on_doubleSpinBox_6_valueChanged(double arg1)
{
    yrotate=arg1;
}

void MainWindow::on_doubleSpinBox_7_valueChanged(double arg1)
{
    zrotate=arg1;
}

void MainWindow::on_doubleSpinBox_9_valueChanged(double arg1)
{
    xorigin=arg1;
}

void MainWindow::on_doubleSpinBox_8_valueChanged(double arg1)
{
    yorigin=arg1;
}

void MainWindow::on_doubleSpinBox_10_valueChanged(double arg1)
{
    zorigin=arg1;
}

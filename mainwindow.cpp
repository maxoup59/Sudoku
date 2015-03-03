#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "src/Sudoku.hpp"
#include "src/Sudokusolver.hpp"
#include <QList>
#include <QLabel>
#include <QBoxLayout>
#include <QMainWindow>//For Qt5
#include <QPushButton>//For Qt5
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *window = new QWidget;
    QVBoxLayout *global = new QVBoxLayout;
    QGridLayout *layout = new QGridLayout;
    QHBoxLayout *menu = new QHBoxLayout;

    for (int j = 0 ; j < 9 ; j++)
    {
        for (int i = 0 ;i < 9;i++)
        {
            QLineEdit *lineEdit = new QLineEdit;
            lineEdit->setMaxLength(1);
            lineEdit->setMaximumWidth(20);
            lineEdit->setMaximumHeight(20);
            lineEditList.append(lineEdit);
            lineEdit->setText(QString('0'));
            layout->addWidget(lineEdit,j,i);
        }
    }
    QPushButton *btnOk = new QPushButton("solve");
    connect ( btnOk, SIGNAL( clicked() ), this, SLOT( solve() ) );
    menu->addWidget(btnOk);

    unsigned short num_rows = 3;
    unsigned short num_cols = 3;

    global->addLayout(layout);
    global->addLayout(menu);
    window->setLayout(global);
    window->show();

}
void MainWindow::solve()
{
    //SUPER MEGA FUNCTION SOLVER
    unsigned short num_rows = 3;
    unsigned short num_cols = 3;
    stringstream ss;
    string grid = "xx5x8xxxx78x3xxxxxx04x2xxxx84xx1xxxxx6xxxxxxx1x0x7xxxxxx3x6xxxxxxx5xxxxxxx120xxxx";
    for (int i = 0 ; i < grid.length() ; i++)
    {
        ss.put(grid[i]);
    }
    try {

        Sudoku s(ss.str(), num_rows, num_cols);
        DancingLinksSolver dls;
        dls.solve(s);
        std::string solved = s.getString(s);
        std::string solvedclear ="";
        for (int i = 0; i < solved.length() ; i++)
        {
            if(solved[i] != ' ')
            {
                solvedclear += solved[i];
            }
        }
        for (int i = 0 ;i < 81 ;i++)
        {

            lineEditList[i]->setText( QString(solvedclear[i]));
        }


    } catch(const std::exception& err) {
        cout << err.what() << endl;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

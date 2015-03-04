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
#include <QMessageBox>
#include <QMainWindow>//For Qt5
#include <QPushButton>//For Qt5

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*
     * ui ???
     * comme user interface !
     * c'est bien de faire ta grille à la main
     * mais enfin, tu peux quand même utiliser Qt Designer
     * */
    ui->setupUi(this);
    for (int j = 0 ; j < 9 ; j++)
    {
        for (int i = 0 ;i < 9;i++)
        {
            QLineEdit *lineEdit = new QLineEdit;
            lineEdit->setMaxLength(1);
            lineEdit->setMaximumWidth(20);
            QRegExp rx("(|\"|x|\\.|[0-9]){30}");
            lineEdit->setValidator(new QRegExpValidator(rx, this));
            lineEdit->setMaximumHeight(20);
            lineEditList.append(lineEdit);
            lineEdit->setText(QString('0'));
            ui->gridLayoutSudoku->addWidget(lineEdit,j,i);
        }
    }
    string grid = "4xxx3xxx2"
            "x2xxx135x"
            "x7x02xxxx"
            "x4xxxx6xx"
            "1x2xxx0x5"
            "xx8xxxx7x"
            "xxxx54x6x"
            "x648xxx1x"
            "3xxx7xxx0"
;
    applyGrid(grid);
}
void MainWindow::applyGrid(std::string grid)
{
    for (int i = 0 ;i < 81 ;i++)
    {
        lineEditList[i]->setText( QString(grid[i]));
    }
}

std::string MainWindow::readGrid()
{
    string read = "";
    for (int i = 0 ;i < 81 ;i++)
    {
        read +=  lineEditList[i]->text().toUtf8().constData();
    }
    return read;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSolve_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Qt Designer, c'est le bien !");
    msgBox.exec();
    //SUPER MEGA FUNCTION SOLVER
    unsigned short num_rows = 3;
    unsigned short num_cols = 3;
    try
    {
        Sudoku s(readGrid(), num_rows, num_cols);
        DancingLinksSolver dls;
        dls.solve(s);
        std::string solved = s.getString();
        std::string solvedclear ="";
        for (int i = 0; i < solved.length() ; i++)
        {
            if(solved[i] != ' ')
            {
                solvedclear += solved[i];
            }
        }
        applyGrid(solvedclear);
    }
    catch(const std::exception& err)
    {
        cout << err.what() << endl;
    }
}

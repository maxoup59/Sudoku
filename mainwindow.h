#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void applyGrid(std::string grid);
     std::string readGrid();
/*public slots:
        void oldSolve();*/
private slots:
        void on_pushButtonSolve_clicked();
private:
    Ui::MainWindow *ui;
    QList<QLineEdit *> lineEditList;
};

#endif // MAINWINDOW_H

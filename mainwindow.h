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
public slots:
        void solve();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void applyGrid(std::string grid);
     std::string readGrid();

private:
    Ui::MainWindow *ui;
    QList<QLineEdit *> lineEditList;
};

#endif // MAINWINDOW_H

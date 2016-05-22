#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "analyesdialog.h"
#include "lr0.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    string filename;

private slots:
    void on_browerBtn_clicked();

    void on_toAnalyesBtn_clicked();

    void on_startBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef ANALYESDIALOG_H
#define ANALYESDIALOG_H

#include <QDialog>
#include<list>
#include<sstream>
#include"lr0.h"

namespace Ui {
class AnalyesDialog;
}

class AnalyesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyesDialog(QWidget *parent = 0);
    void make(string filename);
    void initTable();
    void init();
    string checkActionTable(string state,string _vt);
    string checkGotoTable(string state,string _vn);
    void analyes();
    void printProcess();
    ~AnalyesDialog();
private slots:
    void on_back_clicked();

    void on_analyes_clicked();

private:
    Ui::AnalyesDialog *ui;
    list<string> stateStack;
    list<string> symbolStack;
    string input;
    int input_point;
    int flag;
    LR0 lr0;
};

#endif // ANALYESDIALOG_H

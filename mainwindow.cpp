#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "qdebug.h"
#include <QGroupBox>
#include <QDesktopWidget>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_browerBtn_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);   //AcceptOpen打开,AcceptSave保存文件
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setOption(QFileDialog::ReadOnly, true);
    fileDialog.setWindowTitle(QString("select grammar file"));
    fileDialog.setDirectory(QString("./"));
    fileDialog.setNameFilter(QString("all(*.*)"));
    if (fileDialog.exec())
    {
        QString path = fileDialog.selectedFiles()[0];
        qDebug()<<"path"<<path<<endl;
        ui->fileName->setText(path);
        filename=path.toStdString();
    }
}

void MainWindow::on_toAnalyesBtn_clicked()
{
    this->hide();
    AnalyesDialog analyesDialog;
    analyesDialog.make(filename);
    analyesDialog.show();
    analyesDialog.move ((QApplication::desktop()->width() - analyesDialog.width())/2,
                        (QApplication::desktop()->height() - analyesDialog.height())/2);
    analyesDialog.exec();
    this->show();
}

void MainWindow::on_startBtn_clicked()
{
    ui->lr0Table->setColumnCount(0);
    ui->lr0Table->setRowCount(0);
    QString qfilename=ui->fileName->text();
    filename=qfilename.toStdString();
    LR0 lr0;
    lr0.exec(filename);

    int columnLen=1+lr0.vt_length+lr0.vn_length;
    ui->lr0Table->verticalHeader()->setVisible(false);
    ui->lr0Table->horizontalHeader()->setVisible(false);
    ui->lr0Table->setColumnCount(columnLen);
    ui->lr0Table->setRowCount(1+lr0.item_set_length);
    ui->lr0Table->setItem(0,0,new QTableWidgetItem("state"));
    for(int i=0;i<lr0.vt_length;i++){
        ui->lr0Table->setItem(0,i+1,new QTableWidgetItem(QString::fromStdString(lr0.vt[i])));
    }
    int temp;
    for(int i=0;i<lr0.vn_length;i++){
        temp=i+lr0.vt_length+1;
        ui->lr0Table->setItem(0,temp,new QTableWidgetItem(QString::fromStdString(lr0.vn[i])));
    }
    for(int i=0;i<lr0.item_set_length;i++){
        ui->lr0Table->setItem(i+1,0,new QTableWidgetItem(QString::number(i)));
    }
    for(int i=0;i<lr0.item_set_length;i++){
        for(int j=0;j<lr0.vt_length;j++){
            ui->lr0Table->setItem(i+1,j+1,new QTableWidgetItem(QString::fromStdString(lr0.actionTable[i][j])));
        }
    }
    for(int i=0;i<lr0.item_set_length;i++){
        for(int j=0;j<lr0.vn_length;j++){
            temp=j+lr0.vt_length+1;
            ui->lr0Table->setItem(i+1,temp,new QTableWidgetItem(QString::fromStdString(lr0.gotoTable[i][j])));
        }
    }
}

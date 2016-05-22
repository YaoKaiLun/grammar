#include "analyesdialog.h"
#include "ui_analyesdialog.h"
#include "qdebug.h"

AnalyesDialog::AnalyesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyesDialog)
{
    ui->setupUi(this);
    ui->processTable->verticalHeader()->setVisible(false);
    ui->processTable->horizontalHeader()->setVisible(false);
    initTable();
}

AnalyesDialog::~AnalyesDialog()
{
    delete ui;
}

void AnalyesDialog::make(string filename){
    qDebug()<<"filename:"<<QString::fromStdString(filename)<<endl;
    lr0.exec(filename);
}

void AnalyesDialog::initTable(){
    ui->processTable->setColumnCount(3);
    ui->processTable->setRowCount(1);
    ui->processTable->setItem(0,0,new QTableWidgetItem("state"));
    ui->processTable->setItem(0,1,new QTableWidgetItem("symbol"));
    ui->processTable->setItem(0,2,new QTableWidgetItem("input"));
}

void AnalyesDialog::init(){
    input_point=0;
    flag=0;
    stateStack.clear();
    symbolStack.clear();
    stateStack.push_back("0");
    symbolStack.push_back("#");
}

void AnalyesDialog::on_back_clicked()
{
    this->close();
}

void AnalyesDialog::on_analyes_clicked()
{
    initTable();
    init();
     QString qinput=ui->expression->text();
     qDebug()<<"input:"<<qinput<<endl;
     if(qinput!=""){
         input=qinput.toStdString()+"#";
         analyes();
         if(flag==1){
             int row=ui->processTable->rowCount();
             ui->processTable->insertRow(row);
             ui->processTable->setItem(row,1,new QTableWidgetItem("success!"));
         }
         else{
             int row=ui->processTable->rowCount();
             ui->processTable->insertRow(row);
             ui->processTable->setItem(row,1,new QTableWidgetItem("fail!"));
         }
     }
}

string AnalyesDialog::checkActionTable(string state,string _vt){
    int st;
    stringstream ss;
    ss<<state;
    ss>>st;
    for(int i=0;i<lr0.vt_length;i++){
        if(_vt==lr0.vt[i]){
            //cout<<"actionTable["<<st<<"]["<<i<<"]"<<endl;
            return lr0.actionTable[st][i];
        }
    }
    return "";
}

string AnalyesDialog::checkGotoTable(string state,string _vn){
    int st;
    stringstream ss;
    ss<<state;
    ss>>st;
    for(int i=0;i<lr0.vn_length;i++){
        if(_vn==lr0.vn[i]){
            return lr0.gotoTable[st][i];
        }
    }
    return "";
}

void AnalyesDialog::analyes(){
    printProcess();
    string stateStack_top=stateStack.back();
    string now_input=input.substr(input_point,1);
    string goal=checkActionTable(stateStack_top,now_input);
    if(goal=="acc"){
        flag=1;
    }
    else if(goal[0]=='s'){
        stateStack.push_back(goal.substr(1));
        symbolStack.push_back(now_input);
        input_point++;
        analyes();
    }
    else if(goal[0]=='r'){
        int state;
        stringstream ss;
        ss<<goal[1];
        ss>>state;
        int len=lr0.x_grammars[state].length;
        for(int i=0;i<len;i++){
            symbolStack.pop_back();
            stateStack.pop_back();
        }
        symbolStack.push_back(lr0.x_grammars[state].left);
        stateStack_top=stateStack.back();
        string st=checkGotoTable(stateStack_top,lr0.x_grammars[state].left);
        stateStack.push_back(st);
        analyes();
    }
}

void AnalyesDialog::printProcess(){
   int row=ui->processTable->rowCount();
   list<string>::const_iterator pos;
   string str="";
   for(pos=stateStack.begin();pos!=stateStack.end();++pos){
      str+=*pos;
      str+=" ";
   }
   ui->processTable->insertRow(row);
   ui->processTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString(str)));
   str="";
   for(pos=symbolStack.begin();pos!=symbolStack.end();++pos){
      str+=*pos;
      str+=" ";
   }
   ui->processTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString(str)));
   str="";
   for(int i=input_point;i<input.length();i++){
       str+=input[i];
   }
   ui->processTable->setItem(row,2,new QTableWidgetItem(QString::fromStdString(str)));
}

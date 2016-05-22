#ifndef LR0_H
#define LR0_H
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
class LR0{
private:
    friend class MainWindow;
    friend class AnalyesDialog;
    int grammar_length,item_set_length,dfa_length,vn_length,vt_length;
    static const int GRAMMAR_COUNT=100;
    static const int VN_COUNT=100;
    static const int VT_COUNT=100;
    static const int ITEM_SET_COUNT=100;
    static const int ITEM_COUNT=100;
    static const int DFA_COUNT=500;
    static const int ACC_FLAG=1000;
    static const int REDUCE_FLAG=2000;
    struct Grammar{
        int grammar_num;
        string left;
    string right;
        int length;
    };
    struct Item{
        int item_group_num;
        int grammar_num;
        int point_position;
    };
    struct ItemGroup{
        int item_group_num;
    Item items[ITEM_COUNT];
    int length=0;
    };
    struct Conversion{
        int init;
        string trigger;
        int end;
    };
    Grammar x_grammars[GRAMMAR_COUNT];
    string vn[VN_COUNT];
    string vt[VT_COUNT];
    ItemGroup item_set[ITEM_SET_COUNT];
    Conversion DFA[DFA_COUNT];
    string actionTable[ITEM_SET_COUNT][VT_COUNT];
    string gotoTable[ITEM_SET_COUNT][VN_COUNT];
public:
    LR0();
    bool exec(string filename);
    bool readGrammar(string filename);
    void CLOSUSER(Item _item);
    bool inItemGroup(int _group_num,int _grammar_num,int position);
    void GO(Item _item,string left);
    int itemGroupExist(Item _item);
    void printItemSet();
    void printDFA();
    int inVn(string _vn);
    int inVt(string _vt);
    void makeTable();
    void makeActionTable(Conversion conversion);
    void makeGotoTable(Conversion conversion);
    void printTable();
};
#endif // LR0_H

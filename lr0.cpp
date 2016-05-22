#include "lr0.h"

#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

LR0::LR0(){
    grammar_length=0;
    item_set_length=0;
    dfa_length=0;
    vn_length=0;
    vt_length=0;
    for(int i=0;i<ITEM_SET_COUNT;i++){
        for(int j=0;j<VT_COUNT;j++){
            actionTable[i][j]="";
        }
    }
    for(int i=0;i<ITEM_SET_COUNT;i++){
        for(int j=0;j<VN_COUNT;j++){
            gotoTable[i][j]="";
        }
    }

}

bool LR0::exec(string filename){
    if(readGrammar(filename)){
    ItemGroup item_group;
        item_group.item_group_num=item_set_length;
        item_group.items[0].grammar_num=0;
        item_group.items[0].point_position=0;
        item_group.items[0].item_group_num=0;
    item_group.length++;
    item_set[0]=item_group;
    item_set_length++;
    CLOSUSER(item_set[0].items[0]);
    int i=0;
        string right;
        string next;
    while(1){
        if(i<item_set_length){
            for(int j=0;j<item_set[i].length;j++){
                    right=x_grammars[item_set[i].items[j].grammar_num].right;
                next =right[item_set[i].items[j].point_position];
                GO(item_set[i].items[j],next);
            }
        i++;
        }
        else break;
    }
    printItemSet();
        //printDFA();
        makeTable();
        printTable();
        return true;
    }
    else{
        return false;
    }
}

bool LR0::readGrammar(string filename){
    ifstream ifs(filename);
    string line;
    int fpos1=0;
    int fpos2=0;
    int pos;
    if(ifs.is_open()){
        //get vn and vt
        if(getline(ifs,line)){
            while((fpos2=line.find(" ",fpos1))!=string::npos){
               vn[vn_length]=line.substr(fpos1,fpos2-fpos1);
               fpos1=fpos2+1;
               vn_length++;
            }
            vn[vn_length]=line.substr(fpos1);
            vn_length++;
        }
        for(int i=0;i<vn_length;i++){
            cout<<"vn:"<<vn[i]<<"  ";
        }
        fpos1=0;
        fpos2=0;
        if(getline(ifs,line)){
            while((fpos2=line.find(" ",fpos1))!=string::npos){
               vt[vt_length]=line.substr(fpos1,fpos2-fpos1);
               fpos1=fpos2+1;
               vt_length++;
            }
            vt[vt_length]=line.substr(fpos1);
            vt_length++;
        }
        vt[vt_length]="#";
        vt_length++;
        cout<<endl;
        for(int i=0;i<vt_length;i++){
            cout<<"vt:"<<vt[i]<<"  ";
        }
        cout<<endl;
        while(getline(ifs,line)){
            pos=line.find(":");
            x_grammars[grammar_length].left=line.substr(0,pos);
            x_grammars[grammar_length].right=line.substr(pos+1);
            x_grammars[grammar_length].length=x_grammars[grammar_length].right.length();
            cout<<"("<<grammar_length<<") "<<x_grammars[grammar_length].left<<"->"<<x_grammars[grammar_length].right<<endl;
            grammar_length++;
        }
        return true;
    }else{
        return false;
    }

}

void LR0::CLOSUSER(Item _item){
        string right;
        string next;
        int item_num;
        if(_item.point_position<x_grammars[_item.grammar_num].length){
            right=x_grammars[_item.grammar_num].right;
        next=right[_item.point_position];
            if(inVn(next)!=-1){
            for(int i=0;i<grammar_length;i++){
                if(x_grammars[i].left==next){
                Item next_item;
                next_item.item_group_num=_item.item_group_num;
                next_item.grammar_num=i;
                    next_item.point_position=0;
                        if(!inItemGroup(_item.item_group_num,i,0)){
                            item_num=item_set[_item.item_group_num].length;
                        item_set[_item.item_group_num].items[item_num]=next_item;
                    item_set[_item.item_group_num].length++;
                    CLOSUSER(next_item);
                        }
                    }
            }
            }
    }
}

bool LR0::inItemGroup(int _group_num,int _grammar_num,int position){
    if(item_set[_group_num].length==0){
        return false;
    }
    else{
        for(int i=0;i<item_set[_group_num].length;i++){
            if(item_set[_group_num].items[i].grammar_num==_grammar_num&&item_set[_group_num].items[i].point_position==position){
                return true;
            }
        }
        return false;
    }
}

void LR0::GO(Item _item,string next){
    Item new_item;
    int _group_num;
    new_item.grammar_num=_item.grammar_num;
    new_item.point_position=_item.point_position+1;
    //cout<<"GO("<<x_grammars[_item.grammar_num].left<<","<<next<<")"<<"len:"<<x_grammars[_item.grammar_num].length<<" pos:"<<_item.point_position<<endl;
    if(_item.point_position<x_grammars[_item.grammar_num].length&&(_group_num=itemGroupExist(new_item))==-1){
        int flag=1;
        //cout<<"GO("<<x_grammars[_item.grammar_num].left<<","<<next<<")"<<endl;
        for(int i=0;i<dfa_length;i++){
            if(DFA[i].init==_item.item_group_num&&DFA[i].trigger==next){
                 flag=0;
                 new_item.item_group_num=DFA[i].end;
                 item_set[DFA[i].end].items[item_set[DFA[i].end].length]=new_item;
                 item_set[DFA[i].end].length++;
                 CLOSUSER(new_item);
            }
        }
        if(flag==1){
            ItemGroup item_group;
            item_group.item_group_num=item_set_length;
            new_item.item_group_num=item_set_length;
            item_group.items[0]=new_item;
            item_group.length++;
            item_set[item_set_length]=item_group;
            item_set_length++;

            DFA[dfa_length].init=_item.item_group_num;
            DFA[dfa_length].trigger=next;
            DFA[dfa_length].end=new_item.item_group_num;
            dfa_length++;
            CLOSUSER(new_item);
        }
    }
    if(_item.point_position==x_grammars[_item.grammar_num].length){
        DFA[dfa_length].init=_item.item_group_num;
        stringstream ss;
        string t;
        ss<<t<<_item.grammar_num;
        DFA[dfa_length].trigger=ss.str();
        if(_item.grammar_num==0){
            DFA[dfa_length].end=ACC_FLAG;
        }
        else{
            DFA[dfa_length].end=REDUCE_FLAG;
        }
        dfa_length++;
    }
    else if(_group_num!=-1){
       DFA[dfa_length].init=_item.item_group_num;
       DFA[dfa_length].trigger=next;
       DFA[dfa_length].end=_group_num;
       dfa_length++;
    }
}

int LR0::itemGroupExist(Item _item){
    int num;
    for(int i=0;i<item_set_length;i++){
        num=item_set[i].items[0].grammar_num;
        if(_item.grammar_num==num&&_item.point_position==item_set[i].items[0].point_position){
            return i;
    }
    }
    return -1;
}

void LR0::printItemSet(){
    cout<<"item set:"<<endl;
    int num;
    for(int i=0;i<item_set_length;i++){
        cout<<"I"<<i<<endl;
    for(int j=0;j<item_set[i].length;j++){
            num=item_set[i].items[j].grammar_num;
            cout<<x_grammars[num].left<<"->"<<x_grammars[num].right<<"("<<item_set[i].items[j].point_position<<")"<<endl;
        }
    }
}

void LR0::printDFA(){
    cout<<"DFA:"<<endl;
    for(int i=0;i<dfa_length;i++){
       cout<<"("<<DFA[i].init<<","<<DFA[i].trigger<<","<<DFA[i].end<<")"<<endl;
    }
}

int LR0::inVn(string _vn){
    for(int i=0;i<vn_length;i++){
        if(_vn==vn[i]) return i;
    }
    return -1;
}

int LR0::inVt(string _vt){
    for(int i=0;i<vt_length;i++){
        if(_vt==vt[i]){
            return i;
        }
    }
    return -1;
}

void LR0::makeTable(){
    for(int i=0;i<dfa_length;i++){
        if(inVn(DFA[i].trigger)!=-1){
            //cout<<"vn:"<<DFA[i].trigger<<endl;
            makeGotoTable(DFA[i]);
        }
        else{
            makeActionTable(DFA[i]);
        }
    }
}

void LR0::makeActionTable(Conversion conversion){
    int temp=inVt(conversion.trigger);
    if(conversion.end==ACC_FLAG){
        temp=inVt("#");
        actionTable[conversion.init][temp]="acc";
    }
    else if(conversion.end==REDUCE_FLAG){
        for(int i=0;i<vt_length;i++){
            if(actionTable[conversion.init][i]==""){
                actionTable[conversion.init][i]="r"+conversion.trigger;
            }
        }
    }
    else{
        stringstream ss;
        ss<<"s"<<conversion.end;
        actionTable[conversion.init][temp]=ss.str();
        //cout<<actionTable[conversion.init][temp]<<endl;
    }
}

void LR0::makeGotoTable(Conversion conversion){
    int temp=inVn(conversion.trigger);
    stringstream ss;
    ss<<conversion.end;
    gotoTable[conversion.init][temp]=ss.str();
    //cout<<gotoTable[conversion.init][temp]<<endl;
}

void LR0::printTable(){
    cout<<"table:"<<endl;
    cout<<"state"<<'\t';
    for(int i=0;i<vt_length;i++){
        cout<<vt[i]<<'\t';
    }
    for(int i=0;i<vn_length;i++){
        cout<<vn[i]<<'\t';
    }
    cout<<endl;
    for(int i=0;i<vn_length;i++){

    }
    for(int i=0;i<item_set_length;i++){
        cout<<i<<'\t';
        for(int j=0;j<vt_length;j++){
            cout<<actionTable[i][j]<<'\t';
        }
        for(int r=0;r<vn_length;r++){
            cout<<gotoTable[i][r]<<'\t';
        }
        cout<<endl;
    }
}

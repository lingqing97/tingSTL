#include"stl_wj_set.h"
#include<iostream>
#include<algorithm>

using namespace std;

int main(){
    wj::set<int> iset;
    iset.insert(0);
    iset.insert(1);
    iset.insert(2);
    iset.insert(3);
    iset.insert(4);

    cout<<"size="<<iset.size()<<endl;

    wj::set<int>::iterator ite1=iset.begin();
    wj::set<int>::iterator ite2=iset.end();
    for(;ite1!=ite2;++ite1){
        cout<<*ite1<<' ';
    }
    cout<<endl;

    //使用STL算法find()来搜寻元素，可以有效运作，但不是好办法
    //因为STL算法find()只是循序搜寻
    ite1=std::find(iset.begin(),iset.end(),3);
    if(ite1!=iset.end())
        cout<<"3 found"<<endl;

    ite1=iset.find(1);

    if(ite1!=iset.end())
        cout<<"1 found"<<endl;

    // *ite1=9;     //error:set元素不被允许修改

    ite1=iset.find(7);
    if(ite1==iset.end())
        cout<<"7 not found"<<endl;


    return 0;
}
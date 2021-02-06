#include"stl_wj_slist.h"
#include<iostream>
#include<algorithm>

using namespace std;

int main(){
    int i;
    wj::slist<int> islist;
    cout<<"size="<<islist.size()<<endl;

    islist.push_front(9);
    islist.push_front(1);
    islist.push_front(2);
    islist.push_front(3);
    islist.push_front(4);
    cout<<"size="<<islist.size()<<endl;

    wj::slist<int>::iterator ite=islist.begin();
    wj::slist<int>::iterator ite2=islist.end();
    for(;ite!=ite2;++ite)
        cout<<*ite<<' ';
    cout<<endl;

    ite=islist.begin();
    ite2=islist.end();
    for(;ite!=ite2;++ite)
        cout<<*ite<<' ';
    cout<<endl;

    ite=std::find(islist.begin(),islist.end(),3);
    if(ite!=0)
        cout<<*(islist.erase(ite))<<endl;

    ite=islist.begin();
    ite2=islist.end();
    for(;ite!=ite2;++ite)
        cout<<*ite<<' ';
    cout<<endl;

    return 0;
}
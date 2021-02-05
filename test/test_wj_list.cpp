#include"stl_wj_list.h"
#include<iostream>
#include<algorithm>

int main(){
    int i;
    wj::list<int> ilist;
    std::cout<<"size="<<ilist.size()<<std::endl;

    ilist.push_back(0);
    ilist.push_back(1);
    ilist.push_back(2);
    ilist.push_back(3);
    ilist.push_back(4);
    std::cout<<"size="<<ilist.size()<<std::endl;

    wj::list<int>::iterator ite;
    for(ite=ilist.begin();ite!=ilist.end();++ite){
        std::cout<<*ite<<' ';
    }
    std::cout<<std::endl;

    ite=std::find(ilist.begin(),ilist.end(),3);
    if(ite!=0)
        ilist.insert(ite,99);

    std::cout<<"size="<<ilist.size()<<std::endl;
    std::cout<<*ite<<std::endl;

    for(ite=ilist.begin();ite!=ilist.end();++ite){
        std::cout<<*ite<<' ';
    }
    std::cout<<std::endl;

    ite=std::find(ilist.begin(),ilist.end(),1);
    if(ite!=0)
        std::cout<<*(ilist.erase(ite))<<std::endl;

    ilist.pop_back();
    ilist.push_back(4);

    ilist.pop_front();
    ilist.push_front(0);

    ilist.push_back(1);
    ilist.push_back(1);
    ilist.push_back(1);
    ilist.push_back(1);

    ilist.unique();

    for(ite=ilist.begin();ite!=ilist.end();++ite){
        std::cout<<*ite<<' ';
    }
    std::cout<<std::endl;

    int iv[5]={5,6,7,8,9};
    wj::list<int> ilist2(iv,iv+5);

    for(ite=ilist2.begin();ite!=ilist2.end();++ite){
        std::cout<<*ite<<' ';
    }
    std::cout<<std::endl;

    ite=std::find(ilist.begin(),ilist.end(),99);
    ilist.splice(ite,ilist2);

    for(ite=ilist.begin();ite!=ilist.end();++ite){
        std::cout<<*ite<<' ';
    }
    std::cout<<std::endl;

    int iv2[5]={11,12,13,14,15};
    wj::list<int> ilist3(iv2,iv2+5);

    int iv3[5]={6,7,10,19,20};
    wj::list<int> ilist4(iv3,iv3+5);

    ilist3.merge(ilist4);

    for(ite=ilist3.begin();ite!=ilist3.end();++ite){
        std::cout<<*ite<<' ';
    }
    std::cout<<std::endl;

    ilist.reverse();

    for(ite=ilist.begin();ite!=ilist.end();++ite){
        std::cout<<*ite<<' ';
    }
    std::cout<<std::endl;

    return 0;
}
#include"stl_wj_vector.h"
#include<iostream>


int main(){
    int i;
    wj::vector<int> iv(2,9);
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    iv.push_back(1);
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    iv.push_back(2);
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    iv.push_back(3);
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    iv.push_back(4);
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    for(i=0;i<iv.size();++i)
        std::cout<<iv[i]<<' ';
    std::cout<<std::endl;

    iv.push_back(5);

    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;
    for(i=0;i<iv.size();++i)
        std::cout<<iv[i]<<' ';
    std::cout<<std::endl;

    iv.pop_back();
    iv.pop_back();
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    iv.pop_back();
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    wj::vector<int>::iterator ivite=std::find(iv.begin(), iv.end(),1);
    if(ivite) iv.erase(ivite);
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;
    for(i=0;i<iv.size();++i)
        std::cout<<iv[i]<<' ';
    std::cout<<std::endl;

    wj::vector<int>::iterator ite=std::find(iv.begin(),iv.end(),2);
    if(ite) iv.insert(ite,3,7);

    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;
    for(i=0;i<iv.size();++i)
        std::cout<<iv[i]<<' ';
    std::cout<<std::endl;

    iv.clear();
    std::cout<<"size="<<iv.size()<<std::endl;
    std::cout<<"capacity="<<iv.capacity()<<std::endl;

    return 0;
}
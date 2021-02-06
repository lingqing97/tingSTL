#include"stl_wj_array.h"
#include<iostream>

int main(){
    wj::array<int,10> myarray;
    for(int i=0;i<10;++i)
        myarray[i]=i;
    for(auto ite=myarray.begin();ite!=myarray.end();++ite)
        std::cout<<*ite<<' ';
    std::cout<<std::endl;
    for(auto ite=myarray.cbegin();ite!=myarray.cend();++ite)
        std::cout<<*ite<<' ';
    std::cout<<std::endl;
    std::cout<<myarray.size()<<" "<<myarray.max_size()<<std::endl;
    std::cout<<myarray.front()<<" "<<myarray.back()<<std::endl;
    //std::cout<<myarray.at(12)<<std::endl;
    return 0;
}
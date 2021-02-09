#include"stl_wj_algorithm.h"
#include<vector>
#include<iostream>

using namespace std;

int main(){
    int ia[9]={0,1,2,3,4,8,9,3,5};
    vector<int> ivec(ia,ia+9);

    wj::make_heap(ivec.begin(),ivec.end());
    for(int i=0;i<ivec.size();++i)
        cout<<ivec[i]<<' ';
    cout<<endl;

    ivec.push_back(7);
    wj::push_heap(ivec.begin(),ivec.end());
    for(int i=0;i<ivec.size();++i)
        cout<<ivec[i]<<' ';
    cout<<endl;

    wj::pop_heap(ivec.begin(),ivec.end());
    cout<<ivec.back()<<endl;
    ivec.pop_back();

    for(int i=0;i<ivec.size();++i)
        cout<<ivec[i]<<' ';
    cout<<endl;

    wj::sort_heap(ivec.begin(),ivec.end());
    for(int i=0;i<ivec.size();++i)
        cout<<ivec[i]<<' ';
    cout<<endl;

    return 0;
}
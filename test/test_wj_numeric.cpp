#include"stl_wj_numeric.h"
#include"stl_wj_function.h"
#include<iostream>
#include<vector>
using namespace std;

int main(){
    int ia[5]={1,2,3,4,5};
    vector<int> iv(ia,ia+5);
    cout<<wj::accumulate(iv.begin(),iv.end(),0)<<endl;

    //cout<<wj::accumulate(iv.begin(),iv.end(),wj::minus<int>())<<endl;
    cout<<wj::accumulate(iv.begin(),iv.end(),0,wj::minus<int>())<<endl;

    cout<<wj::power(10,3)<<endl;
    cout<<wj::power(10,3,wj::plus<int>())<<endl;

    return 0;
}
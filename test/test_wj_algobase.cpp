#include"stl_wj_algobase.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;


int main(){
    int ia[9]={0,1,2,3,4,5,6,7,8};
    vector<int> ivc(ia,ia+9);

    wj::swap(*ivc.begin(),*ivc.rbegin());
    for(auto num:ivc) cout<<num<<' ';
    cout<<endl;

    string stra1[]={"Jamie","JJHou","Jason"};
    string stra2[]={"Jamie","JJhou","Jason"};

    cout<<wj::lexicographic_compare(stra1,stra1+2,stra2,stra2+2);

    return 0;
}
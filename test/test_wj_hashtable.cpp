#include"stl_wj_hashtable.h"
#include"stl_wj_function.h"
#include"stl_wj_hash_fun.h"
#include<iostream>

using namespace std;

int main(){
    wj::hashtable<int,int,wj::hash<int>,wj::identity<int>,wj::equal_to<int>> iht(50,wj::hash<int>(),wj::equal_to<int>());
    cout<<iht.size()<<endl;
    cout<<iht.bucket_count()<<endl;

    iht.insert_unique(59);
    iht.insert_unique(63);
    iht.insert_unique(108);

    for(auto ite=iht.begin();ite!=iht.end();++ite)
        cout<<*ite<<' ';
    cout<<endl;
    cout<<iht.size()<<endl;
    cout<<iht.bucket_count()<<endl;

    iht.insert_equal(59);
    iht.insert_equal(100);
    iht.insert_equal(18);

    for(auto ite=iht.begin();ite!=iht.end();++ite)
        cout<<*ite<<' ';
    cout<<endl;
    cout<<iht.size()<<endl;
    cout<<iht.bucket_count()<<endl;

    auto ite=iht.find(100);
    if(ite!=iht.end())
        cout<<"found 100"<<endl;
    else
        cout<<"not found 100"<<endl;

    cout<<iht.count(59)<<endl;

    ite=iht.begin();
    for(int i=0;i<iht.size();++i,++ite)
        cout<<*ite<<' ';
    cout<<endl;

    return 0;
}
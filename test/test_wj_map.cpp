#include"stl_wj_map.h"
#include<iostream>
#include<algorithm>
#include<string>

using namespace std;

int main(){
    wj::map<std::string,int> simap;
    simap[string("key1")]=1;
    simap[string("key2")]=2;
    simap[string("key3")]=3;
    simap[string("key4")]=4;

    for(auto ite=simap.begin(); ite!=simap.end();++ite){
        cout<<ite->first<<' '<<ite->second<<endl;
    }
    cout<<endl;

    simap.insert({string("key5"),5});
    simap.insert({string("key6"),6});
    for(auto ite=simap.begin(); ite!=simap.end();++ite){
        cout<<ite->first<<' '<<ite->second<<endl;
    }
    cout<<endl;

    cout<<simap["key5"]<<endl;

    auto ite=simap.find("key8");
    if(ite==simap.end())
        cout<<"key8 not found"<<endl;

    ite=simap.find("key3");
    ite->second=30;
    cout<<simap["key3"]<<' ';

    return 0;
}
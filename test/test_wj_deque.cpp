#include"stl_wj_deque.h"
#include<iostream>

using namespace std;

int main(){
    wj::deque<int,8> ideq(20,9);

    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.push_back(10);
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.push_front(10);
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.pop_back();
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.pop_front();
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.push_back(10);
    ideq.push_back(11);
    ideq.push_back(12);
    ideq.push_back(13);
    ideq.push_back(14);
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;


    ideq.pop_back();
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.pop_back();
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.pop_back();
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    ideq.insert(ideq.end(),20);
    ideq.insert(ideq.begin(),20);
    ideq.insert(ideq.begin()+6,20);
    ideq.insert(ideq.end()-5,20);
    for(auto ite=ideq.begin();ite!=ideq.end();++ite){
        cout<<*ite<<" ";
    }
    cout<<endl;
    cout<<"size="<<ideq.size()<<endl;

    return 0;
}
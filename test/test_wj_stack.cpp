#include"stl_wj_stack.h"
#include<iostream>
using namespace std;


int main(){
    wj::stack<int> istack;
    istack.push(1);
    istack.push(3);
    istack.push(5);
    istack.push(7);

    cout<<istack.size()<<endl;
    cout<<istack.top()<<endl;

    istack.pop();
    cout<<istack.top()<<endl;

    istack.pop();
    cout<<istack.top()<<endl;

    istack.pop();
    cout<<istack.top()<<endl;

    cout<<istack.size()<<endl;

    return 0;
}
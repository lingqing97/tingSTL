#include"stl_wjalloc.h"
#include<vector>
#include<iostream>
using std::vector;

int main(){
    int elements[]={1,2,4,5,6,3,2,4};
    const int n=sizeof(elements)/sizeof(int);
    vector<int,wj::allocator<int>> vec(elements,elements+n);
    for_each(vec.begin(),vec.end(),[](int a){ std::cout<<a<<" "; });
    return 0;
}
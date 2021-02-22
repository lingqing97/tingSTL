#include"stl_wj_algo.h"
#include<iostream>
#include<algorithm>
#include<iterator>  //for ostream_iterator
#include<set>
using namespace std;

template<class T>
struct display{
    void operator()(const T& x)
    { cout<<x<<' '; }
};

int main(){

    //测试set相关算法
    int ia1[6]={1,3,5,7,9,11};
    int ia2[7]={1,1,2,3,5,8,13};
    std::multiset<int> s1(ia1,ia1+6);
    std::multiset<int> s2(ia2,ia2+7);

    std::for_each(s1.begin(),s1.end(),display<int>());
    cout<<endl;

    std::for_each(s2.begin(),s2.end(),display<int>());
    cout<<endl;

    auto first1=s1.begin();
    auto first2=s2.begin();
    auto last1=s1.end();
    auto last2=s2.end();

    cout<<"Union s1 and s2:";
    wj::set_union(first1,last1,first2,last2,std::ostream_iterator<int>(cout," "));  //Union s1 and s2:1 1 2 3 5 7 8 9 11 13
    cout<<endl;

    cout<<"Intersection s1 and s2:";
    wj::set_intersection(first1,last1,first2,last2,std::ostream_iterator<int>(cout," "));  //Intersection s1 and s2:1 3 5
    cout<<endl;

    cout<<"Difference of s1 and s2:";
    wj::set_difference(first1,last1,first2,last2,std::ostream_iterator<int>(cout," "));     //Difference of s1 and s2:7 9 11
    cout<<endl;

    cout<<"Symmetric difference of s1 and s2:";
    wj::set_symmetric_difference(first1,last1,first2,last2,std::ostream_iterator<int>(cout," "));   //Symmetric difference of s1 and s2:1 2 7 8 9 11 13
    cout<<endl;

    return 0;

}
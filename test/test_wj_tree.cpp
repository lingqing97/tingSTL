#include"stl_wj_tree.h"
#include<iostream>
#include"stl_wj_algorithm.h"
#include"stl_wj_function.h"


int main(){
    wj::rb_tree<int,int,wj::identity<int>,wj::less<int>> itree;
    std::cout<<itree.size()<<std::endl;

    itree.insert_unique(10);
    itree.insert_unique(7);
    itree.insert_unique(8);

    itree.insert_unique(15);
    itree.insert_unique(5);
    itree.insert_unique(6);

    itree.insert_unique(11);

    itree.insert_unique(13);
    itree.insert_unique(12);

    // itree.insert_equal(13);
    // itree.insert_equal(13);
    // itree.insert_equal(13);
    // itree.insert_equal(12);

    std::cout<<itree.size()<<std::endl;
    for(auto ite=itree.begin();ite!=itree.end();++ite)
        std::cout<<*ite<<' ';
    std::cout<<std::endl;

    //测试颜色
    wj::__rb_tree_base_iterator rbtite;
    for(auto ite=itree.begin();ite!=itree.end();++ite)
    {
        //向上转型up-casting
        rbtite=wj::__rb_tree_base_iterator(ite);
        std::cout<<*ite<<'('<<rbtite.node->color<<") ";
    }
    std::cout<<std::endl;

    return 0;
}
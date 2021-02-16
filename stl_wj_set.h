#ifndef __WJSET__
#define __WJSET__

/*
    本文件实现set
*/


#include"stl_wj_tree.h"
#include"stl_wj_algorithm.h"    //for less function
#include"stl_wj_function.h"     //for identity function

namespace wj{
    template<class Key,class Compare = wj::less<Key>>
    class set{
        public:
            typedef Key key_type;
            typedef Key value_type;
            //key和value使用同一个比较函数
            typedef Compare key_compare;
            typedef Compare value_compare;

            typedef size_t size_type;
        private:
            typedef wj::rb_tree<key_type, value_type, wj::identity<value_type>, key_compare> rep_type;
            rep_type t;     //采用红黑树来表现set
        public:
            typedef typename rep_type::const_pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::const_reference reference;
            typedef typename rep_type::const_reference const_reference;
            //set中使用红黑树的const iterator，不允许用户在任意处写入操作
            typedef typename rep_type::const_iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;
        public:
            //ctor
            set():t(Compare()) {}
            explicit set(const Compare& comp):t(comp) {}
            set& operator=(const set& x){
                t=x.t;
            }
        public:
            //accessors:
            key_compare key_comp() const { return t.key_comp(); }
            value_compare value_comp() const { return t.key_comp(); }
            iterator begin() const { return t.begin(); }
            iterator end() const { return t.end(); }
            bool empty() const { return t.empty(); }
            size_type size() const { return t.size(); }
            size_type max_size() const { return t.max_size(); }
            //insert
            typedef wj::pair<iterator,bool> pair_iterator_bool;
            wj::pair<iterator,bool> insert(const value_type& x){
                wj::pair<typename rep_type::iterator,bool> p=t.insert_unique(x);
                return wj::pair<iterator,bool>(p.first,p.second);
            }
            //clear
            void clear() { t.clear(); }
            //find
            iterator find(const key_type& x) const { return t.find(x); }
    };
}

#endif /* __WJSET__ */
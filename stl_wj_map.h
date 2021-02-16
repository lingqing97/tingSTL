#ifndef __WJMAP__
#define __WJMAP__

/*
    本文件实现map
*/

#include"stl_wj_algorithm.h"    //for less function
#include"stl_wj_pair.h"         //for class pair
#include"stl_wj_tree.h"
#include"stl_wj_function.h"

namespace wj{
    template<class Key,class T,class Compare=wj::less<Key>>
    class map{
        public:
            //typedefs
            typedef Key key_type;   //键值类别
            typedef T data_type;    //数据类别
            typedef T mapped_type;
            typedef wj::pair<const Key,T> value_type;   //元素类型
            typedef Compare key_compare;    //键值比较函数

            //定义一个functor,其作用就是调用"元素比较函数
            class value_compare:public wj::binary_function<value_type,value_type,bool>{
                friend class map;
                protected:
                    Compare comp;
                    value_compare(Compare c):comp(c) {}
                public:
                    bool operator()(const value_type& x,const value_type& y) const{
                        return comp(x.first,y.first);
                    }
            };

        private:
            typedef wj::rb_tree<key_type,value_type,wj::select1st<value_type>,key_compare> rep_type;
            rep_type t;     //以红黑树表现map
        public:
            typedef typename rep_type::pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::reference reference;
            typedef typename rep_type::const_reference  const_reference;
            typedef typename rep_type::iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;
            typedef typename rep_type::size_type size_type;
            typedef typename rep_type::difference_type difference_type;
        public:
            //ctor
            map():t(Compare()) {}
            explicit map(const Compare& comp):t(comp) {}
        public:
            //accessors
            key_compare key_comp() const { return t.key_comp(); }
            value_compare value_comp() const { return value_compare(t.key_comp()); }
            iterator begin() { return t.begin(); }
            const_iterator begin() const { return t.begin(); }
            iterator end() { return t.end(); }
            const_iterator end() const { return t.end(); }
            bool empty() const { return t.empty(); }
            size_type size() const { return t.size(); }
            size_type max_size() const { return t.max_size();}
            //subscript operator
            T& operator[](const key_type& k){
                return (*(insert(value_type(k,T())).first)).second;
            }
            //insert
            wj::pair<iterator,bool> insert(const value_type& x){
                //返回pair
                return t.insert_unique(x);
            }
            //find
            iterator find(const key_type& x) { return t.find(x); }
    };
}

#endif /* __WJMAP__ */
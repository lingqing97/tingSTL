#ifndef __WJLIST__
#define __WJLIST__

#include"stl_memory.h"      //for stl_wj_list.h stl_wj_construct.h
#include"stl_wj_iterator.h"

namespace wj{

    //定义链表节点
    template<class T>
    struct __list_node{
        __list_node* prev;
        __list_node* next;
        T data;
    };

    //定义链表迭代器
    template<class T,class Ref,class Ptr>
    struct __list_iterator{

        //下面两个声明其实作用是一样的，只是会为后面代码的可读性
        typedef __list_iterator<T,T&,T*>        iterator;
        typedef __list_iterator<T,Ref,Ptr>      self;

        //这里未继承iterator,所以显示定义迭代器的五种相应类别
        typedef T                               value_type;
        typedef Ptr                             pointer;
        typedef Ref                             reference;
        typedef ptrdiff_t                       difference_type;
        typedef wj::bidirectional_iterator_tag  iterator_category;

        typedef __list_node<T>* link_type;
        typedef size_t size_type;

        //指向一个链表节点
        link_type node;

        //定义ctor
        __list_iterator(){}
        __list_iterator(link_type x):node(x){}
        __list_iterator(const iterator& x):node(x.node){}

        //重载相关操作
        bool operator==(const iterator& x) const { return node==x.node; }
        bool operator!=(const iterator& x) const { return node!=x.node; }
        reference operator*() const { return (*node).data; }
        pointer operator->() const { return &(operator*()); }
        self& operator++(){
            node=(*node).next;
            return *this;
        }
        self operator++(int){
            self tmp=*this;
            node=(*node).next;
            return tmp;
        }
        self& operator--(){
            node=(*node).prev;
            return *this;
        }
        self operator--(int){
            self tmp=*this;
            node=(*node).prev;
            return tmp;
        }
    };

    //定义list
    //这里的Alloc性能并不优,一般情况下很少自己定义内存分配器
    template<class T,class Alloc=wj::alloc<__list_node<T>>>
    class list{
        protected:
            typedef __list_node<T>                    list_node;
            typedef wj::simple_alloc<list_node,Alloc> list_node_allocator;
        public:
            typedef list_node*                        link_type;
            typedef __list_iterator<T,T&,T*>          iterator;
            typedef T                                 value;
            typedef T*                                pointer;
            typedef T&                                reference;
            typedef ptrdiff_t                         difference_type;
            typedef size_t                            size_type;

            //获取头尾节点的迭代器
            iterator begin() { return (link_type)((*node).next);}
            iterator end() { return (link_type)node; }

            //取头尾节点的元素值
            reference front() { return *begin(); }
            reference back() { return *(--end());}

            //定义大小相关的函数
            bool empty() { return node->next==node; }
            size_type size() {
                size_type result=0;
                wj::distance(begin(),end(),result);
                return result;
            }

        protected:
            //定义链表中内存管理相关函数
            link_type get_node() { return list_node_allocator::allocate(); }
            void put_node(link_type p) { list_node_allocator::deallocate(p); }
            link_type create_node(const T& x){
                link_type p=get_node();         //先申请内置再构造
                wj::construct(&p->data,x);
                return p;
            }
            void destory_node(link_type p){
                wj::destory(&p->data);          //先析构再释放内存
                put_node(p);
            }
        public:
            list() { empty_initialize(); }
            template<class InputIterator>
            list(InputIterator first, InputIterator last){ range_init(first, last); }

            ~list() {
                clear();
                put_node(node);
            }
        protected:
            void empty_initialize(){
                node=get_node();        //配置一个节点空间，令node指向它
                node->next=node;        //令node头尾都指向自己，不设元素值
                node->prev=node;
            }
            template<class InputIterator>
            void range_init(InputIterator first,InputIterator last){
                empty_initialize();
                insert(begin(),first,last);
            }
        protected:
            link_type node;             //只要一个指针，便可表示整个环状双向链表
        public:
            //插入相关操作
            //函数目的：在迭代器position所指位置插入一个节点，内容为x
            //函数返回：返回新创建的节点
            iterator insert(iterator position,const T& x){
                link_type tmp=create_node(x);
                tmp->next=position.node;
                tmp->prev=position.node->prev;
                position.node->prev->next=tmp;
                position.node->prev=tmp;
                return tmp;
            }
            template<class InputIterator>
            void insert(iterator position,InputIterator first,InputIterator last){
                for(;first!=last;++first){
                    insert(position,*first);
                }
            }
            void push_back(const T& x) { insert(end(),x);}
            void push_front(const T& x){ insert(begin(),x);}

            //删除相关操作
            //函数目的：移除迭代器position所指节点
            iterator erase(iterator position){
                link_type prev_node=position.node->prev;
                link_type next_node=position.node->next;
                prev_node->next=next_node;
                next_node->prev=prev_node;
                destory_node(position.node);
                return iterator(next_node);
            }
            void pop_back(){
                iterator tmp=end();
                erase(--tmp);
            }
            void pop_front(){ erase(begin());}

            //清除所有节点
            void clear();
            //将数值为value之所有元素移除
            void remove(const T&);
            //移除数值相同的连续元素。注意，只有“连续而相同的元素”，才会被移除剩余一个
            void unique();
        protected:
            //迁移操作
            //函数目的：将[first,last)内的所有元素移动到position之前
            void transfer(iterator,iterator,iterator);
        public:

            //迁移操作的公开接口
            //将x界河于position所指位置之前。x必须不同于*this
            void splice(iterator position ,list& x){
                if(!x.empty()) transfer(position,x.begin(),x.end());
            }

            //将[first,last)内的所有元素接合于postition所指所指位置之前.
            //position和[first,last)可指向同一个list
            //但position不能位于[first,last)之内
            void splice(iterator position,list&,iterator first,iterator last){
                if(first!=last)
                    transfer(position,first,last);
            }

            //merge()将x合并到*this身上。两个lists的内容都必须先经过递增排序
            void merge(list&);
            //reverse()将*this的内容逆向重置
            void reverse();
    };

    template<class T,class Alloc>
    void list<T,Alloc>::clear(){
        link_type cur=node->next;       //头结点
        while(cur!=node){
            link_type tmp=cur;
            cur=cur->next;
            destory_node(tmp);
        }
        //恢复node原始状态
        node->next=node;
        node->prev=node;
    }

    template<class T,class Alloc>
    void list<T,Alloc>::remove(const T& value){
        iterator first=begin();
        iterator last=end();
        while(first!=last){
            iterator next=first;
            ++next;
            if(*first==value){ erase(first); }
            first=next;
        }
    }

    template<class T,class Alloc>
    void list<T,Alloc>::unique(){
        iterator first=begin();
        iterator last=end();
        if(first==last) return;     //空链表，什么都不做
        iterator next=first;
        while(++next!=last){
            if(*first==*next)
                erase(next);
            else
                first=next;
            next=first;
        }
    }

    template<class T,class Alloc>
    void list<T,Alloc>::transfer(iterator position,iterator first,iterator last){
        if(position!=last){
            (*(link_type((*last.node).prev))).next=position.node;
            (*(link_type((*first.node).prev))).next=last.node;
            (*(link_type((*position.node).prev))).next=first.node;
            link_type tmp=link_type((*position.node).prev);
            (*position.node).prev=(*last.node).prev;
            (*last.node).prev=(*first.node).prev;
            (*first.node).prev=tmp;
        }
    }

    template<class T,class Alloc>
    void list<T,Alloc>::merge(list& x){
        iterator first1=begin();
        iterator last1=end();
        iterator first2=x.begin();
        iterator last2=x.end();

        //进行合并的前提是，两个list都已经经过递增排序
        while(first1!=last1 && first2!=last2){
            if(*first2<*first1){
                iterator next=first2;
                transfer(first1,first2,++next);
                first2=next;
            }
            else{
                ++first1;
            }
        }
        if(first2!=last2) transfer(last1,first2,last2);
    }

    template<class T,class Alloc>
    void list<T,Alloc>::reverse(){
        //如果是空链表或仅有一个元素，就不进行任何操作
        if(node->next==node||link_type(node->next)->next==node) return;
        iterator first=begin();
        ++first;
        while(first!=begin()){
            iterator old=first;
            ++first;
            transfer(begin(),old,first);
        }
    }
}

#endif

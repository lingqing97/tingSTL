#ifndef __WJFORWARDLIST__
#define __WJFORWARDLIST__

/*
    本文件实现forward_list
    在GC2.9版本中forward_list被定义为非标准的slist
    本文件也使用slist
*/

#include"stl_memory.h"      //for wj::alloc

namespace wj{
    //单向链表的节点
    struct __slist_node_base{
        __slist_node_base* next;
    };

    template<class T>
    struct __slist_node:public __slist_node_base{
        T data;
    };

    //全局函数：已知某一节点，插入新节点于其后
    inline __slist_node_base* __slist_make_link(
        __slist_node_base* prev_node, __slist_node_base* new_node)
    {
        //令new节点的下一节点为prev节点的下一节点
        new_node->next=prev_node->next;
        prev_node->next=new_node;
        return new_node;
    }

    //全局函数：单向链表的大小（元素大小）
    inline size_t __slist_size(__slist_node_base* node)
    {
        size_t result=0;
        for(;node!=nullptr;node=node->next)
            ++result;
        return result;
    }

    //全局函数：查找某节点的前一个节点
    inline __slist_node_base* __slist_node_previous(
        __slist_node_base* head, const __slist_node_base* node)
    {
        while(head&&head->next!=node)
            head=head->next;
        return head;
    }

    //单向链表的迭代器
    struct __slist_iterator_base
    {
        typedef size_t                   size_type;
        typedef ptrdiff_t                difference_type;
        typedef wj::forward_iterator_tag iterator_category; //单向

        __slist_node_base* node;        //指向节点基本结构

        __slist_iterator_base(__slist_node_base* x):node(x){}

        void incr() { node=node->next; }    //前进一个节点

        bool operator==(const __slist_iterator_base& x) const{
            return node==x.node;
        }
        bool operator!=(const __slist_iterator_base& x) const{
            return node!=x.node;
        }
    };

    template<class T,class Ref,class Ptr>
    struct __slist_iterator:public __slist_iterator_base
    {
        typedef __slist_iterator<T,T&,T*>       iterator;
        typedef __slist_iterator<T,const T&,const T*> const_iterator;
        typedef __slist_iterator<T,Ref,Ptr>     self;

        typedef T              value_type;
        typedef Ptr            pointer;
        typedef Ref            reference;
        typedef __slist_node<T> list_node;

        //ctor
        __slist_iterator(list_node* x):__slist_iterator_base(x) {}      //这里的node使用了多态,将派生类指针赋值给基类指针
        __slist_iterator():__slist_iterator_base(0){}
        __slist_iterator(const iterator& x):__slist_iterator_base(x.node){}

        //elements access
        reference operator*() const { return ((list_node*)node)->data; }//将基类指针强制转为派生类指针
        pointer operator->() const { return &(operator*()); }

        self& operator++(){
            incr();     //前进一个节点
            return *this;
        }
        self operator++(int){
            self tmp=*this;
            incr();     //前进一个节点
            return tmp;
        }
    };

    //slist实现
    template<class T,class Alloc=wj::alloc<__slist_node<T>>>
    class slist{
        public:
            typedef T       value_type;
            typedef value_type* pointer;
            typedef const value_type* const_pointer;
            typedef value_type& reference;
            typedef const value_type& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;

            typedef __slist_iterator<T,T&,T*> iterator;
            typedef __slist_iterator<T,const T&,const T*> const_iterator;

        private:
            typedef __slist_node<T> list_node;
            typedef __slist_node_base list_node_base;
            typedef __slist_iterator_base iterator_base;
            typedef wj::simple_alloc<list_node,Alloc> list_node_allocator;

            //内存管理
            static list_node* create_node(const value_type& x){
                list_node* node=list_node_allocator::allocate();//配置空间
                try{
                    wj::construct(&node->data,x);   //构造元素
                    node->next=0;
                }
                catch(...){
                    list_node_allocator::deallocate(node);
                }
                return node;
            }
            static void destory_node(list_node* node){
                wj::destory(&node->data);       //将元素析构
                list_node_allocator::deallocate(node);
            }
        private:
            //删除节点
            //删除pos的后一个节点
            list_node_base* erase_after(list_node_base* pos){
                list_node* node =(list_node*)pos->next;
                list_node* next_next=(list_node*)node->next;
                pos->next = next_next;
                destory_node(node);
                return next_next;
            }
            //删除(sfirst,last)之间的节点
            list_node_base* erase_after(list_node_base* first,list_node_base* last){
                list_node* node =(list_node*)first->next;
                while(node!=last){
                    list_node* tmp=node;
                    node=(list_node*)node->next;
                    destory_node(tmp);
                }
                first->next=last;
                return last;
            }
        private:
            //dummy node
            list_node_base head;        //头部，注意，它不是指针，是实物
        public:
            slist() { head.next=nullptr; }
            ~slist(){ clear(); }
        public:
            iterator begin() { return iterator((list_node*)head.next); }
            iterator end() { return iterator(nullptr); }
            size_type size() const { return __slist_size(head.next); }
            bool empty() const { return head.next==nullptr; }
            //两个slist交换：只要将head交换互指即可
            void swap(slist& L){
                list_node_base* tmp=head.next;
                head.next=L.head.next;
                L.head.next=tmp;
            }
        public:
            //删除操作
            iterator erase(iterator pos){
                return (list_node*)erase_after(__slist_node_previous(&head,pos.node));
            }
        public:
            //取头部元素
            reference front() { return ((list_node*)head.next)->data; }
            //从头部插入元素
            void push_front(const value_type& x){
                __slist_make_link(&head,create_node(x));
            }
            //从头部取走元素（删除之）
            void pop_front() {
                list_node* node=(list_node*) head.next;
                head.next=node->next;
                destory_node(node);
            }
            //删除节点
            iterator erase_after(iterator pos){
                return (list_node*)erase_after(pos.node);
            }
            iterator erase_after(iterator before_first,iterator last){
                return (list_node*)erase_after(before_first.node,last.node);
            }
            //清空链表数据
            void clear(){
                //这里&head是__slist_node_base转换为__slist_iterator
                erase_after(&head,nullptr);
            }
    };
};

#endif /* __WJFORWARDLIST__ */
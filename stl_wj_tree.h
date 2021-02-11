#ifndef __WJTREE__
#define __WJTREE__

/*
    本文件实现rb_tree
*/

#include"stl_memory.h"      //for alloc
#include"stl_wj_iterator.h"
#include<cstddef>           //for ptrdiff_t
#include"stl_wj_pair.h"     //for pair

namespace wj{
    typedef bool        __rb_tree_color_type;
    const __rb_tree_color_type __rb_tree_red   = false;       //红色为0
    const __rb_tree_color_type __rb_tree_black = true;      //黑色为1

    //定义红黑树节点
    struct __rb_tree_node_base{
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node_base* base_ptr;

        color_type      color;      //节点颜色，非红即黑
        base_ptr        parent;     //父节点
        base_ptr        left;       //指向左节点
        base_ptr        right;      //指向右节点

        static base_ptr minimum(base_ptr x)
        {
            while(x->left!=nullptr)
                x=x->left;
            return x;
        }

        static base_ptr maximum(base_ptr x)
        {
            while(x->right!=nullptr)
                x=x->right;
            return x;
        }
    };

    //函数声明
    inline void __rb_tree_rebalance(__rb_tree_node_base*, __rb_tree_node_base*&);
    inline void __rb_tree_rotate_left(__rb_tree_node_base*, __rb_tree_node_base*&);
    inline void __rb_tree_rotate_right(__rb_tree_node_base*, __rb_tree_node_base*&);

    template<class Value>
    struct __rb_tree_node: public __rb_tree_node_base
    {
        typedef __rb_tree_node<Value>* link_type;
        Value value_field;          //节点值
    };

    //红黑树的迭代器
    struct __rb_tree_base_iterator
    {
        typedef __rb_tree_node_base::base_ptr       base_ptr;
        typedef wj::bidirectional_iterator_tag      iterator_category;
        typedef ptrdiff_t                           difference_type;

        base_ptr node;      //它用来与容器之间产生一个连结关系

        //以下其实可实现于operator++内，因为再无他处会调用该函数
        void increment()
        {
            if(node->right!=nullptr){       //如果有右节点
                node=node->right;
                while(node->left!=nullptr)
                    node=node->left;
            }
            else{                           //没有右节点
                base_ptr y=node->parent;
                while(node==y->right){
                    node=y;
                    y=y->parent;
                }
                if(node->right!=y)          //node不为header的情况
                    node=y;
            }
        }

        //以下其实可实现于operator--内，因为再无他处会调用该函数
        void decrement()
        {
            if(node->color==__rb_tree_red &&        //如果是红节点,且
                node->parent->parent==node)         //父节点的父节点等于自己
                node=node->right;                   //这种情况发生于node为header时
            else if(node->left!=nullptr){           //如果有左节点
                base_ptr y=node->left;
                while(y->right!=nullptr)
                    y=y->right;
                node=y;
            }
            else{                                   //既非根节点，亦无左子节点
                base_ptr y=node->parent;
                while(node==y->left){
                    node=y;
                    y=y->parent;
                }
                node=y;
            }
        }
    };

    //红黑树的正规迭代器
    template<class Value,class Ref,class Ptr>
    struct __rb_tree_iterator: public __rb_tree_base_iterator
    {
        typedef Value value_type;
        typedef Ref   reference;
        typedef Ptr   pointer;
        typedef __rb_tree_iterator<Value,Value&,Value*>              iterator;
        typedef __rb_tree_iterator<Value,const Value&,const Value*>  const_iterator;
        typedef __rb_tree_iterator<Value,Ref,Ptr>     self;
        typedef __rb_tree_node<Value>*                link_type;

        //ctor
        __rb_tree_iterator(){}
        __rb_tree_iterator(link_type x) { node=x; }
        __rb_tree_iterator(const iterator& it) { node=it.node; }

        reference operator*() const { return link_type(node)->value_field; }
        pointer operator->() const { return &(operator*()); }

        self& operator++() { increment(); return *this; }
        self operator++(int){
            self tmp=*this;
            increment();
            return tmp;
        }
        self& operator--() { decrement(); return *this; }
        self operator--(int){
            self tmp=*this;
            decrement();
            return tmp;
        }
    };

    inline bool operator==(const __rb_tree_base_iterator& x,
                            const __rb_tree_base_iterator& y)
    {
        return x.node == y.node;
    }

    inline bool operator!=(const __rb_tree_base_iterator& x,
                            const __rb_tree_base_iterator& y)
    {
        return x.node != y.node;
    }

    //定义红黑树
    template<class Key,class Value,class KeyOfValue,class Compare,class Alloc=wj::alloc<__rb_tree_node<Value>>>
    class rb_tree{
        protected:
            typedef void* void_pointer;
            typedef __rb_tree_node_base*    base_ptr;
            typedef __rb_tree_node<Value>   rb_tree_node;
            typedef wj::simple_alloc<rb_tree_node,Alloc> rb_tree_node_allocator;
            typedef __rb_tree_color_type    color_type;
        public:
            typedef Key     key_type;
            typedef Value   value_type;
            typedef value_type*         pointer;
            typedef const value_type*   const_pointer;
            typedef value_type&         reference;
            typedef const value_type&   const_reference;
            typedef rb_tree_node*       link_type;
            typedef size_t  size_type;
            typedef ptrdiff_t           difference_type;
        public:
            typedef __rb_tree_iterator<value_type,reference,pointer> iterator;
        protected:
            link_type get_node() { return rb_tree_node_allocator::allocate(); }
            void put_node(link_type p)  { rb_tree_node_allocator::deallocate(p); }

            link_type create_node(const value_type& x){
                link_type tmp=get_node();                   //申请内存
                try{
                    wj::construct(&tmp->value_field,x);     //元素构造
                }
                catch(...){
                    put_node(tmp);
                }
                return tmp;
            }

            link_type clone_node(link_type x)               //复制一个节点(的值和颜色)
            {
                link_type tmp=create_node(x->value_field);
                tmp->color=x->color;
                tmp->left=nullptr;
                tmp->right=nullptr;
                return tmp;
            }

            void destory_node(link_type p){
                wj::destory(&p->value_field);               //元素析构
                put_node(p);                                //释放内存
            }
        protected:
            //红黑树的数据成员
            size_type node_count;               //记录树的大小（节点数量）
            link_type header;                   //header节点
            Compare   key_compare;              //节点间的键值大小比较准则，应该会是个function object

            //以下三个函数用来方便取得header的成员
            link_type& root() const { return (link_type&)header->parent; }
            link_type& leftmost() const { return (link_type&)header->left; }
            link_type& rightmost() const { return (link_type&)header->right; }

            //以下六个函数用来方便取得节点x的成员
            static link_type& left(link_type x)
            { return (link_type&)(x->left); }
            static link_type& right(link_type x)
            { return (link_type&)(x->right); }
            static link_type& parent(link_type x)
            { return (link_type&)(x->parent); }
            static reference value(link_type x)
            { return x->value_field; }
            static const key_type& key(link_type x)
            { return KeyOfValue()(value(x)); }
            static color_type& color(link_type x)
            { return (color_type&)(x->color); }

            //以下六个函数用来方便取得节点x的成员
            static link_type& left(base_ptr x)
            { return (link_type&)(x->left); }
            static link_type& right(base_ptr x)
            { return (link_type&)(x->right); }
            static link_type& parent(base_ptr x)
            { return (link_type&)(x->parent); }
            static reference value(base_ptr x)
            { return ((link_type)x)->value_field; }
            static const key_type& key(base_ptr x)
            { return KeyOfValue()(value(link_type(x))); }
            static color_type& color(base_ptr x)
            { return (color_type&)(link_type(x)->color); }

            //求取极大值和极小值
            static link_type minimum(link_type x){
                return (link_type) __rb_tree_node_base::minimum(x);
            }
            static link_type maximum(link_type x){
                return (link_type) __rb_tree_node_base::maximum(x);
            }
        private:
            iterator __insert(base_ptr x,base_ptr y,const value_type& v);
            //link_type __copy(link_type x,link_type p);
            void __erase(link_type x){
                while(x!=nullptr){
                    __erase(right(x));
                    link_type y=left(x);
                    destory_node(x);
                    x=y;
                }
            }            void init(){
                header=get_node();            //产生一个节点空间，令head指向它
                color(header)=__rb_tree_red;  //令header为红色，用来区分header和root
                root()=nullptr;
                leftmost()=header;              //令header的左子节点为自己
                rightmost()=header;             //令header的右子节点为自己
            }
        public:
            rb_tree(const Compare& comp=Compare())
                :node_count(0),key_compare(comp) { init(); }
            rb_tree& operator=(const rb_tree& x);
            ~rb_tree(){
                clear();
                put_node(header);
            }
        public:
            void clear(){
                if(node_count!=0){
                    __erase(root());
                    leftmost()=header;
                    rightmost()=header;
                    root()=nullptr;
                    node_count=0;
                }
            }
            Compare key_comp() const { return key_compare; }
            iterator begin() { return leftmost(); }     //RB述的起头为最左节点
            iterator end() { return header; }      //RB树的终点为header所指处
            bool empty() const { return node_count == 0; }
            size_type size() const { return node_count; }
            size_type max_size() const { return size_type(-1); }
        public:
            //将x插入到RB-tree中（保持节点值独一无二）
            //若插入已有值，则不会进行任何操作
            wj::pair<iterator,bool> insert_unique(const value_type& x);
            //将x插入到RB-turee中（允许节点值重复）
            iterator insert_equal(const value_type& x);
        public:
            iterator find(const Key& k);
    };
    //插入新值；节点键值允许重复
    //注意，返回值是一个RB-tree迭代器，指向新增节点
    template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v)
    {
        link_type y=header;
        link_type x=root();         //从根节点开始
        while(x!=nullptr){
            y=x;
            //v的键值更小则往左子树移动，否则往右子树移动
            x=key_compare(KeyOfValue()(v),key(x))?left(x):right(x);
        }
        //x为新值插入点，y为插入点之父节点，v为新值
        return __insert(x,y,v);
    }

    //插入新值；节点键值不允许重复，若重复插入无效
    //注意，返回是一个pair,第一元素是个RB-tree迭代器，指向新增节点
    //第二元素表示插入成功与否
    template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    wj::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
    {
        link_type y=header;
        link_type x=root();         //从根节点开始
        bool comp=true;
        while(x!=nullptr){
            y=x;
            comp=key_compare(KeyOfValue()(v),key(x));
            x=comp?left(x):right(x);
        }
        iterator j=iterator(y);
        if(comp){
            if(j==begin())
                return wj::pair<iterator,bool>(__insert(x,y,v),true);
            else        //移动到前一个节点
                --j;
        }
        if(key_compare(key(j.node),KeyOfValue()(v)))
            //v大于前一个节点
            return wj::pair<iterator,bool>(__insert(x,y,v),true);
        //表示新值一定与树中键值重复，那么就不该插入新值
        return wj::pair<iterator,bool>(j,false);
    }

    //真正的插入执行程序
    template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_,base_ptr y_,const Value& v)
    {
        //参数x_为新值插入点，参数y_为插入点之父节点，参数v为新值
        link_type x=(link_type) x_;
        link_type y=(link_type) y_;
        link_type z;

        if(y==header||x!=nullptr||key_compare(KeyOfValue()(v),key(y))){
            z=create_node(v);
            left(y)=z;      //这使得当y为header时，leftmost()=z
            if(y==header){
                root()=z;
                rightmost()=z;
            }
            else if(y==leftmost()){ //如果y为最左节点
                leftmost()=z;       //维护leftmost(),使它永远指向最左节点
            }
        }
        else{
            z=create_node(v);       //产生一个新节点
            right(y)=z;             //令新节点成为插入点之父节点y的右子节点
            if(y==rightmost()){
                rightmost()=z;      //维护rightmost(),使它永远指向最右节点
            }
        }
        parent(z)=y;
        left(z)=nullptr;
        right(z)=nullptr;

        //新节点的颜色将在__rb_tree_rebalance()设定
        __rb_tree_rebalance(z,header->parent);
        ++node_count;
        return iterator(z);
    }

    //全局函数
    //重新令树形平衡(改变颜色及旋转树形)
    //参数一位新增节点，参数二维root
    inline void
    __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        x->color=__rb_tree_red;     //新节点必为红
        while(x!=root&&x->parent->color==__rb_tree_red){    //父节点为红色
            if(x->parent==x->parent->parent->left){   //父节点为祖父节点之左子节点
                __rb_tree_node_base* y=x->parent->parent->right;
                if(y&&y->color==__rb_tree_red){   //case1: 伯父节点存在且为红
                    x->parent->color=__rb_tree_black;   //更改父节点为黑色
                    y->color=__rb_tree_black;           //更改伯父节点为黑色
                    x->parent->parent->color=__rb_tree_red; //更改祖父节点为红色
                    x=x->parent->parent;        //令祖父为新的x
                }
                else{
                    //无伯父节点，或伯父节点为黑
                    if(x==x->parent->right){    //case2:如果新节点为父节点之右子节点
                        x=x->parent;
                        __rb_tree_rotate_left(x,root);
                    }
                    //case3:
                    x->parent->color=__rb_tree_black;       //改变颜色
                    x->parent->parent->color=__rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent,root);
                }
            }
            else{   //父节点为祖父节点之右子节点
                __rb_tree_node_base* y=x->parent->parent->left;     //令y为伯父节点
                if(y&&y->color==__rb_tree_red){
                    x->parent->color=__rb_tree_black;
                    y->color=__rb_tree_black;
                    x->parent->parent->color=__rb_tree_red;
                    x=x->parent->parent;
                }
                else{   //无伯父节点，或伯父节点为黑
                    if(x==x->parent->left){
                        x=x->parent;
                        __rb_tree_rotate_right(x,root);
                    }
                    x->parent->color=__rb_tree_black;
                    x->parent->parent->color=__rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent,root);
                }
            }
        }   //while结束
        root->color=__rb_tree_black;    //根节点永远为黑
    }
    //全局函数
    //左旋函数
    inline void
    __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root){
        //x为旋转点
        __rb_tree_node_base* y=x->right;    //令y为旋转点的右子节点
        x->right=y->left;
        if(y->left!=nullptr)
            y->left->parent=x;
        y->parent=x->parent;
        if(x==root)                         //x为根节点
            root=y;
        else if(x==x->parent->left)         //x为其父节点的左子节点
            x->parent->left=y;
        else
            x->parent->right=y;
        y->left=x;
        x->parent=y;
    }

    //全局函数
    //右旋函数
    inline void
    __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root){
        //x为旋转点
        __rb_tree_node_base* y=x->left;     //令y为旋转点的左子节点
        x->left=y->right;
        if(y->right!=nullptr)
            y->right->parent=x;
        y->parent=x->parent;
        if(x==root)
            root=y;
        else if(x==x->parent->right)
            x->parent->right=y;
        else
            x->parent->left=y;
        y->right=x;
        x->parent=y;
    }

    //寻找RB树中是否有键值为k的节点
    template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k){
        link_type y=header;
        link_type x=root();

        while(x!=0){
            //x的键值大于等于k
            if(!key_compare(key(x),k)){
                y=x,x=left(x);
            }
            else{
                x=right(x);
            }
        }
        iterator j=iterator(y);
        return (j==end()||key_compare(k,key(j.node)))?end():j;
    }
}

#endif /* __WJTREE__ */
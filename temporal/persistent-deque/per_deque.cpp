#include <iostream>
#include <vector>

#define item int
using namespace std;

typedef struct cell{
    int D;
    item val;
    struct cell *jump;
    struct cell *parent;
} node;
typedef struct{
    node *first;
    node *last;
} deq;

//Auxiliary Functions
node* LevelAncestor(int, node*);
node* LowestCommonAncestor(node*, node*);
deq Swap(deq);

//Node Addition Functions
node* AddLeaf(node*);
node* NewNode(item, node*, int);
deq Deque();
deq PushFront(deq, item);
deq PushBack(deq, item);

//Query functions
item Front(deq);
item Back(deq);
item Kth(deq, int);
void Print(deq);

//Node Removal Functions
deq PopFront(deq);
deq PopBack(deq);

node* AddLeaf(node *u){
    node *v = u->parent;
    if(v->jump != nullptr && v->jump->jump != nullptr && (v->D - v->jump->D) == (v->jump->D - v->jump->jump->D))
        u->jump = v->jump->jump;
    else
        u->jump = v;
    return u;
}
node* NewNode(item x, node *p, int d){
    node *u = (node*)malloc(sizeof(node));
    u->D = d;
    u->val = x;
    u->parent = p;
    
    if(p != nullptr)
        u = AddLeaf(u);
    return u;
}
deq Swap(deq d){
    deq aux;
    aux.first = d.last;
    aux.last = d.first;
    return aux;
}
deq Deque(){
    deq u;
    u.first = nullptr;
    u.last = nullptr;
    return u;
}
deq PushFront(deq d, item x){
    deq v;
    if(d.first == nullptr){
        node *u = NewNode(x, nullptr, 1);
        v.first = u;
        v.last = u;
    }
    else{
        v.first = NewNode(x, d.first, d.first->D + 1);
        v.last = d.last;
    }
    return v;
}
deq PushBack(deq d, item x){
    return Swap(PushFront(Swap(d), x));
}

node* LevelAncestor(int k, node *u){
    int y = u->D - k;
    while(u->D != y){
        if(u->jump->D >= y)
            u = u->jump;
        else
            u = u->parent;
    }
    return u;
}
node* LowestCommonAncestor(node *u, node *v){
    if(u->D > v->D){
        node *aux = u;
        u = v;
        v = aux;
    }
    v = LevelAncestor(v->D - u->D, v);
    if(u == v)
        return u;
    
    while(u->parent != v->parent){
        if(u->jump != v->jump){
            u = u->jump;
            v = v->jump;
        }
        else{
            u = u->parent;
            v = v->parent;
        }
    }
    return u->parent;
}
item Front(deq d){
    return d.first->val;
}
item Back(deq d){
    return d.last->val;
}
item Kth(deq d, int k){
    node *mid = LowestCommonAncestor(d.first, d.last);
    int l1 = d.first->D - mid->D;
    int l2 = d.last->D - mid->D;
    item value;

    if(k-1 <= l1)
        value = LevelAncestor(k-1, d.first)->val;

    else
        value = LevelAncestor(l2 - (k-1 - l1), d.last)->val;
    
    return value;
}
void Print(deq d){
    if(d.first != nullptr){
        node *mid = LowestCommonAncestor(d.first, d.last);
        int l1 = d.first->D - mid->D;
        int l2 = d.last->D - mid->D;

        node *aux = d.first;
        for(int i = 0; i < l1; i++){
            cout << aux->val << " ";
            aux = aux->parent;
        }

        cout << mid->val << " ";
        vector<item> vec;
        aux = d.last;

        for(int i = 0; i < l2; i++){
            vec.push_back(aux->val);
            aux = aux->parent;
        }
        while(!vec.empty()){
            cout << vec.back() << " ";
            vec.pop_back();
        }
    }
    cout << endl;
}

deq PopFront(deq d){
    node *mid = LowestCommonAncestor(d.first, d.last);
    deq v;
    
    if(d.first != mid){
        v.first = d.first->parent;
        v.last = d.last;
    }
    else if(d.last != mid){
        v.first = LevelAncestor(d.last->D - d.first->D - 1, d.last);
        v.last = d.last;
    }
    else
        v.first = v.last = nullptr;

    return v;
}
deq PopBack(deq d){
    return Swap(PopFront(Swap(d)));
}

int main(){
    vector<deq> deq_versions;
    int t, x, k, option;

    deq_versions.push_back(Deque());

    while(cin >> option){
        cin >> t;

        if(option == 1){
            cin >> x;
            deq_versions.push_back(PushFront(deq_versions[t], x));
        }
        else if(option == 2){
            cin >> x;
            deq_versions.push_back(PushBack(deq_versions[t], x));
        }
        else if(option == 3)
            deq_versions.push_back(PopFront(deq_versions[t]));        
        else if(option == 4)
            deq_versions.push_back(PopBack(deq_versions[t]));
        else if(option == 5)
            cout << Front(deq_versions[t]) << endl;
        else if(option == 6)
            cout << Back(deq_versions[t]) << endl;
        else if(option == 7){
            cin >> k;
            cout << Kth(deq_versions[t], k) << endl;
        }
        else
            Print(deq_versions[t]);
    }
    return 0;
}
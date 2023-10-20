#include <iostream>
#include <vector>

#define item int
using namespace std;

//in order to change the priority range
int range_rand = 2000;
int range_aux = 1;

//in order to don't copy the node that will be deleted more than once
bool is_copy;

typedef struct cell{
    item key;
    int prior;
    struct cell* left;
    struct cell* right;
} node;
typedef struct{
    node* raiz;
} treap;

node* Copy(node* r){
    node* aux;
    aux = (node*)malloc(sizeof(node));
    aux->right = r->right;
    aux->key = r->key;
    aux->prior = r->prior;
    aux->left = r->left;
    return aux;
}

node* rotationRight(node* r){
    node* aux; 
    if(r != nullptr && r->left != nullptr){
        aux = r->left;
        r->left = aux->right;
        aux->right = r;
        r = aux;
    }
    return r;
}
node* rotationLeft(node* r){
    node*aux; 
    if(r != nullptr && r->right != nullptr){
        aux = r->right;
        r->right = aux->left;
        aux->left = r;
        r = aux;
    }
    return r;
}

treap Treap(){
    treap u;
    u.raiz = nullptr;
    return u;
}
node* Insert(node* r, item x){
    if(r == nullptr){
        r = (node*)malloc(sizeof(node));
        r->right = r->left = nullptr;
        r->prior = rand() % range_rand + range_aux;
        r->key = x;
    }
    else if(r->key > x){
        r = Copy(r);
        r->left = Insert(r->left, x);
        if(r->prior < r->left->prior)
            r = rotationRight(r);
    }
    else{
        r = Copy(r);
        r->right = Insert(r->right, x);
        if(r->prior < r->right->prior)
            r = rotationLeft(r);
    }
    return r;
}
node* Delete(node* r, item x){
    if(r != nullptr){
        if(r->key == x){
            if(!is_copy){
                r = Copy(r);
                is_copy = true;
            }
            if(r->left == nullptr && r->right == nullptr){
                free(r);
                r = nullptr;
            }
            else if(r->left == nullptr){
                node* aux = r->right;
                free(r);
                r = aux;
            }
            else if(r->right == nullptr){
                node* aux = r->left;
                free(r);
                r = aux;
            }
            else{
                if(r->left->prior > r->right->prior){
                    r->left = Copy(r->left);
                    r = rotationRight(r);
                    r->right = Delete(r->right, x);
                }
                else{
                    r->right = Copy(r->right);
                    r = rotationLeft(r);
                    r->left = Delete(r->left, x);
                }
            }
        }
        else if(r->key > x){
            r = Copy(r);
            r->left = Delete(r->left, x);    
        }
        else{
            r = Copy(r);
            r->right = Delete(r->right, x);
        }
    }
    return r;
}
bool Search(node* r, item x){
    if(r == nullptr)
        return false;
    else if(r->key == x)
        return true;
    else if(r->key > x)
        return Search(r->left, x);
    else
        return Search(r->right, x);
}
item Min(node* r){
    if(r->left == nullptr)
        return r->key;
    else
        return Min(r->left);
}
void printRec(node* r, int i){
    if(r != nullptr){
        printRec(r->left, i+3);
        for(int j = 0; j < i; j++)
            cout << " ";
        cout << r->key << " " << r->prior << endl;
        printRec(r->right, i+3);
    }
}
void Print(treap t){
    printRec(t.raiz, 0);
}

int main(){
    treap aux;
    int t, x, option;   
    vector<treap> treap_versions;

    treap_versions.push_back(Treap());

    while(cin >> option){
        cin >> t;

        if(option == 1){
            cin >> x;
            aux.raiz = Insert(treap_versions[t].raiz, x);
            treap_versions.push_back(aux);
        }
        else if(option == 2){
            cin >> x;
            is_copy = false;
            aux.raiz = Delete(treap_versions[t].raiz, x);
            treap_versions.push_back(aux);
        }
        else if(option == 3){
            cin >> x;
            cout << Search(treap_versions[t].raiz, x) << endl;       
        }
        else if(option == 4)
            cout << Min(treap_versions[t].raiz) << endl;
        else
            Print(treap_versions[t]);
    }
    return 0;
}
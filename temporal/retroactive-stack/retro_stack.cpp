//EP feito em cerca de 15h, basicamente um final de semana inteiro dedicado a ele
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

#define item int
#define LEAF -1
using namespace std;

//para alterar o range da prioridade
int range_rand = 2000;
int range_aux = 1;

/* A mesma estrutura possui definições diferentes de acordo com o tipo, interna ou folha
 * Consideramos folha todo o nó com prior = -1 */
typedef struct cell{
    /* interna: maior chave da subárvore
     * folha: tempo da operação(chave)                  */
    int maxi;

    /* interna: soma de todos os valores da subárvore
     * folha: +1 se "push", -1 se "pop"                 */
    int sum;

    /* interna: max(0, left.smax + right.sum)
     * folha: 1 se "push", 0 se "pop"                   */
    int smax;

    /* interna: valor aleatório
     * folha: -1                                        */
    int prior;

    /* interna: 0
     * folha: item inserido se "push", 0 se "pop"    */
    item value;

    struct cell* left;
    struct cell* right;
} node;

typedef struct{
    node* raiz;
} retro_stack;

//cria e devolve uma pilha retroativa vazia
retro_stack retroStack();                           

//começa a inserir uma operação na sequência de modificações no instante t 
node* start_insert(int, item, bool, node*);               
//insere recursivamente
node* rec_insert(int, item, bool, node*);                 

//começa a deletar uma operação na sequência de modificações no instante t 
node* start_delete(int, node*);                     
//deleta recursivamente
node* rec_delete(int, node*);                       

//número de elementos na pilha retroativa no instante t
int size(int, node*);   

//primeira descida de localização e compreensão da situação (t' > t?)
int kth_descent_1(int, int, node*, pair<int, bool>&, bool&);            
//segunda descida em que há garantia de achar o kth (conforme consistência dos testes)
int kth_descent_2(int, node*, pair<int, bool>&);                        

//elemento do topo da pilha retroativa no instante t
int top(int, retro_stack);                                 

//imprime todos os elementos da pilha retroativa no instante t
void print(int, retro_stack);                             


retro_stack retroStack(){
    retro_stack p;
    p.raiz = nullptr;
    return p;
}

node* rotationRight(node* r){
    node* aux; 
    aux = r->left;

    r->maxi = r->right->maxi;
    r->sum = aux->right->sum + r->right->sum;
    r->smax = max(0, aux->right->smax + r->right->sum);

    r->left = aux->right;
    aux->right = r;

    aux->maxi = r->maxi;
    aux->sum = aux->left->sum + r->sum;
    aux->smax = max(0, aux->left->smax + r->sum);
    
    r = aux;
    return r;
}
node* rotationLeft(node* r){
    node* aux; 
    aux = r->right;

    r->maxi = aux->left->maxi;
    r->sum = r->left->sum + aux->left->sum;
    r->smax = max(0, r->left->smax + aux->left->sum);

    r->right = aux->left;
    aux->left = r;

    aux->maxi = aux->right->maxi;
    aux->sum = r->sum + aux->right->sum;
    aux->smax = max(0, r->smax + aux->right->sum);
    
    r = aux;
    return r;
}

node* start_insert(int t, item x, bool is_pop, node* r){
    //pilha vazia
    if(r == nullptr){
        r = (node*)malloc(sizeof(node));
        r->maxi = t;
        if(is_pop){
            r->sum = -1;
            r->smax = 0;
        }
        else{
            r->sum = 1;
            r->smax = 1;
        }
        r->prior = LEAF;
        r->value = x;
        r->left = r->right = nullptr;
        return r;
    }
    r = rec_insert(t, x, is_pop, r);
    return r;
}
node* rec_insert(int t, item x, bool is_pop, node* r){
    //folha encontrada
    if(r->prior == LEAF){
        node* inner = (node*)malloc(sizeof(node));
        node* leaf = (node*)malloc(sizeof(node));

        leaf->maxi = t;
        if(is_pop){
            leaf->sum = -1;
            leaf->smax = 0;
        }
        else{
            leaf->sum = 1;
            leaf->smax = 1;
        }
        leaf->prior = LEAF;
        leaf->value = x;
        leaf->left = leaf->right = nullptr;

        if(leaf->maxi > r->maxi){
            inner->left = r;
            inner->right = leaf;
        }
        else{
            inner->left = leaf;
            inner->right = r;
        }
        inner->maxi = inner->right->maxi;
        inner->sum = inner->left->sum + inner->right->sum;
        inner->smax = max(0, inner->left->smax + inner->right->sum);
        inner->value = 0;
        inner->prior = rand() % range_rand + range_aux;

        return inner;
    }
    //vá para a esquerda
    else if(t < r->left->maxi){
        r->left = rec_insert(t, x, is_pop, r->left);
        if(r->prior < r->left->prior)
            r = rotationRight(r);
        else{
            r->maxi = r->right->maxi;
            r->sum = r->left->sum + r->right->sum;
            r->smax = max(0, r->left->smax + r->right->sum);
        }
    }
    //vá para a direita
    else{
        r->right = rec_insert(t, x, is_pop, r->right);
        if(r->prior < r->right->prior)
            r = rotationLeft(r);
        else{
            r->maxi = r->right->maxi;
            r->sum = r->left->sum + r->right->sum;
            r->smax = max(0, r->left->smax + r->right->sum);
        }
    }
    return r;
}

node* start_delete(int t, node* r){
    //pilha com apenas uma operação
    if(r->prior == LEAF){
        free(r);
        return nullptr;
    }
    r = rec_delete(t, r);
    return r;
}
node* rec_delete(int t, node* r){
    if(t > r->left->maxi){
        //operação encontrada
        if(r->right->prior == LEAF && r->right->maxi == t){
            node* aux = r->left;
            free(r->right); free(r);
            r = aux;
        }
        //procurar na direita
        else{
            r->right = rec_delete(t, r->right);
            r->maxi = r->right->maxi;
            r->sum = r->left->sum + r->right->sum;
            r->smax = max(0, r->left->smax + r->right->sum);
        }
    }
    else{
        //operação encontrada
        if(r->left->prior == LEAF && r->left->maxi == t){
            node* aux = r->right;
            free(r->left); free(r);
            r = aux;
        }
        //procurar na esquerda
        else{
            r->left = rec_delete(t, r->left);
            r->maxi = r->right->maxi;
            r->sum = r->left->sum + r->right->sum;
            r->smax = max(0, r->left->smax + r->right->sum);
        }
    }
    return r;
}

int size(int t, node* r){
    int sum = 0;
    while(r->prior != LEAF){
        if(r->left->maxi == t){
            sum += r->left->sum;
            break;
        }
        else if(r->left->maxi > t)
            r = r->left;
        else{
            sum += r->left->sum;
            r = r->right;
        }
    }
    if(r->prior == LEAF && r->maxi <= t)
        sum += r->sum;

    return sum;
}

/* Podemos dividir o kth em 3 etapas:
 * 1°) Descer em busca do t'. Se t' > t, voltar recursivamente em busca do primeiro 
 *     t' tal que 1 <= t' <= t que acharmos;
 * 2°) A partir desse t', voltar recursivamente procurando a subárvore que com
 *     centeza, dada a consistência do usuário, possui o k-ésimo item requisitado;
 * 3°) Quando achar essa subárvore, descer novamente em busca do item, com a 
 *     função kth_descent_2.
 */
int kth_descent_1(int t, int k, node* r, pair<int, bool>&val, bool& invalid_t){
    //Possível fim da 1ª etapa
    if(r->prior == LEAF){
        //t' é menor ou igual a t e é um 'push'
        if(r->maxi <= t && r->sum == 1){
            invalid_t = false;
            //se o kth for um top()
            if(--k == 0){
                val.first = r->value;
                val.second = true;
            }
        }
        //t' é menor ou igual a t e é um 'pop'
        else if(r->maxi <= t){
            invalid_t = false;
            val.second = false;
            k++;
        }
        //t' é maior que t, logo a operação é irrelevante
        else{
            invalid_t = true;    //preparando a busca de um t' menor que t
            val.second = false;
        }
        return k;
    }
    
    //vá para a esquerda
    else if(t <= r->left->maxi){
        //Seguindo com a 1ª etapa
        k = kth_descent_1(t, k, r->left, val, invalid_t);

        /* Achamos o kth e apenas saímos na recursão ou estamos procurando
         * um t' válido e por isso, não procuramos na folha direita,
         * já estivemos na esquerda e não faz sentido ir para a direita  */
        if(val.second || invalid_t)
            return k;   

        //Início da 2ª etapa
        else if(r->left->smax >= k)
            k = kth_descent_2(k, r->left, val);
        else
            k -= r->left->sum;
    }

    //vá para a direita
    else{
        //Seguindo com a 1ª etapa
        k = kth_descent_1(t, k, r->right, val, invalid_t);
        
        //Achamos o kth e apenas saímos na recursão
        if(val.second)
            return k;
        //Estamos procurando um t' válido e por isso vemos se ele está na esquerda
        else if(invalid_t)
            k = kth_descent_1(t, k, r->left, val, invalid_t);
        
        //Início da 2ª etapa
        else if(r->left->smax >= k)
            k = kth_descent_2(k, r->left, val);
        else
            k -= r->left->sum;
    }
    return k;
}
int kth_descent_2(int k, node* r, pair<int, bool>&val){
    //Só pode ser um 'push'
    if(r->prior == LEAF){
        val.first = r->value;
        val.second = true;
        k--;
    }
    else if(r->right->smax >= k)
        k = kth_descent_2(k, r->right, val);
    else{
        k -= r->right->sum;
        k = kth_descent_2(k, r->left, val);
    }
    return k;
}

int top(int t, retro_stack p){
    bool invalid_t;
    pair<int, bool> val;
    kth_descent_1(t, 1, p.raiz, val, invalid_t);
    return val.first;
}

void print(int t, retro_stack p){
    bool invalid_t;
    pair<int, bool> val;
    if(p.raiz != nullptr){
        int sz = size(t, p.raiz);
        for(int i = 1; i <= sz; i++){
            kth_descent_1(t, i, p.raiz, val, invalid_t);
            val.second = false;
            cout << val.first << " ";
        }
    }
    cout << endl;
}

//print de visualização da árvore
/*void printRec(node* r, int i){
    if(r != nullptr){
        printRec(r->left, i+3);
        for(int j = 0; j < i; j++)
            cout << " ";
        cout << r->maxi << " " << r->prior << endl;
        printRec(r->right, i+3);
    }
}
void Print_tree(retro_stack t){
    printRec(t.raiz, 0);
}*/

int main(){
    retro_stack p = retroStack();
    int x, t, option;

    while(cin >> option){
        cin >> t;
        if(option == 1){
            cin >> x;
            p.raiz = start_insert(t, x, false, p.raiz);
        }
        else if(option == 2)
            p.raiz = start_insert(t, 0, true, p.raiz);
        else if(option == 3)
            p.raiz = start_delete(t, p.raiz);
        else if(option == 4)
            cout << size(t, p.raiz) << endl;
        else if(option == 5)
            cout << top(t, p) << endl;       
        else if(option == 6){
            cin >> x;
            bool invalid_t;
            pair<int, bool> val;
            kth_descent_1(t, x, p.raiz, val, invalid_t);
            cout << val.first << endl;       
        }
        else
            print(t, p);
    }
    return 0;
}
#include "Estat.h"

vector<int> pre_process(vector<pair<int,int>> segs){
    int bef, aft;
    vector<int> ext;

    ext.push_back(INT_MIN);
    for(int i = 0; i < segs.size(); i++){
        ext.push_back(segs[i].first);
        ext.push_back(segs[i].second);
    }
    sort(ext.begin(), ext.end());
    ext.push_back(INT_MAX);
    
    bef = ext[0];
    for(int i = 1; i < ext.size(); i++){
        aft = ext[i];
        if(aft == bef)
            ext.erase(ext.begin() + i--);
        bef = aft;
    }

    return ext;
}

node* new_node(int s, int f, bool s_open, bool f_open, node* left, node* right){
    node* r = (node*)malloc(sizeof(node));
    r->s_open = s_open;
    r->s = s;
    r->f_open = f_open;
    r->f = f;
    r->left = left;
    r->right = right;
    return r;
}

node* AS(vector<pair<int,int>>& segs, vector<int>& index){
    node* r;
    vector<int> ext;
    vector<node*> parents;
    vector<node*> leaves;
    int sz, lf, opt, lf_last;

    ext = pre_process(segs);

    sz = ext.size();                        //# de diferentes valores dos segmentos
    lf = 2 * sz - 3;                        //qtd de folhas desta árvore
    for(opt = 2; opt < lf; opt *= 2){}      //qtd ideal de folhas na última camada
    lf_last = 2 * lf - opt;                 //qtd real de folhas na última camada
    
    //Gerando as folhas da última camada
    for(int i = 0; i < lf_last/2; i++){
        r = new_node(ext[i], ext[i+1], true, true, nullptr, nullptr);
        leaves.push_back(r);

        r = new_node(ext[i+1], ext[i+1], false, false, nullptr, nullptr);
        leaves.push_back(r);
    }

    //Gerando os pais destas folhas
    for(int i = 0; i < leaves.size()-1; i++){
        r = new_node(leaves[i]->s, leaves[i+1]->f, leaves[i]->s_open, 
                    leaves[i+1]->f_open, leaves[i], leaves[i+1]);
        parents.push_back(r);
        i++;
    }

    //Gerando o restante das folhas na penúltima camada
    for(int i = lf_last/2; i < sz-2; i++){
        r = new_node(ext[i], ext[i+1], true, true, nullptr, nullptr);
        parents.push_back(r);

        r = new_node(ext[i+1], ext[i+1], false, false, nullptr, nullptr);
        parents.push_back(r);
    }
    r = new_node(ext[sz-2], ext[sz-1], true, true, nullptr, nullptr);
    parents.push_back(r);

    //Construindo a árvore da penúltima camada até a raiz
    while(parents.size() > 1){
        leaves.clear();
        for(int i = 0; i < parents.size(); i++){
            r = new_node(parents[i]->s, parents[i+1]->f, parents[i]->s_open, 
                        parents[i+1]->f_open, parents[i], parents[i+1]);
            leaves.push_back(r);
            i++;
        }
        parents = leaves;
    }

    //Preenchendo em cada nó o campo que contém os segmentos
    for(int i = 0; i < segs.size(); i++)
        parents[0] = Fill_AS(parents[0], segs[i], index[i]);

    return parents[0];
}
node* Fill_AS(node* r, pair<int,int>& seg, int i){
    //achou o intervalo contido
    if(r->s >= seg.first && r->f <= seg.second){
        r->segs.push_back(i);
        return r;
    }
    //chama na esquerda
    if(r->left != nullptr){
        if(!r->left->f_open && r->left->f == seg.first)
            Fill_AS(r->left, seg, i);
        else if(r->left->f > seg.first)
            Fill_AS(r->left, seg, i);
    }
    //chama na direita
    if(r->right != nullptr){
        if(!r->right->s_open && r->right->s == seg.second)
            Fill_AS(r->right, seg, i);
        else if(r->right->s < seg.second)
            Fill_AS(r->right, seg, i);
    }
    return r;
}

void Segments(node* r, int x, vector<int>& segs){
    //achou os segmentos
    for(int i = 0; i < r->segs.size(); i++)
        segs.push_back(r->segs[i]);
    //chama na esquerda
    if(r->left != nullptr){
        if(!r->left->f_open && x <= r->left->f)
            Segments(r->left, x, segs);
        else if(x < r->left->f)
            Segments(r->left, x, segs);
    }
    //chama na direita
    if(r->right != nullptr){
        if(!r->right->s_open && x >= r->right->s)
            Segments(r->right, x, segs);
        else if(x > r->right->s)
            Segments(r->right, x, segs);
    }
}

void Print(node* r){
    if(r != nullptr)
        PrintRec(r, 0);
    else
        cout << "árvore vazia\n";
}
void PrintRec(node* r, int i){
    if(r != nullptr){
        PrintRec(r->left, i+3);
        for(int j = 0; j < i; j++)
            cout << " ";
        
        if(r->s_open)           cout << "(";
        else                    cout << "[";

        if(r->s == INT_MIN)     cout << "-inf,";
        else                    cout << r->s << ",";

        if(r->f == INT_MAX)     cout << "+inf";
        else                    cout << r->f;

        if(r->f_open)           cout << ")";
        else                    cout << "]";
        
        for(int i = 0; i < r->segs.size(); i++)
            cout << " " << r->segs[i];
        
        cout << endl;
        PrintRec(r->right, i+3);
    }
}
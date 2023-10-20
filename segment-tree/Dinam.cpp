#include "Dinam.h"

nodeD* ASD(){
    nodeD* asd = (nodeD*)malloc(sizeof(nodeD));;
    asd->next = nullptr;
    asd->AS = nullptr;
    return asd;
}

nodeD* Insert(nodeD* r, vector<pair<int,int>> segs, vector<int> index){
    if(r == nullptr)
        r = ASD();

    //chegamos em um nó vazio e podemos descarregas os segmentos anteriores aqui
    if(r->index.size() == 0){
        r->AS = AS(segs, index);
        r->index = index;
        for(int i = 0; i < segs.size(); i++){
            r->s.push_back(segs[i].first);
            r->f.push_back(segs[i].second);
        }
    }
    //aqui já está cheio, portanto precisamos carregar esses segmentos para o próximo
    else{
        for(int i = 0; i < r->index.size(); i++){
            index.push_back(r->index[i]);
            segs.push_back({r->s[i], r->f[i]});
        }

        r->index.clear();
        r->s.clear();
        r->f.clear();
        r->AS = nullptr;

        r->next = Insert(r->next, segs, index);
    }
    return r;
}

vector<int> SegmentsD(nodeD* r, int x){
    vector<int> segs, aux;
    if(r != nullptr){
        if(r->AS != nullptr)
            Segments(r->AS, x, segs);
        if(r->next != nullptr){
            aux = SegmentsD(r->next, x);
            for(int i = 0; i < aux.size(); i++)
                segs.push_back(aux[i]);
        }
    }
    return segs;
}

void PrintASD(nodeD* r){
    if(r != nullptr){
        Print(r->AS);
        cout << endl;
        PrintASD(r->next);
    }
}
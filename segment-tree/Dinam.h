#ifndef _DINAM_H_
#define _DINAM_H_

#include "Estat.h"

typedef struct cellD{
    vector<int> index;              //indices dos segmentos conforme ordem de inserção
    vector<int> s;                  //lista com o início dos segmentos que estão nesta AS
    vector<int> f;                  //lista com o fim dos segmentos que estão nesta AS
    node* AS;
    struct cellD* next;
} nodeD;

//cria e devolve uma AS dinâmica vazia
nodeD* ASD();

//insere na ASD o intervalo [s,f]
nodeD* Insert(nodeD*, vector<pair<int,int>>, vector<int> index);

//devolve uma lista com os segmentos na ASD que contém x
vector<int> SegmentsD(nodeD*, int);

//aciona o Print para cada AS da ASD
void PrintASD(nodeD*);

#endif // _DINAM_H_
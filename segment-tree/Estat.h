#ifndef _ESTAT_H_
#define _ESTAT_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <limits.h>

using namespace std;

typedef struct cell{
    vector<int> segs;   //lista com os segmentos cujo intervalo do nó está
    
    int s;              //valor do início do intervalo
    int f;              //valor do final do intervalo

    bool s_open;        //se o início do intervalo é aberto ou fechado
    bool f_open;        //se o final do intervalo é aberto ou fechado

    struct cell* left;
    struct cell* right;
} node;

//Retorna um vetor ordenado com valores únicos das estremidades dos segmentos
vector<int> pre_process(vector<pair<int,int>>);

//Retorna um nó alocado
node* new_node(int, int, bool, bool, node*, node*);

//Cria uma árvore de segmentos para o conjunto de segmentos
node* AS(vector<pair<int,int>>&, vector<int>&);
//Preenche a árvore com os segmentos, em cada nó
node* Fill_AS(node*, pair<int,int>&, int);

//Devolve uma lista com os segmentos na AS que contém x
void Segments(node*, int, vector<int>&);

//Imprime a AS, com a lista de segmentos para cada nó
void Print(node*);
void PrintRec(node*, int);

#endif // _ESTAT_H_
//Parte (b) do EP feita em cerca de 2 horas
#include "Dinam.h"

int main(){
    int n, s, f, option, x;
    nodeD* r = ASD();

    n = 1;

    while(cin >> option){
        if(option == 1){
            cin >> s >> f;

            vector<pair<int,int>> segs;
            segs.push_back({s,f});
            vector<int> index;
            index.push_back(n++);

            r = Insert(r, segs, index);
        }
        else if(option == 2){
            cin >> x;

            vector<int> seg_list = SegmentsD(r, x);
            //sort(seg_list.begin(), seg_list.end());
            if(seg_list.size() > 0)
                for(int i = 0; i < seg_list.size(); i++)
                    cout << seg_list[i] << " ";
            else
                cout << "nenhum segmento";   
            cout << endl;
        }
        else{
            PrintASD(r);
            cout << endl;
        }
    }
    return 0;
}
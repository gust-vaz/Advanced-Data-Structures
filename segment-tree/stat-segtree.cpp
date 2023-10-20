#include "Estat.h"

int main(){
    vector<pair<int,int>> segs;
    vector<int> index;
    int n, s, f, option, x;
    node* r = nullptr;

    cin >> n;
    if(n > 0){
        for(int i = 0; i < n; i++){
            cin >> s >> f;
            segs.push_back({s,f});
            index.push_back(i+1);
        }
        r = AS(segs, index);
    }

    while(cin >> option){
        if(option == 1){
            cin >> x;
            if(n > 0){
                vector<int> seg_list;
                Segments(r, x, seg_list);
                //sort(seg_list.begin(), seg_list.end());
                if(seg_list.size() > 0)
                    for(int i = 0; i < seg_list.size(); i++)
                        cout << seg_list[i] << " ";
                else
                    cout << "nenhum segmento";            
            }
            else
                cout << "nenhum segmento";
            cout << endl;
        }
        else
            Print(r);
    }
    return 0;
}

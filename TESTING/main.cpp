/**
 * Program untuk tes perulangan dan fungsi sesuai dengan
 * program referensi adaptive PD
 * Created by:  H. Taufik
 * **/
#include <stdio.h>
#include <iostream>

using namespace std;

int main(){
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            if(i+j <= 2) cout << "ph";
            if(i+j == 3) cout << "pb";
            if(i+j == 4) cout << "pm";
            if(i+j == 5) cout << "ps";
            if(i+j == 6) cout << "z";
            if(i+j == 7) cout << "ns";
            if(i+j == 8) cout << "nm";
            if(i+j == 9) cout << "nb";
            if(i+j >= 10) cout << "nh";
            
            cout << ",";
        }
        cout << endl;
    }

    // fungsi max adaptive pd
    float e[7], ed[7];
    float temp[7][7];
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            temp[i][j] = max(e[i], ed[j]);
        }
    }
    // fungsi min adaptive pd
    float ph, pb, pm, ps, z, ns, nm, nb, nh;
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            if(i+j <= 2) ph = min(ph, temp[i][j]);
            if(i+j == 3) pb = min(pb, temp[i][j]);
            if(i+j == 4) pm = min(pm, temp[i][j]);
            if(i+j == 5) ps = min(ps, temp[i][j]);
            if(i+j == 6) z = min(z, temp[i][j]);
            if(i+j == 7) ns = min(ns, temp[i][j]);
            if(i+j == 8) nm = min(nm, temp[i][j]);
            if(i+j == 9) nb = min(nb, temp[i][j]);
            if(i+j >= 10) nh = min(nh, temp[i][j]);
        }
    }

    return 0;
}
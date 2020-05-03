/**
 * Program untuk testing program adaptive PID sesuai
 * dengan referensi dan data excel
 * 
 * Created by:  H. Taufik
 * **/
#include <stdio.h>
#include <iostream>

using namespace std;

int main(){
    float e[7], de[7], ie[7];
    float temp[7][7][7];
    float ph, pb, pm, ps, z, ns, nm, nb, nh;

    // mencari nilai max/and dari e, de, dan ie lalu di simpan ke temp
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            for(int k=0; k<7; k++){
                temp[i][j][k] = max(e[i], max(de[j], ie[k]));
            }
        }
    }
    // mencari nilai min/or
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            for(int k=0; k<7; k++){
                #if defined(mode1)
                if(i+j <= 2) ph = min(ph, temp[i][j][k]);
                if(i+j == 3 || i+j == 4) pb = min(pb, temp[i][j][k]);
                if(i+j == 5 || i+j == 6) pm = min(pm, temp[i][j][k]);
                if(i+j == 7 || i+j == 8) ps = min(ps, temp[i][j][k]);
                if(i+j == 9) z = min(z, temp[i][j][k]);
                if(i+j == 10 || i+j == 11) ns = min(ns, temp[i][j][k]);
                if(i+j == 12 || i+j == 13) nm = min(nm, temp[i][j][k]);
                if(i+j == 14 || i+j == 15) nb = min(nb, temp[i][j][k]);
                if(i+j >= 13) nh = min(nh, temp[i][j][k]);
                #endif
                #if defined(mode2)
                if(i+j <= 2) ph = min(ph, temp[i][j][k]);
                if(i+j >=3 && i+j <= 5) pb = min(pb, temp[i][j][k]);
                if(i+j == 6 || i+j == 7) pm = min(pm, temp[i][j][k]);
                if(i+j == 8) ps = min(ps, temp[i][j][k]);
                if(i+j == 9) z = min(z, temp[i][j][k]);
                if(i+j == 10) ns = min(ns, temp[i][j][k]);
                if(i+j == 11 || i+j == 12) nm = min(nm, temp[i][j][k]);
                if(i+j >= 13 && i+j <= 15) nb = min(nb, temp[i][j][k]);
                if(i+j >= 16) nh = min(nh, temp[i][j][k]);
                #endif
                #if defined(mode3)
                if(i+j <= 3) ph = min(ph, temp[i][j][k]);
                if(i+j == 4 || i+j == 5) pb = min(pb, temp[i][j][k]);
                if(i+j == 6 || i+j == 7) pm = min(pm, temp[i][j][k]);
                if(i+j == 8) ps = min(ps, temp[i][j][k]);
                if(i+j == 9) z = min(z, temp[i][j][k]);
                if(i+j == 10) ns = min(ns, temp[i][j][k]);
                if(i+j == 11 || i+j == 12) nm = min(nm, temp[i][j][k]);
                if(i+j == 13 || i+j == 14) nb = min(nb, temp[i][j][k]);
                if(i+j >= 15) nh = min(nh, temp[i][j][k]);
                #endif
                #if defined(mode4)
                if(i+j <= 5) ph = min(ph, temp[i][j][k]);
                if(i+j == 6) pb = min(pb, temp[i][j][k]);
                if(i+j == 7) pm = min(pm, temp[i][j][k]);
                if(i+j == 8) ps = min(ps, temp[i][j][k]);
                if(i+j == 9) z = min(z, temp[i][j][k]);
                if(i+j == 10) ns = min(ns, temp[i][j][k]);
                if(i+j == 11) nm = min(nm, temp[i][j][k]);
                if(i+j == 12) nb = min(nb, temp[i][j][k]);
                if(i+j >= 13) nh = min(nh, temp[i][j][k]);
                #endif
            }
        }
    }
    return 0;
}
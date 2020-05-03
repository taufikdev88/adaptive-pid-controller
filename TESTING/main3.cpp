/**
 * Program untuk tes fungsi cpp
 * 
 * **/
#include <stdio.h>
#include <iostream>

#define mode2

using namespace std;

int main(){
    #if defined(mode1)
        cout << "mode1";
    #endif
    #if defined(mode2)
        cout << "mode2";
    #endif
    return 0;
}
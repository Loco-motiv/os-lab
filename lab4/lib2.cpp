#include "libs.h"
float e(int x){
    int fact = 1;
    int ans = 0;
    for (int i = 1; i <= x; i++){
        fact *= i;
        ans += 1/fact;
    }
    return ans;
}

float square(float a, float b){
    return (a*b)/2;
}
#include "hash.hpp"

void linspace(float start, float stop, float step, std::vector<float>& arr)
{   
    while(start < stop)
    {
        arr.push_back(start);
        start += step;   
    }
}

int main(){

    printf("%d %d %f\n", UP, LOW, step);
    
    std::vector<float> x,y;

    x.push_back(5);

    // linspace(UP,LOW,step,x);
    // linspace(UP,LOW,step,y);

    for(int i=0; i<x.size();i++){
        printf("%f\n",x[i]);
    }


}
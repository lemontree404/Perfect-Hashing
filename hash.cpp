#include "hash.hpp"
#include <time.h>

int main(){

    init_global_vars();

    printf("Global Vars:\na: %d\nb: %d\nEND\n",a1,b1);

    fill_hash_table();

    // print_hash_table();

    fetch_keys();

    for(int i = 0; i<SIZE;i++){
        printf("(%f,%f)\n",(hash_table[50] -> arr)[i] -> x,(hash_table[50] -> arr)[i] -> y);
    }
}
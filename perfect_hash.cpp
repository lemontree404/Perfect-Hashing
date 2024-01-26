#include "perfect_hash.hpp"
#include <omp.h>

int main(){

    double start = omp_get_wtime();

    int sum = 0;

    do{

        sum = 0;

        srand(time(NULL));
        init_global_vars();

        printf("Global Vars:\na: %d\nb: %d\n",a1,b1);

        fill_hash_table();

        for(int i = 0; i < M; i++){
            
            int n = find_bin_length(i);
            sum += n*n;
        }

        printf("Sum Squared: %d\nN: %d\nEND\n", sum, N);
    }while(sum>1000*N*N);

    // print_hash_table();

    init_perfect_hash();

    fill_perfect_hash();

    fetch_keys_perfect();

    double duration = omp_get_wtime() - start;

    printf("Duration: %lf\n", duration);

    int x = 99;
    int y = 97;

    int hash1 = hash(x,y);

    int n = find_bin_length(hash1);

    int hash2 = perfect_hash(x, y, a2[hash1], b2[hash1], c2[hash1], P, n*n*n);

    if(perfect_hash_table[hash1][hash2] -> x == x && perfect_hash_table[hash1][hash2] -> y == y){
        printf("(%d,%d)\n",(perfect_hash_table[hash1][hash2] -> arr)[0] -> x,(perfect_hash_table[hash1][hash2] -> arr)[0] -> y);
    }
    else return 0;
}
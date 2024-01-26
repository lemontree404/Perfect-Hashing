#include "perfect_hash.hpp"

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

    fill_perfect_hash_parallel();

    fetch_keys_perfect();

    double duration = omp_get_wtime() - start;

    printf("Duration: %lf\n", duration);
    return 0;

}
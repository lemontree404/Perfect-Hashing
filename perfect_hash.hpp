#include "hash.hpp"
#include <omp.h>

int a2[M];
int b2[M];
int c2[M];

struct trajectory** perfect_hash_table[M];

void init_perfect_global_vars(){
    
    for(int i = 0; i < M; i++){

        a2[i] = get_rand_num(1,P-1);
        b2[i] = get_rand_num(1,P-1);
        c2[i] = get_rand_num(0,P-1);
    }
}

void init_perfect_hash(){
    
    for(int i = 0; i < M; i++){
        
        int n = find_bin_length(i);
        perfect_hash_table[i] = (struct trajectory **) malloc(sizeof(struct trajectory*)*n*n*n);
    }
}

int perfect_hash(int x, int y, int a, int b, int c, int p, int m){

    return (((a * x + c) + b * y)  % p) % m;
}

void fill_perfect_hash(){

    for(int i = 0; i < M; i++){
        
        printf("Perfect Hashing Index %d", i);

        int n = find_bin_length(i);

        struct trajectory* temp = hash_table[i];
        
        while(temp != NULL){

            if(perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] == NULL){

                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] = (struct trajectory *) malloc(sizeof(struct trajectory));
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> x = temp -> x;
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> y = temp -> y;
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> arr = (iter**) malloc(sizeof(temp -> arr));
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> arr = temp -> arr;
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> next = NULL;

                temp = temp -> next;
            }
            else{

                a2[i] = get_rand_num(1,P-1);
                b2[i] = get_rand_num(1,P-1);
                c2[i] = get_rand_num(0,P-1);
                temp = hash_table[i];
                free(perfect_hash_table[i]);
                perfect_hash_table[i] = (struct trajectory **) malloc(sizeof(struct trajectory*)*n*n*n);

            }
        }

        printf("\tDONE\n");
    }
}

int fetch_perfect(int x,int y){
    
    int hash1 = hash(x,y);

    int n = find_bin_length(hash1);

    int hash2 = perfect_hash(x, y, a2[hash1], b2[hash1], c2[hash1], P, n*n*n);

    if(perfect_hash_table[hash1][hash2] -> x == x && perfect_hash_table[hash1][hash2] -> y == y){
        return 1;
    }
    else return 0;
}

void fetch_keys_perfect(){

    int sum=0;
    double time_sum=0;

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {   
            printf("(%d,%d): ",i,j);

            struct timeval start, end; 
            gettimeofday(&start, NULL);

            sum += fetch_perfect(i,j);
            
            gettimeofday(&end, NULL);
            double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
            printf("\tTime taken: %f seconds\n", time_taken);

            time_sum += time_taken;
        } 
    }

    printf("%d out of %d keys found\n", sum, N*N);
    printf("Mean Fetch Time : %lf\nEND\n", time_sum/(N*N));
}

void fill_perfect_hash_parallel(){
    
    omp_set_num_threads(M);

    #pragma omp parallel
    {

        int i = omp_get_thread_num();

        int n = find_bin_length(i);

        struct trajectory* temp = hash_table[i];
        
        while(temp != NULL){

            if(perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] == NULL){

                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] = (struct trajectory *) malloc(sizeof(struct trajectory));
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> x = temp -> x;
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> y = temp -> y;
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> arr = temp -> arr;
                perfect_hash_table[i][perfect_hash(temp -> x, temp -> y, a2[i], b2[i], c2[i], P, n*n*n)] -> next = NULL;

                temp = temp -> next;
            }
            else{

                a2[i] = get_rand_num(1,P-1);
                b2[i] = get_rand_num(1,P-1);
                c2[i] = get_rand_num(0,P-1);
                temp = hash_table[i];
                free(perfect_hash_table[i]);
                perfect_hash_table[i] = (struct trajectory **) malloc(sizeof(struct trajectory*)*n*n*n);

            }
        }

        #pragma omp critical
        {
            printf("Perfect Hashing Index %d", i);
            printf("\tDONE\n");
        }
    }
}
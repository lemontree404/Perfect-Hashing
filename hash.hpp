// #define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <sys/time.h>
#define P 13145 // Middle Prime Dimension
#define M 100 // Hash Table Sizee
#define N 100 // Single Axis - N**2 number of keys
#define SIZE 100
#define UP 1
#define LOW -1

// Global Hash Variables
int a1;
int b1;
int c1;

// Step size:
float step = (float)(UP-LOW)/N;

// One Iteration Values
struct iter{
    float x,y;
};


// Object Stored in Hash Map Slot
// Contains:
// 1) Initial (x,y)
// 2) Array of next 100 iterations' (x,y)
// 3) Pointer to next trajectory object
struct trajectory{
    int i,j;
    float x,y;
    struct iter** arr;
    struct trajectory* next;
};


// Hash Table (array of trajectory objects)
struct trajectory** hash_table;


// Random Number Generator:
// Provides random number between lower and upper limit.
float get_rand_num(float low_lim,float up_lim)
{
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(low_lim, up_lim);
    return (float)dis(gen);
}


// Initialize Hash Table:
// Initializes all slots in Hash Table to NULL
void init_hash_table()
{
    
    for(int i = 0; i < M; i++){

        hash_table[i] = NULL;
    }
}


// Initialize Global Vars:
// Initializes global hash variables and step and then
// clears hash table ,allots new memory to it
// and then initializes hash table.
void init_global_vars()
{
    
    a1 = get_rand_num(1,P-1);
    b1 = get_rand_num(1,P-1);
    c1 = get_rand_num(0,P-1);

    free(hash_table);
    hash_table = (struct trajectory**) malloc(M * sizeof(struct trajectory*));
    init_hash_table();
}


// Hash Function:
// Returns index given x,y
int hash(int x, int y)
{
    return (((a1 * x + c1) + b1 * y) % P) % M;
}



void fill_trajectory(struct trajectory* t){

    t -> arr = (struct iter**) malloc(sizeof(struct iter*)*SIZE);

    float x = t -> x;
    float y = t -> y;
    
    float x_, y_;

    for(int i = 0; i < SIZE; i++)
    {   
        (t -> arr)[i] = (struct iter*) malloc(sizeof(struct iter));
        (t -> arr)[i] -> x = x;
        (t -> arr)[i] -> y = y;

        x_ = 1 - (1.4 * x * x) + y;
        y_ = 0.3 * x;

        x = x_;
        y = y_;
    }    

}

void fill_hash_table(){

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            int index = hash(i,j);

            if(hash_table[index]==NULL)
            {
                hash_table[index] = (struct trajectory*) malloc(sizeof(struct trajectory));
                hash_table[index] -> i = i;
                hash_table[index] -> j = j;
                hash_table[index] -> x = (float)(step * i) + LOW;
                hash_table[index] -> y = (float)(step * j) + LOW;
                fill_trajectory(hash_table[index]);
                hash_table[index] -> next = NULL;
            }
            else{

                struct trajectory* temp = hash_table[index];

                while(temp->next != NULL){
                    temp = temp -> next;
                }

                (temp -> next) = (struct trajectory*) malloc(sizeof(struct trajectory));
                (temp -> next) -> i = i;
                (temp -> next) -> j = j;
                (temp -> next) -> x = step * i + LOW;
                (temp -> next) -> y = step * j + LOW;
                fill_trajectory((temp -> next));
                (temp -> next) -> next = NULL;
        }
        }
    }
}

void print_hash_table(){

    printf("Hash Table:\n");

    for(int i = 0; i < M; i++){
        printf("Index %d:", i);

        if(hash_table[i] == NULL){

            printf("\n");
            continue;
        }
        else{
            struct trajectory* temp;
            temp = hash_table[i];

            while(temp != NULL){
                printf("\t(%d,%d)", temp -> i, temp -> j);
                temp = temp -> next;
            }

            printf("\n");
        }
    }

    printf("END\n");
}

int fetch(int x,int y){
    
    int index = hash(x,y);

    struct trajectory* temp = hash_table[index];

    while(temp != NULL){
        if(temp -> i == x && temp -> j == y){
            
            printf("Key Found");
            return 1;
        }

        temp = temp -> next;
    } 

    printf("Key not Found");
    return 0;
}

void fetch_keys(){

    printf("Key Fetches:\n");

    int sum = 0;
    double time_sum = 0;

    for (int i = 0; i < N; i++)
    {   
        for(int j = 0; j < N; j++)
        {
            printf("(%d,%d): ",i,j);

            struct timeval start, end;
            gettimeofday(&start, NULL);

            sum += fetch(i,j);
            
            gettimeofday(&end, NULL);
            double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
            printf("\tTime taken: %f seconds\r", time_taken);

            time_sum += time_taken; 
        }  
    }

    printf("%d out of %d keys found                                         \n", sum, N*N);
    printf("Mean Fetch Time : %lf\nEND\n", time_sum/N);
}

int find_bin_length(int index){
    
    int len = 0;

    struct trajectory* temp = hash_table[index];

    if(temp == NULL){
        return 0;
    }
    else{
        while(temp != NULL){
            
            len++;
            temp = temp -> next;
        }

        return len;
    }
}
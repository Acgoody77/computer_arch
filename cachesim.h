/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 */

#define BLOCK_SIZE  64        /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
#define WAY_SIZE    16         /* Associativity; 1-way = direct-mapped */
#define CACHE_SIZE  32768     /* Cache capacity in bytes (must be power of 2)*/
//32768 16384 65536 131072
#define NUM_BLOCKS    (CACHE_SIZE / BLOCK_SIZE)
#define NUM_SETS      (NUM_BLOCKS / WAY_SIZE)

//#define DBG

/*The data structure of direct-mapped cache*/
struct direct_mapped_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    double hit_rate;                    /*Hit rate*/
    double miss_rate;                   /*Miss rate*/
};


struct fully_associative_cache {

    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    double hit_rate;                    /*Hit rate*/
    double miss_rate;                   /*Miss rate*/
};


struct nway_cache {
  
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    double hit_rate;                    /*Hit rate*/
    double miss_rate;                   /*Miss rate*/
};

/*PLRU tree integer initilization*/
uint64_t tree[WAY_SIZE*2-1] = {0};

/*Read the memory traces and convert it to binary*/
uint64_t convert_address(char memory[]);

/*Simulate the direct-mapped cache*/
void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address);
void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address);
void nway_cache_access(struct nway_cache *cache, uint64_t address);

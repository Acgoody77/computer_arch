/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "cachesim.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s (<direct>,<fully>,<nway>) <trace file name>\n", argv[0]);
        return 1;
    }

#ifdef DBG
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("\n");
#endif

    char* trace_file_name = argv[2];
    struct direct_mapped_cache d_cache;
    struct fully_associative_cache a_cache;
    struct nway_cache n_cache;
    char mem_request[20];
    uint64_t address;
    FILE *fp;


    /* Initialization */
    for (int i=0; i<NUM_BLOCKS; i++) {
        d_cache.valid_field[i] = 0;
        d_cache.dirty_field[i] = 0;
        d_cache.tag_field[i] = 0;
        //TODO: add a_cache initilization
    }
    d_cache.hits = 0;
    d_cache.misses = 0;
    d_cache.hit_rate = 0;
    d_cache.miss_rate = 0;

    /* Opening the memory trace file */
    fp = fopen(trace_file_name, "r");

    if (strncmp(argv[1], "direct", 6)==0) { /* Simulating direct-mapped cache */
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            direct_mapped_cache_access(&d_cache, address);
        }
        /*calculate hit/miss rates*/
        d_cache.hit_rate = ((double)d_cache.hits/(d_cache.hits + d_cache.misses));
        d_cache.miss_rate = ((double)d_cache.misses/(d_cache.misses + d_cache.hits));

        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    Direct-Mapped Cache\n");
        printf("==================================\n");
        printf("Cache Hits:      %d\n", d_cache.hits);
        printf("Cache Misses:    %d\n", d_cache.misses);
        printf("Cache Hit Rate:  %f\n", d_cache.hit_rate);
        printf("Cache Miss Rate: %f\n", d_cache.miss_rate);
        printf("\n");
    }else if(strncmp(argv[1], "fully", 5)==0){/* Fully-Associative cache */
      while (fgets(mem_request, 20, fp)!= NULL){
        address = convert_address(mem_request);
        fully_associative_cache_access(&a_cache, address);
      }
      //TODO: finish fully logic  

        /*calculate hit/miss rates*/
        a_cache.hit_rate = ((double)a_cache.hits/(a_cache.hits + a_cache.misses));
        a_cache.miss_rate = ((double)a_cache.misses/(a_cache.misses + a_cache.hits));
        
        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type: Fully Associative Cache\n");
        printf("==================================\n");
        printf("Cache Hits:      %d\n", a_cache.hits);
        printf("Cache Misses:    %d\n", a_cache.misses);
        printf("Cache Hit Rate:  %f\n", a_cache.hit_rate);
        printf("Cache Miss Rate: %f\n", a_cache.miss_rate);
        printf("\n");
    }else if(strncmp(argv[1], "nway", 5)==0){
      while (fgets(mem_request, 20, fp)!=NULL){
        address = convert_address(mem_request);
        nway_cache_access(&n_cache, address);
      }
      //TODO: finish set logic
   
        /*calculate hit/miss rates*/
        n_cache.hit_rate = ((double)n_cache.hits/(n_cache.hits + n_cache.misses));
        n_cache.miss_rate = ((double)n_cache.misses/(n_cache.misses + n_cache.hits));
        
        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type: N-Way Associative Cache\n");
        printf("==================================\n");
        printf("Cache Hits:      %d\n", n_cache.hits);
        printf("Cache Misses:    %d\n", n_cache.misses);
        printf("Cache Hit Rate:  %f\n", n_cache.hit_rate);
        printf("Cache Miss Rate: %f\n", n_cache.miss_rate);
        printf("\n");
    }


    fclose(fp);

    return 0;
}

uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}

void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

    if (cache->valid_field[index] && cache->tag_field[index] == tag) { /* Cache hit */
        cache->hits += 1;
#ifdef DBG
        printf("Hit!\n");
#endif
    } else {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }
        cache->tag_field[index] = tag;
        cache->valid_field[index] = 1;
        cache->dirty_field[index] = 0;
    }
}

void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address){
  
  uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE); 
  uint64_t index = block_addr % NUM_BLOCKS;
  uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);
  int hit = 0;
  int write = 0;
#ifdef DBG
  printf("Memory address: %llu, Block addres: %llu, Index: %llu, Tag: %llu", address, block_addr, index, tag);
#endif


  for(uint64_t i = index; i < NUM_BLOCKS; i += NUM_BLOCKS){
    if(cache->valid_field[index] && cache->tag_field[index] == tag) {
      cache->hits += 1;

#ifdef DBG
  printf("Hit!\n");
#endif
      hit = 1;
      break;
    }
  } 
  
  if(hit == 0){
      //means its a miss
      cache->misses += 1;
#ifdef DBG
  printf("Miss!\n");
#endif
    /*NRU replacement algorithm*/
    for(uint64_t i = index; i < NUM_BLOCKS; i += NUM_BLOCKS){
      if(!cache->valid_field[i]){
        cache->tag_field[i] = tag;
        cache->valid_field[i] = 1;
        write = 1;
        break;
       }
    }
    if(write == 0){
      for(uint64_t i = index; i < NUM_BLOCKS; i += NUM_BLOCKS){
        cache->valid_field[i] = 0;
      }
      cache->tag_field[0] = tag;
      cache->valid_field[0] = 1;
    }
   /*End NRU replacement alg*/ 
  }
}

void nway_cache_access(struct nway_cache *cache, uint64_t address){
  uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE); 
  uint64_t index = block_addr % NUM_SETS;
  uint64_t tag = block_addr >> (unsigned)log2(NUM_SETS);
  int hit = 0;
  int write = 0;

#ifdef DBG
  printf("Memory address: %llu, Block addres: %llu, Index: %llu, Tag: %llu", address, block_addr, index, tag);
#endif


  for(uint64_t i = index; i < NUM_BLOCKS; i += NUM_SETS){
    if(cache->valid_field[index] && cache->tag_field[index] == tag) {
      cache->hits += 1;

#ifdef DBG
  printf("Hit!\n");
#endif
      hit = 1;
      break;
    }
  } 
  
  if(hit == 0){
      //means its a miss
      cache->misses += 1;
#ifdef DBG
  printf("Miss!\n");
#endif
    /*NRU replacement algorithm*/
    for(uint64_t i = index; i < NUM_BLOCKS; i += NUM_SETS){
      if(!cache->valid_field[i]){
        cache->tag_field[i] = tag;
        cache->valid_field[i] = 1;
        write = 1;
        break;
       }
    }
    if(write == 0){
      for(uint64_t i = index; i < NUM_BLOCKS; i += NUM_SETS){
        cache->valid_field[i] = 0;
      }
      cache->tag_field[0] = tag;
      cache->valid_field[0] = 1;
    }
   /*End NRU replacement alg*/ 
  }
}

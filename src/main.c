#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
typedef struct{
    char *key;
    void *data;
} hashmap_entry_t;

typedef struct{
    hashmap_entry_t *entries;
    int collisions;
} hashmap_table_t;

typedef struct{
    hashmap_table_t *table;
    size_t size;
    size_t max_collisions;

} hashmap_t;

hashmap_t *hashmap_create(uint64_t size, size_t max_collisions){
    hashmap_t *map = (hashmap_t*)malloc(sizeof(hashmap_t));
    map->table = (hashmap_table_t*)malloc(sizeof(map->table) * size);
    map->size = size;
    map->max_collisions = max_collisions;
    memset(map->table, 0, sizeof(map->table) * size);
    return map;
}

uint64_t hashmap_hash(char *key, size_t size){
    uint64_t hash_value = 0;
    while(*key){
        hash_value = (hash_value << 5) + *key;
        *key++;
    }

    return hash_value % (size - 1);
}

void hashmap_insert(char *key, void *data, hashmap_t *hashmap){
    uint64_t hash = hashmap_hash(key, hashmap->size);
    hashmap_table_t *table = &hashmap->table[hash];
    if(table->entries == NULL || table->collisions == 0){
        table->entries = (hashmap_entry_t*)malloc(sizeof(table->entries) * hashmap->max_collisions);
        memset(table->entries, 0, sizeof(table->entries) * hashmap->max_collisions);
        table->collisions = 0;
    }

    uint64_t index = table->collisions++;
    table->entries[index].key = (char*)malloc(strlen(key));
    strcpy(table->entries[index].key, key);
    table->entries[index].data = data; 
}

void *hashmap_search(char *key, hashmap_t *hashmap){
    uint64_t index = hashmap_hash(key, hashmap->size);
    hashmap_table_t *table = &hashmap->table[index];
    if(table->entries == NULL || table->collisions==0){
        return NULL;
    }

    for(int i=0;i<table->collisions; i++){
        if(!strcmp(key, table->entries[i].key)){
            return table->entries[i].data;
        }
    }
}

hashmap_t *hashmap_resize(hashmap_t *hashmap, size_t new_size, size_t new_collisions){

}

int main(void){
    hashmap_t *hashmap = hashmap_create(30, 5);
    hashmap_insert("teste1", "123", hashmap);
    hashmap_insert("teste2", "1234", hashmap);
    hashmap_insert("teste3", "12345", hashmap);
    hashmap_insert("teste4", "123456", hashmap);
    hashmap_insert("teste5", "1234567", hashmap);
    hashmap_insert("teste6", "12345678", hashmap);
    
    printf("%s", (char*)hashmap_search("teste1", hashmap));

    return 0;
}
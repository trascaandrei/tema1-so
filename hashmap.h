#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    char *key;
    char *val;
    struct node *next;
} node;

typedef struct hashmap {
    int size;
    node **map;
} hashmap;


unsigned long crc32(const unsigned char *s, unsigned int len);
hashmap *map_create(int size);
int hash_code(hashmap *m, char* keystring);
char* map_find(hashmap *map, char *key);
void map_delete(hashmap *map, char *key);
void map_insert(hashmap *map, char *key, char *val);
void map_free(hashmap *map);
void map_print(hashmap *map);

#endif

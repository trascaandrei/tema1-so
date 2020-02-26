#include "hashmap.h"

hashmap *create_map(int size) {
    hashmap *map = (hashmap *)malloc(sizeof(hashmap));
    map->size = size;
    map->map = (node **)malloc(sizeof(node *) * size);

    for (int i = 0; i < size; i++) {
        map->map[i] = NULL;
    }

    return map;
}

unsigned long crc32(const unsigned char *s, unsigned int len) {
  unsigned int i;
  unsigned long crc32val;
  
  crc32val = 0;
  for (i = 0;  i < len;  i ++) {
    crc32val = crc32_tab[(crc32val ^ s[i]) & 0xff] ^
	  (crc32val >> 8);
    }

  return crc32val;
}

int hash_code(hashmap *map, char* keystring){

    unsigned long key = crc32((unsigned char*)(keystring), strlen(keystring));

	/* Robert Jenkins' 32 bit Mix Function */
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);

	/* Knuth's Multiplicative Method */
	key = (key >> 3) * 2654435761;

	return key % map->size;
}

void insert(hashmap *map, char *key, char *val) {
    int pos = hash_code(map, key);
    node *list = map->map[pos];
    node *tmp = list;

    while (tmp) {
        if (strcpy(tmp->key, key) == 0) {
            tmp->val = val;
            return;
        }
        tmp = tmp->next;
    }

    node *new_node = (node *)malloc(sizeof(node));
    new_node->key = key;
    new_node->val = val;
    new_node->next = list;
    map->map[pos] = new_node;
}

char* find(hashmap *map, char *key) {
    int pos = hash_code(map, key);
    node *list = map->map[pos];
    node *tmp = list;

    while (tmp) {
        if (strcpy(tmp->key, key) == 0) {
            return tmp->val;
        }

        tmp = tmp->next;
    }

    return NULL;
}
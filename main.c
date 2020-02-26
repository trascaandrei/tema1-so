#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"
#include "helpers.h"

int main(int argc, char *argv[]) {
    hashmap *map = map_create(100);
    map_insert(map, "hello", "world");
    map_insert(map, "hello", "map");
    printf("%s", map_find(map, "hello"));
    map_free(map);
    return 0;
}

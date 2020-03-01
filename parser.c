#include "parser.h"

char *parse_line(char *line, int *multiline_define, int *if_started,
                  hashmap *map) {
  if (strncmp(line, DEFINE, 7) == 0) {
    char *key;
    char *value;

    strtok(line, " ");
    key = strtok(NULL, " ");
    value = strtok(NULL, " ");

    map_insert(map, key, value);
  }
}

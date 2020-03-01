#include "parser.h"

char *parse_non_preprocessor(char *line, hashmap *map) {
  //const char *sep = " "
  char *token = strtok(line, " ");
  char buffer[1024];
  int string_started = 0;

  while (token != NULL) {
    if (token[0] == '\"') {
      string_started = 1 - string_started;
    }

    char *find_res = map_find(map, token);
    if (!string_started && find_res != NULL) {
      strcat(buffer, find_res);
    } else {
      strcat(buffer, token);
    }

    token = strtok(NULL, " ");
  }
}

char *parse_line(char *line, int *multiline_define, int *if_started,
                  hashmap *map) {

  if (line[0] != '#') {
    parse_non_preprocessor(line, map);
  }

  if (strncmp(line, DEFINE, 7) == 0) {
    char *key;
    char *value;

    strtok(line, " ");
    key = strtok(NULL, " ");
    value = strtok(NULL, " ");

    if (value[strlen(value) - 1] == '\') {
      multiline_define = 1;
    } else {
      map_insert(map, key, value);
    }
  }
}

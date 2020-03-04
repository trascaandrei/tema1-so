#ifndef __PARSER_H__
#define __PARSER_H__

#include "hashmap.h"

typedef struct multi_define {
  int multiline_define;
  char *key;
} multi_define;

void parse_line(char *line, multi_define *is_on, int *if_started,
                  hashmap *map);

void parse_define(char *line, multi_define *is_on, hashmap *map);

char *parse_non_preprocessor(char *line, hashmap *map);

#endif

#ifndef __PARSER_H__
#define __PARSER_H__

#include "hashmap.h"

const char *DEFINE = "#define";
const char *IF = "#if";
const char *ELIF = "#elif";
const char *ELSE = "#else";
const char *ENDIF = "#endif";
const char *IFDEF = "#ifdef";
const char *IFNDEF = "#ifndef";
const char *INCLUDE = "include";

char *parse_line(char *line, int *multiline_define, int *if_started,
                  hashmap *map);

char *parse_non_preprocessor(char *line, hashmap *map);

#endif

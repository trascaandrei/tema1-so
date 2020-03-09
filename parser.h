/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __PARSER_H__
#define __PARSER_H__

#include "hashmap.h"

typedef struct multi_define {
	int multiline_define;
	char *key;
} multi_define;

typedef struct if_struct {
	int if_started;
	int ifdef_started;
	int skip_lines;
} if_struct;

char *parse_line(char *line, multi_define *is_on, if_struct *is,
	hashmap *map);

void parse_define(char *line, multi_define *is_on, hashmap *map);

char *parse_non_preprocessor(char *line, hashmap *map);

#endif

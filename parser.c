/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "parser.h"

const char *DEFINE_S = "#define";
const char *UNDEF_S = "#undef";
const char *IF_S = "#if";
const char *ELIF_S = "#elif";
const char *ELSE_S = "#else";
const char *ENDIF_S = "#endif";
const char *IFDEF_S = "#ifdef";
const char *IFNDEF_S = "#ifndef";
const char *INCLUDE_S = "include";

int is_separator(char c) {
	char *sep = "\t[]{}<>=+-*/%!&|^.,:;()\\ ";
	if (strchr(sep, c) == NULL) {
		return 0;
	} else {
		return 1;
	}
}

char *parse_non_preprocessor(char *line, hashmap *map) {
	char *copy = strdup(line);
	static char buffer[1024];
	int n = 0;
	int string_started = 0;
	int word_started = 1;
	char new_word[100];
	int new_word_n = 0;

	while ((*copy) != '\n') {
		if ((*copy) == '\"') {
			buffer[n++] = (*copy);
			copy++;
			string_started = 1 - string_started;
		} else if (string_started) {
			buffer[n++] = (*copy);
			copy++;
		} else if (is_separator(*copy)) {
			if (word_started == 1) {
				new_word[new_word_n] = '\0';
				char *find_res = map_find(map, new_word);

				if (find_res == NULL) {
					for (int i = 0; i < new_word_n; i++) {
						buffer[n++] = new_word[i];
					}
				} else {
					for (int i = 0; i < strlen(find_res); i++) {
						buffer[n++] = find_res[i];
					}
				}

				word_started = 0;
				new_word_n = 0;
			}
			buffer[n++] = (*copy);
			copy++;
		} else {
			word_started = 1;
			new_word[new_word_n++] = (*copy);
			copy++;
		}
	}


	buffer[n++] = '\n';
	buffer[n] = '\0';
	return buffer;
}

void parse_define(char *line, multi_define *is_on, hashmap *map) {
	if (is_on->multiline_define == 1) {
		char add_new_line[1024];
		strcpy(add_new_line, map_find(map, is_on->key));
		while((*line) == ' ') {
			line++;
		}
		char *copy = strdup(line);
		char *value;
		if (strchr(line, '\\') == NULL) {
			is_on->multiline_define = 0;
			value = strtok(copy, "\n");
		} else {
			value = strtok(copy, "\\");
		}
		strcat(add_new_line, value);
		map_insert(map, is_on->key, add_new_line);
	} else if (strstr(line, DEFINE_S) != NULL) {
		char *copy = strdup(line);
		char *key;
		char *value;

		strtok(copy, " ");
		key = strtok(NULL, " ");
		value = strtok(NULL, "\r\n");

		if (value[strlen(value) - 1] == '\\') {
			// eliminate \ character
			value[strlen(value) - 1] = '\0';

			map_insert(map, key, value);
			is_on->multiline_define = 1;
			is_on->key = strdup(key);
		} else {
			map_insert(map, key, value);
		}
	}
}

void parse_undefine(char *line, hashmap *map) {
	char *copy = strdup(line);
	char *key;
	strtok(copy, " \t");
	key = strtok(NULL, " \r\n");

	map_delete(map, key);
}

char *parse_line(char *line, multi_define *is_on, int *if_started,
                 hashmap *map) {

	char *copy = strdup(line);

	if ((is_on->multiline_define == 1) || strstr(line, DEFINE_S) != NULL) {
		parse_define(line, is_on, map);
	} else if (strstr(line, UNDEF_S) != NULL) {
		parse_undefine(line, map);
	} else {
		//printf("%s", parse_non_preprocessor(line, map));
		char *buffer = strdup(parse_non_preprocessor(line, map));
		return buffer;
	}


}

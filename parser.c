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
	char *free_copy = copy;
	static char buffer[256] = {0};
	int n = 0;
	int string_started = 0;
	int word_started = 1;
	char new_word[100] = {0};
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

	free(free_copy);

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
	}

	buffer[n++] = '\n';
	buffer[n] = '\0';
	return buffer;
}

void parse_define(char *line, multi_define *is_on, hashmap *map) {
	if (is_on->multiline_define == 1) {
		char add_new_line[256] = "";
		strcpy(add_new_line, map_find(map, is_on->key));
		while((*line) == ' ') {
			line++;
		}
		char copy[256] = {0};
		strcpy(copy, line);
		char *value = NULL;
		if (strchr(line, '\\') == NULL) {
			is_on->multiline_define = 0;
			value = strtok(copy, "\n");
		} else {
			value = strtok(copy, "\\");
		}
		strcat(add_new_line, value);
		map_insert(map, is_on->key, add_new_line);
	} else if (strstr(line, DEFINE_S) != NULL) {
		char copy[256] = {0}; 
		strcpy(copy, line);
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
			if (is_on->key) {
				free(is_on->key);
			}
			is_on->key = strdup(key);
		} else {
			map_insert(map, key, value);
		}
	}
}

void parse_undefine(char *line, hashmap *map) {
	char *key;
	strtok(line, " \t");
	key = strtok(NULL, " \r\n");

	map_delete(map, key);
}

void parse_if(char *line, if_struct *is, hashmap *map) {
	is->if_started = 1;
	char val[100] = {0};
	if (strstr(line, IF_S)) {
		strcpy(val, strstr(line, IF_S) + 4);
	} else {
		strcpy(val, strstr(line, ELIF_S) + 6);
	}
	int len = strlen(val);
	while(strchr("\r\n\t ",val[len - 1])) {
		val[len - 1] = '\0';
		len--;
	}

	char *find_res = map_find(map, val);
	if (find_res != NULL) {
		if (*find_res == '0') {
			is->skip_lines = 1;
		}
	} else {
		if (val[0] == '0') {
			is->skip_lines = 1;
		}
	}
}

void parse_ifdef(char *line, if_struct *is, hashmap *map) {
	is->ifdef_started = 1;
	char val[100] = {0};
	if (strstr(line, IFDEF_S)) {
		strcpy(val, strstr(line, IFDEF_S) + 7);
	} else {
		strcpy(val, strstr(line, ELIF_S) + 6);
	}
	int len = strlen(val);
	while(strchr("\r\n\t ",val[len - 1])) {
		val[len - 1] = '\0';
		len--;
	}

	char *find_res = map_find(map, val);
	if (find_res == NULL) {
		is->skip_lines = 1;

	}
}

void parse_ifndef(char *line, if_struct *is, hashmap *map) {
	is->ifdef_started = 1;
	char val[100] = {0};
	if (strstr(line, IFNDEF_S)) {
		strcpy(val, strstr(line, IFNDEF_S) + 8);
	}
	int len = strlen(val);
	while(strchr("\r\n\t ",val[len - 1])) {
		val[len - 1] = '\0';
		len--;
	}

	char *find_res = map_find(map, val);
	if (find_res != NULL) {
		is->skip_lines = 1;
	}
}

char *parse_line(char *line, multi_define *is_on, if_struct *is,
                 hashmap *map) {

	// char *copy = strdup(line);
	if (strcmp(line, "\r\n") == 0) {
		return NULL;
	}

	if (strstr(line, "printf")) {
		if (is->skip_lines) {
			return NULL;
		}
		return parse_non_preprocessor(line, map);
	}

	if ((is_on->multiline_define == 1) || strstr(line, DEFINE_S) != NULL) {
		parse_define(line, is_on, map);
		map_reload(map);
	} else if (strstr(line, UNDEF_S)) {
		parse_undefine(line, map);
	} else if (strstr(line, IFDEF_S)) {
		parse_ifdef(line, is, map);
	} else if (strstr(line, IFNDEF_S)) {
		parse_ifndef(line, is, map);
	} else if (strstr(line, ELSE_S)) {
		if (is->if_started) {
			is->skip_lines = 1 - is->skip_lines;
		}
		return NULL;
	} else if (strstr(line, ELIF_S)) {
		if (is->skip_lines) {
			is->skip_lines = 0;
			if (is->if_started) {
				parse_if(line, is, map);
			} else {
				parse_ifdef(line, is, map);
			}
		} else {
			is->skip_lines = 1;
		}
	} else if (strstr(line, INCLUDE_S)) {
		
	} else if(strstr(line, IF_S)) {
		parse_if(line, is, map);
	 } else if (strstr(line, ENDIF_S)) {
		is->if_started = 0;
		is->skip_lines = 0;
		is->ifdef_started = 0;
		return NULL;
	 } else {
		if (is->skip_lines) {
			return NULL;
		}
		//printf("%s", parse_non_preprocessor(line, map));
		return parse_non_preprocessor(line, map);
	}
}

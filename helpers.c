/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "helpers.h"

#define D_ARG "-D"
#define I_ARG "-I"
#define O_ARG "-o"
#define MAPPINGS_VARIATION 20

void display_error() {
	printf("Error on input arguments");
}

parsed_args *alloc_args(int mappings_number,
                        int input_files_number) {
	parsed_args *args = (parsed_args *)malloc(sizeof(parsed_args));
	args->bad_args = 0;

	args->map = map_create(mappings_number + MAPPINGS_VARIATION);
	args->input_files = (char **)malloc(sizeof(char *) * input_files_number);
	args->input_files_number = input_files_number;

	return args;
}

void parse_map_arg(parsed_args *args, char *value) {
	const char delim[2] = "=";

	char *key = strtok(value, delim);
	char *val = strtok(NULL, delim);

	if (val == NULL) {
		//not sure about // !!!!!!!!!!
		map_insert(args->map, key, "");
	} else {
		map_insert(args->map, key, val);
	}
}

parsed_args *parse_arguments(int argc, char *argv[]) {
	parsed_args *args = NULL;

	// counting input files and mappings
	int mappings_number = 0;
	int input_files_number = 0;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], D_ARG) == 0) {
			mappings_number++;
		} else if (strcmp(argv[i], I_ARG) == 0) {
			input_files_number++;
		}
	}

	args = alloc_args(mappings_number, input_files_number);

	input_files_number = 0;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], D_ARG) == 0) {
			i++;
			if (i > argc) {
				args->bad_args = 1;
				display_error(args);
				return args;
			}
			parse_map_arg(args, argv[i]);
		} else if (strcmp(argv[i], I_ARG) == 0) {
			i++;
			if (i > argc) {
				args->bad_args = 1;
				display_error(args);
				return args;
			}
			args->input_files[input_files_number] = (char *)malloc(sizeof(char)
			                                        * strlen(argv[i]));
			strcpy(args->input_files[input_files_number++], argv[i]);
		} else if (strcmp(argv[i], O_ARG) == 0) {
			i++;
			if (args->output_file != NULL || i > argc) {
				args->bad_args = 1;
				display_error(args);
				return args;
			}
			args->output_file = argv[i];
		} else {
			if (args->infile != NULL) {
				args->bad_args = 1;
				display_error(args);
				return args;
			}
			args->infile = (char *)malloc(sizeof(char) * strlen(argv[i]));
			strcpy(args->infile, argv[i]);
		}
	}


	return args;
}

char *read(FILE *ifp) {
	static char buff[1024];

	if (ifp == NULL) {
		if (fgets(buff, sizeof(buff), stdin) == NULL) {
			return NULL;
		}
	} else {
		if (fgets(buff, sizeof(buff), ifp) == NULL) {
			return NULL;
		}
	}

	return buff;
}

void write(FILE *ofp, char *string) {
	if (ofp == NULL) {
		printf("%s", string);
	} else {
		fprintf(ofp, "%s", string);
	}
}

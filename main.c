/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdlib.h>

#include "helpers.h"
#include "hashmap.h"
#include "parser.h"


int main(int argc, char *argv[])
{
	parsed_args *args = parse_arguments(argc, argv);
	FILE *ifp = NULL;
	FILE *ofp = NULL;
	hashmap *map = args->map;

	if (args->bad_args == 1) {
		printf("Bad args");
		return -1;
	}

	if (args->infile != NULL) {
		ifp = fopen(args->infile, "r");

		if (ifp == NULL) {
			printf("Error on opening input file");
			return -1;
		}
	}

	if (args->output_file != NULL) {
		ofp = fopen(args->output_file, "w");


		if (ofp == NULL) {
			printf("Error on opening output file");
			return -1;
		}
	}

	char *line = read(ifp);
	multi_define *is_on = (multi_define *)malloc(sizeof(multi_define));
	is_on->key = NULL;
	is_on->multiline_define = 0;

	while (line != NULL) {
		int is = 0;
		char *parsed_line = parse_line(line, is_on, &is, map);

		if (parsed_line != NULL) {
			//printf("%s", parsed_line);
			write(ofp, parsed_line);
		}
        //free(parsed_line);

		line = read(ifp);
	}

	//map_print(map);

	map_free(map);
    free(is_on->key);
    free(is_on);
    free_args(args);
    if (ifp != NULL) {
        fclose(ifp);
    }
	fflush(ofp);
    if (ofp != NULL) {
        fclose(ofp);
    }
	return 0;
}

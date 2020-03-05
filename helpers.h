#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>

#include "hashmap.h"

typedef struct parsed_args {
    hashmap *map;
    char **input_files;
    char *output_file;
    char *infile;
    int input_files_number;
    int bad_args;
} parsed_args;

parsed_args *parse_arguments(int argc, char *argv[]);
char *read(FILE *ifp);
void write(FILE *ofp, char *string);

#endif

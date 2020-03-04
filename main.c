#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "hashmap.h"
#include "parser.h"


int main(int argc, char *argv[]) {
    parsed_args *args = parse_arguments(argc, argv);
    FILE *ifp = NULL;
    hashmap *map = args->map;

    if (args->bad_args) {
      return 12;
    }

    if (args->infile == NULL) {
      args->infile = "stdin";
    } else {
      ifp = fopen(args->infile, "r");

      if (ifp == NULL) {
        printf("Error on opening input file");
        return 12;
      }
    }

    char *line = read(ifp);
    multi_define *is_on = (multi_define *)malloc(sizeof(multi_define));
    while (line != NULL) {
      int is = 0;
      parse_line(line, is_on, &is, map);
      line = read(ifp);
    }

    //map_print(map);

    return 0;
}

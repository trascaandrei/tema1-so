#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    parsed_args *args = parse_arguments(argc, argv);
    FILE *ifp;

    if (args->bad_args) {
      return 12;
    }

    if (args->infile == NULL) {
      args->infile = "stdin";
    } else {
      ifp = fopen(args->infile, "r")

      if (ifp == NULL) {
        printf("Error on opening input file");
        return 12;
      }
    }


    return 0;
}

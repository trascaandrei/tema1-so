#include "helpers.h"

#define D_ARG "-D"
#define I_ARG "-I"
#define O_ARG "-o"
#define MAPPINGS_VARIATION 20

void display_error(parsed_args *args) {
  args->bad_args = 1;
  printf("Error on input arguments");
}

void alloc_args(parsed_args *args, int mappings_number,
                  int input_files_number) {
  args = (parsed_args *)malloc(sizeof(parsed_args));
  args->bad_args = 0;

  args->map = map_create(mappings_number + MAPPINGS_VARIATION);
  args->input_files = (char **)malloc(sizeof(char *) * input_files_number);
  args->input_files_number = input_files_number;
}

void parse_map_arg(parsed_args *args, char *value) {

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

  alloc_args(args, mappings_number, input_files_number);

  input_files_number = 0;
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], D_ARG) == 0) {
      i++;
      parse_map_arg(args, argv[i]);
    } else if (strcmp(argv[i], I_ARG) == 0) {
      i++;
      args->input_files[input_files_number] = (char *)malloc(sizeof(char)
        * strlen(argv[i]));
      strcpy(args->input_files[input_files_number++], argv[i]);
    } else if (strcmp(argv[i], O_ARG) == 0) {
      i++;
      if (strlen(args->output_file) > 0) {
        display_error(args);
        return args;
      }
      args->output_file = (char *)malloc(sizeof(char) * strlen(argv[i]));
      strcpy(args->output_file, argv[i]);
    } else {
      if (strlen(args->infile) > 0) {
        display_error(args);
        return args;
      }
      args->infile = (char *)malloc(sizeof(char) * strlen(argv[i]));
      strcpy(args->infile, argv[i]);
    }
  }


  return args;
}

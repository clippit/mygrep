#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if !defined(EXIT_TROUBLE)
# define EXIT_TROUBLE 2
#endif

void display_usage(int stauts, char* message)
{
    if (message != NULL) {
        printf("ERROR: %s\n----------------------------------------\n", message);
    }
    puts("A tiny grep impletment as the homework of Linux Programming");
    puts("Usage: mygrep [OPTION]... PATTERN [FILE]...");
    puts("Search for PATTERN in each FILE or standard input.");
    puts("Options:");
    puts("-h    Show this help.");
    exit(stauts);
}

struct global_args_t {
    char *pattern;
    char **input_files;
    int  input_files_num;
} global_args;

static const char *optstr = "h";

int main(int argc, char **argv) {
    global_args.pattern         = NULL;
    global_args.input_files     = NULL;
    global_args.input_files_num = 0;

    int opt = 0;
    while ((opt = getopt(argc, argv, optstr)) != -1) {
        switch(opt) {
        case 'h':
            display_usage(EXIT_SUCCESS, NULL);
            break;
        default: // shouldn't reach here
            break;
        }
    }
    if (argc - optind < 2)
        display_usage(EXIT_TROUBLE, "Please provide pattern and files to be greped.");
    global_args.pattern = argv[optind++];
    global_args.input_files = argv + optind;
    global_args.input_files_num = argc - optind;

    exit( EXIT_SUCCESS );
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

char *read_pattern(const char* source) {
    char *pattern = malloc( (strlen(source) + 1) * sizeof(char) );
    if (pattern != 0) {
        strcpy(pattern, source);
        return pattern;
    } else {
        display_usage(EXIT_TROUBLE, "Not enough memory space");
    }
}

int do_grep(const char* pattern, char* file) {
    printf("use %s to grep file %s\n", pattern, file);
    return 0;
}

int main(int argc, char **argv) {
    char *pattern = NULL;
    char *file    = NULL;
    int status = EXIT_SUCCESS;

    static const char *optstr = "e:h";
    int opt = 0;
    while ((opt = getopt(argc, argv, optstr)) != -1) {
        switch(opt) {
        case 'e':
            pattern = read_pattern(optarg);
            break;
        case 'h':
            display_usage(EXIT_SUCCESS, NULL);
            break;
        default: // shouldn't reach here
            break;
        }
    }
    if ( !pattern ) { // no -e option
        if (argc - optind < 2) {
            display_usage(EXIT_TROUBLE, "Please provide pattern and files to be greped.");
        } else {
            pattern = read_pattern(argv[optind++]);
        }
    } else if (argc - optind < 1) {
        display_usage(EXIT_TROUBLE, "Please provide files to be greped.");
    }

    while (optind < argc) {
        char *file = argv[optind++];
        status = do_grep(pattern, file);
    }

    exit(status);
}
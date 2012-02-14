#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

#if !defined(EXIT_TROUBLE)
# define EXIT_TROUBLE 2
#endif

void display_usage(int stauts, char* message, int errcode)
{
    if (message != NULL || errcode != 0) 
        printf("ERROR: %s\nError Code: %d\n----------------------------------------\n", message, errcode);
        fprintf(stderr, "ERROR: %s\nError Code: %d\n----------------------------------------\n", message, errcode);
    puts("A tiny grep impletment as the homework of Linux Programming");
    puts("Usage: mygrep [OPTION]... PATTERN [FILE]...");
    puts("Search for PATTERN in each FILE or standard input.");
    puts("Options:");
    puts("-h            Show this help");
    puts("-e PATTERN    Use PATTERN as the pattern. This can be used to protect a pattern");
    puts("              beginning with a hyphen (-).  (-e is specified by POSIX.)" );
    puts("-i            Ignore case distinctions.");
    exit(stauts);
}

char *read_pattern(const char* source) {
    char *pattern = malloc( (strlen(source) + 1) * sizeof(char) );
    if (pattern != 0) {
        strcpy(pattern, source);
        return pattern;
    } else {
        display_usage(EXIT_TROUBLE, "Not enough memory space", -1);
    }
    return NULL; // won't be here
}

int do_grep(regex_t* preg, char* file) {
    //printf("use %d to grep file %s\n", preg->re_nsub, file);
    return 0;
}

int main(int argc, char **argv) {
    char *pattern     = NULL;
    char *file        = NULL;
    int status        = EXIT_SUCCESS;
    int regcomp_flags = REG_BASIC;
    regex_t preg;

    static const char *optstr = "e:hi";
    int opt = 0;
    while ((opt = getopt(argc, argv, optstr)) != -1) {
        switch(opt) {
        case 'e':
            pattern = read_pattern(optarg);
            break;
        case 'h':
            display_usage(EXIT_SUCCESS, NULL, EXIT_SUCCESS);
            break;
        case 'i':
            regcomp_flags |= REG_ICASE;
            break;
        default: // shouldn't reach here
            break;
        }
    }
    if ( !pattern ) { // no -e option
        if (argc - optind < 2) {
            display_usage(EXIT_TROUBLE, "Please provide pattern and files to be greped.", -1);
        } else {
            pattern = read_pattern(argv[optind++]);
        }
    } else if (argc - optind < 1) {
        display_usage(EXIT_TROUBLE, "Please provide files to be greped.", -1);
    }

    int regcomp_errcode;
    if ( (regcomp_errcode = regcomp(&preg, pattern, regcomp_flags)) != 0 ) {
        regfree(&preg);
        display_usage(EXIT_TROUBLE, "Pattern invalid.", regcomp_errcode);
    }
        
    free(pattern);

    while (optind < argc) {
        char *file = argv[optind++];
        status = do_grep(&preg, file);
    }

    exit(status);
}

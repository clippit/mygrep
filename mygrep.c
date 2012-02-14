#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <regex.h>
#include "isdir.h"

#ifndef EXIT_TROUBLE
# define EXIT_TROUBLE 2
#endif
#define MAXLINE 1000

void display_usage(int status, char* message, int errcode)
{
    if (message != NULL || errcode != 0) 
        fprintf(stderr, "ERROR: %s\nError Code: %d\n----------------------------------------\n", message, errcode);
    puts("A tiny grep impletment as the homework of Linux Programming");
    puts("Usage: mygrep [OPTION]... PATTERN [FILE]...");
    puts("Search for PATTERN in each FILE or standard input.");
    puts("Options:");
    puts("-h            Show this help");
    puts("-e PATTERN    Use PATTERN as the pattern. This can be used to protect a pattern");
    puts("              beginning with a hyphen (-).  (-e is specified by POSIX.)" );
    puts("-i            Ignore case distinctions.");
    exit(status);
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

int do_grep(regex_t* preg, char* filename) {
    #ifdef DEBUG
    printf("----preg re_nsub %d to grep file %s\n", (int)preg, filename);
    #endif

    FILE *file = fopen(filename, "r");
    if (file == NULL)
        display_usage(EXIT_TROUBLE, "Open file failed.", errno);
    
    char line[MAXLINE];
    while( fgets(line, MAXLINE, file) != NULL ) {
        puts(line);
    }

    fclose(file);
    return 0;
}

int main(int argc, char **argv) {
    char *pattern     = NULL;
    int status        = EXIT_SUCCESS;
    regex_t *preg     = malloc(sizeof(regex_t));
    int regcomp_flags = REG_BASIC;
    int regex_errcode = 0;

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

    if ( (regex_errcode = regcomp(preg, pattern, regcomp_flags)) != 0 ) {
        regfree(preg);
        display_usage(EXIT_TROUBLE, "Pattern invalid.", regex_errcode);
    }
        
    free(pattern);

    while (optind < argc) {
        char *filename = argv[optind++];
        if (isdir(filename))
            display_usage(EXIT_TROUBLE, "Please provide a file instead of directory", -1);
        status = do_grep(preg, filename);
    }

    exit(status);
}

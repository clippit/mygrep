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

struct global_args_t {
    int is_invert;        // for option -v
    int show_line_number; // for option -n
    int multi_file;       // flag for multi file
} gargs;

void display_usage(int status, char* message, int errcode)
{
    if (message != NULL || errcode != 0) 
        fprintf(stderr, "ERROR: %s\nError Code: %d\n----------------------------------------\n", message, errcode);
    puts("A tiny grep impletment as the homework of Linux Programming");
    puts("Usage: mygrep [OPTION]... PATTERN [FILE]...");
    puts("Search for PATTERN in each FILE or standard input.");
    puts("Options:");
    puts("-h            Show this help");
    puts("-e PATTERN    Use PATTERN as the pattern. This can be used to protect a");
    puts("              pattern beginning with a hyphen (-)." );
    puts("-i            Ignore case distinctions.");
    puts("-n            Print line number with output lines.");
    puts("-v            Select non-matching lines.");
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
    printf("----preg %d to grep file %s\n", (int)preg, filename);
    #endif

    FILE *file = fopen(filename, "r");
    if (file == NULL)
        display_usage(EXIT_TROUBLE, "Open file failed.", errno);
    
    char linebuf[MAXLINE];
    int regexec_code = 0;
    int line_number = 0;
    while ( fgets(linebuf, MAXLINE, file) != NULL ) {
        line_number++;
        int len = strlen(linebuf) - 1;
        if ( linebuf[len] == '\n' )
            linebuf[len] = 0; //replace newline with null
        regexec_code = regexec(preg, linebuf, 0, NULL, 0);
        if (regexec_code > REG_NOMATCH) { // >1?
            regfree(preg);
            display_usage(EXIT_TROUBLE, "Match Error.", regexec_code);
        }

        /* this condition equals to:
           (regexec_code == 0 && gargs.is_invert == 0) 
           || (regexec_code == 1 && gargs.is_invert == 1)
        */
        if ( !(regexec_code ^ gargs.is_invert) ) {
            if (gargs.multi_file && gargs.show_line_number)
                printf("%s:%d:%s\n", filename, line_number, linebuf);
            else if (gargs.multi_file && !gargs.show_line_number)
                printf("%s:%s\n", filename, linebuf);
            else if (!gargs.multi_file && gargs.show_line_number)
                printf("%d:%s\n", line_number, linebuf);
            else
                puts(linebuf);
        }
    }

    fclose(file);
    return regexec_code;
}

int main(int argc, char **argv) {
    char *pattern          = NULL;
    int status             = 0;
    regex_t *preg          = malloc(sizeof(regex_t));
    int regcomp_flags      = REG_BASIC;
    int regex_errcode      = 0;
    gargs.is_invert        = 0;
    gargs.multi_file       = 0;
    gargs.show_line_number = 0;

    static const char *optstr = "e:hinv";
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
        case 'n':
            gargs.show_line_number = 1;
            break;
        case 'v':
            gargs.is_invert = 1;
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

    #ifdef DEBUG
    printf("optind:%d, argc:%d\n\n", optind, argc);
    #endif

    if (argc - optind > 1)
        gargs.multi_file = 1;

    while (optind < argc) {
        char *filename = argv[optind++];
        if (isdir(filename))
            display_usage(EXIT_TROUBLE, "Please provide a file instead of directory", -1);
        status = do_grep(preg, filename);
    }

    regfree(preg);
    exit(status);
}

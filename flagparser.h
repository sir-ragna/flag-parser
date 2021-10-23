
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void parseInt(int *value, char * name, int default_value, char * help_str);
void parseFlags(int argc, char *argv[]);

typedef struct {
    int *value;
    int default_value;
    char *name;
    char *help_str;
} IntFlag;

IntFlag *iflags = NULL;
size_t   iflagsc = 0;

void parseInt(int *value, char *name, int default_value, char *help_str)
{
    IntFlag int_flag;
    int_flag.value = value;
    int_flag.default_value = default_value;
    int_flag.name = name;
    int_flag.help_str = help_str;
    *value = default_value;

    iflagsc++;
    iflags = (IntFlag *)realloc(iflags, sizeof(IntFlag) * iflagsc);
    iflags[iflagsc - 1] = int_flag;
}

void printUsage(char *filename)
{
    fprintf(stdout, "%s [args]\n", filename);
    for (size_t j = 0; j < iflagsc; j++)
        fprintf(stdout, "\t%s\t%s (default: %i)\n", 
            iflags[j].name, 
            iflags[j].help_str, 
            *iflags[j].value
        );
}

void parseFlags(int argc, char *argv[])
{
    if (iflags == NULL || iflagsc == 0)
    {
        fprintf(stderr, "No flags to parse\n");
        /* maybe someone called parseFlags() twice? */
        exit(1);
    }

    if (argc == 1)
    {
        printUsage(argv[0]);
        exit(1);
    }

    /* Parse all flags */
    for (int i = 1; i < argc; i++)
    {
        char *arg = argv[i];
        
        if (arg[0] != '-')
            continue; /* not a flag */
        
        for (size_t j = 0; j < iflagsc; j++)
        {
            if (strcmp(iflags[j].name, arg) != 0)
                continue;

            // Retrieve the next arg and use as value
            i++;
            if (i < argc) /* convert str to int */
                *iflags[j].value = atoi(argv[i]);
            else
                fprintf(stderr, "No value found for \"%s\"\n", arg);
        }
    }

    /* Maybe clean-up and free memory here? */
    free(iflags);
    iflags = NULL;
    iflagsc = 0;
}

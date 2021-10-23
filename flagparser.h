
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void initFlagParser(int argc, char *argv[]);
void parseInt(int * out, char * name, int def_val, char * help_str);

typedef struct {
    int argc;
    char **argv;
} FlagArgs;

typedef struct {
    int *out;
    char *name;
    char *help_str;
} IntFlag;

FlagArgs flagArgs;
IntFlag *iflags = NULL;
size_t   iflagsc = 0;

void initFlagParser(int argc, char *argv[])
{
    flagArgs.argc = argc;
    flagArgs.argv = argv;
}

void parseInt(int *out, char *name, int def_val, char * help_str)
{
    IntFlag int_flag;
    int_flag.out = out;
    int_flag.name = name;
    int_flag.help_str = help_str;
    *out = def_val;

    iflagsc++;
    iflags = (IntFlag *)realloc(iflags, sizeof(IntFlag) * iflagsc);
    iflags[iflagsc - 1] = int_flag;
}

void parseFlags()
{
    if (iflags == NULL || iflagsc == 0)
    {
        fprintf(stderr, "No flags to parse\n");
        /* maybe someone called parseFlags() twice? */
        exit(1);
    }

    /* Parse all flags */
    for (int i = 1; i < flagArgs.argc; i++)
    {
        char *arg = flagArgs.argv[i];
        
        if (arg[0] != '-')
            continue; /* not a flag */
        
        for (size_t j = 0; j < iflagsc; j++)
        {
            if (strcmp(iflags[j].name, arg) != 0)
                continue;

            // Retrieve the next arg and use as value
            i++;
            if (i < flagArgs.argc) /* convert str to int */
                *iflags[j].out = atoi(flagArgs.argv[i]);
            else
                fprintf(stderr, "No value found for \"%s\"\n", arg);
        }
    }

    /* Maybe clean-up and free memory here? */
    free(iflags);
    iflags = NULL;
    iflagsc = 0;
}

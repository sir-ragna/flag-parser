
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void parseInt(int *value, char *name, int default_value, char *help_str);
void parseStr(char **value, char *name, char *default_value, char *help_str);
void _printUsage(char *filename);
void parseFlags(int argc, char *argv[]);

typedef struct {
    int *value;
    int default_value;
    char *name;
    char *help_str;
} IntFlag;

IntFlag *iflags = NULL;
size_t   iflagsc = 0;

typedef struct {
    char **value;
    char *default_value;
    char *name;
    char *help_str;
} StrFlag;

StrFlag *sflags = NULL;
size_t   sflagsc = 0;

void parseInt(int *value, char *name, int default_value, char *help_str)
{
    IntFlag int_flag;
    int_flag.name = name;
    int_flag.value = value;
    int_flag.help_str = help_str;
    int_flag.default_value = default_value;
    *value = default_value;

    iflagsc++;
    iflags = (IntFlag *)realloc(iflags, sizeof(IntFlag) * iflagsc);
    iflags[iflagsc - 1] = int_flag;
}

void parseStr(char **value, char *name, char *default_value, char *help_str)
{
    StrFlag str_flag;
    str_flag.value = value;

    str_flag.name = (char *)malloc(strlen(name) + 1);
    strcpy(str_flag.name, name);
    
    if (default_value == NULL) 
    {
        *str_flag.value = NULL;
        str_flag.default_value = NULL;
    }
    else
    {
        int default_val_len = strlen(default_value);

        *value = (char *)malloc(default_val_len + 1);
        strcpy(*value, default_value);
        
        str_flag.default_value = (char *)malloc(default_val_len + 1);
        strcpy(str_flag.default_value, default_value);
    }

    if (help_str == NULL)
    {
        str_flag.help_str = NULL;
    }
    else
    {
        str_flag.help_str = (char *)malloc(strlen(help_str) + 1);
        strcpy(str_flag.help_str, help_str);
    }
    
    sflagsc++;
    sflags = (StrFlag *)realloc(sflags, sizeof(StrFlag) * sflagsc);
    sflags[sflagsc - 1] = str_flag;
}

void _printUsage(char *filename)
{
    fprintf(stdout, "%s [args]\n", filename);
    size_t i;
    for (i = 0; i < iflagsc; i++)
    {
        fprintf(stdout, "\t%s\t%s (default: %i)\n", 
            iflags[i].name, 
            iflags[i].help_str, 
            iflags[i].default_value
        );
    }
    for (i = 0; i < sflagsc; i++)
    {
        if (sflags[i].default_value == NULL)
            fprintf(stdout, "\t%s\t%s (default: NULL)\n", 
                sflags[i].name, 
                sflags[i].help_str);    
        else
            fprintf(stdout, "\t%s\t%s (default: \"%s\")\n", 
                sflags[i].name, 
                sflags[i].help_str, 
                sflags[i].default_value
            );
    }

}

void parseFlags(int argc, char *argv[])
{
    if ((iflags == NULL || iflagsc == 0) && 
        (sflags == NULL || sflagsc == 0))
    {
        fprintf(stderr, "No flags to parse.\n");
        fprintf(stderr, "Did you call parseFlags twice?\n");
        exit(1);
    }

    if (argc == 1)
    {
        _printUsage(argv[0]);
        exit(1);
    }

    /* Parse all flags */
    int i = 1;
    for (; i < argc; i++)
    {
        char *arg = argv[i];
        
        if (arg[0] != '-')
            continue; /* not a flag */
        
        /* Parse int flags */
        size_t j;
        for (j = 0; j < iflagsc; j++)
        {
            if (strcmp(iflags[j].name, arg) != 0)
                continue;

            i++; /* retrieve the next arg */
            if (i < argc) 
            {   /* convert str to int */
                *iflags[j].value = atoi(argv[i]);
            }
            else
            {
                fprintf(stderr, "No value found for \"%s\"\n", arg);
                _printUsage(argv[0]);
                exit(2);
            }
        }

        /* Parse str flags */
        for (j = 0; j < sflagsc; j++)
        {
            if (strcmp(sflags[j].name, arg) != 0)
                continue;

            i++; /* Retrieve the next argument */
            if (i < argc) 
            {   
                int arg_len = strlen(argv[i]);
                *sflags[j].value = (char *)malloc(arg_len + 1);
                strcpy(*sflags[j].value, argv[i]);
            }
            else
            {
                fprintf(stderr, "No value found for \"%s\"\n", arg);
                _printUsage(argv[0]);
                exit(2);
            }
        }
    }

    /* Clean-up, you ain't printing usage after this
     * and you won't be able to call parseFlags */
    if (iflags != NULL)
    {
        free(iflags);
        iflags = NULL;
    }
    iflagsc = 0;

    for (i = 0; i < sflagsc; i++)
    {
        if (sflags[i].default_value != NULL)
            free(sflags[i].default_value);
        
        if (sflags[i].help_str != NULL)
            free(sflags[i].help_str);

        if (sflags[i].name != NULL)
            free(sflags[i].name);
    }
    
    if (sflags != NULL)
    {
        free(sflags);
        sflags = NULL;
    }
    sflagsc = 0;
}

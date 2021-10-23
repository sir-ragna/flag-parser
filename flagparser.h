
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void gflg_int_var(int *value, const char *name, int default_value, const char *help_str);
void gflg_str_var(char **value, const char *name, const char *default_value, const char *help_str);
void _gflg_print_usage(const char *filename);
void gflg_parse_flags(const int argc, const char *argv[]);

typedef struct {
    int *value;
    int default_value;
    char *name;
    char *help_str;
} gflg_int_flag;

gflg_int_flag *iflags = NULL;
size_t         iflagsc = 0;

typedef struct {
    char **value;
    char *default_value;
    char *name;
    char *help_str;
} gflg_str_flag;

gflg_str_flag *sflags = NULL;
size_t         sflagsc = 0;

typedef struct {
    bool *value;
    char *name;
    char *help_str;
} gflg_bool_flag;

gflg_bool_flag *bflags = NULL;
size_t          bflagsc = 0;

/* Boolean flags are always false by default */
void gflg_bool_var(bool *value, char *name, const char *help_str)
{
    gflg_bool_flag bool_flag;
    bool_flag.name = name;
    bool_flag.value = value;
    *value = false;

    bflagsc++;
    bflags = (gflg_bool_flag *)realloc(bflags, sizeof(gflg_bool_flag) * bflagsc);
    bflags[bflagsc - 1] = bool_flag;
}

void gflg_int_var(int *value, const char *name, int default_value, const char *help_str)
{
    gflg_int_flag int_flag;
    
    int_flag.name = (char *)malloc(strlen(name) + 1);
    strcpy(int_flag.name, name);

    int_flag.help_str = (char *)malloc(strlen(help_str) + 1);
    strcpy(int_flag.help_str, help_str);

    int_flag.value = value;
    int_flag.default_value = default_value;
    *value = default_value;

    iflagsc++;
    iflags = (gflg_int_flag *)realloc(iflags, sizeof(gflg_int_flag) * iflagsc);
    iflags[iflagsc - 1] = int_flag;
}

void gflg_str_var(char **value, const char *name, const char *default_value, const char *help_str)
{
    gflg_str_flag str_flag;
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
    sflags = (gflg_str_flag *)realloc(sflags, sizeof(gflg_str_flag) * sflagsc);
    sflags[sflagsc - 1] = str_flag;
}

void _gflg_print_usage(const char *filename)
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

void gflg_parse_flags(const int argc, const char *argv[])
{
    if ((iflags == NULL || iflagsc == 0) && 
        (sflags == NULL || sflagsc == 0))
    {
        fprintf(stderr, "No flags to parse.\n");
        fprintf(stderr, "Did you call gflg_parse_flags twice?\n");
        exit(1);
    }

    if (argc == 1)
    {
        _gflg_print_usage(argv[0]);
        exit(1);
    }

    /* Parse all flags */
    size_t i = 1;
    for (; i < (size_t)argc; i++)
    {
        const char *arg = argv[i];
        
        if (arg[0] != '-')
            continue; /* not a flag */
        
        /* Parse int flags */
        size_t j;
        for (j = 0; j < iflagsc; j++)
        {
            if (strcmp(iflags[j].name, arg) != 0)
                continue;

            i++; /* retrieve the next arg */
            if (i < (size_t)argc) 
            {   /* convert str to int */
                *iflags[j].value = atoi(argv[i]);
            }
            else
            {
                fprintf(stderr, "No value found for \"%s\"\n", arg);
                _gflg_print_usage(argv[0]);
                exit(2);
            }
        }

        /* Parse str flags */
        for (j = 0; j < sflagsc; j++)
        {
            if (strcmp(sflags[j].name, arg) != 0)
                continue;

            i++; /* Retrieve the next argument */
            if (i < (size_t)argc) 
            {   
                int arg_len = strlen(argv[i]);
                *sflags[j].value = (char *)realloc(*sflags[j].value, arg_len + 1);
                strcpy(*sflags[j].value, argv[i]);
            }
            else
            {
                fprintf(stderr, "No value found for \"%s\"\n", arg);
                _gflg_print_usage(argv[0]);
                exit(2);
            }
        }
    }

    /* Clean-up, you ain't printing usage after this
     * and you won't be able to call gflg_parse_flags */
    for (i = 0; i < iflagsc; i++)
    {
        if (iflags[i].name != NULL)
        {
            free(iflags[i].name);
            iflags[i].name = NULL;
        }        
        if (iflags[i].help_str != NULL)
        {
            free(iflags[i].help_str);
            iflags[i].help_str = NULL;
        }
    }

    if (iflags != NULL)
    {
        free(iflags);
        iflags = NULL;
    }
    iflagsc = 0;

    for (i = 0; i < sflagsc; i++)
    {
        if (sflags[i].default_value != NULL)
        {
            free(sflags[i].default_value);
            sflags[i].default_value = NULL;
        }        
        if (sflags[i].help_str != NULL)
        {
            free(sflags[i].help_str);
            sflags[i].help_str = NULL;
        }
        if (sflags[i].name != NULL)
        {
            free(sflags[i].name);
            sflags[i].name = NULL;
        }
    }
    
    if (sflags != NULL)
    {
        free(sflags);
        sflags = NULL;
    }
    sflagsc = 0;
}

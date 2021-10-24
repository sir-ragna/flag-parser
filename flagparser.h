
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void flg_bool_var(bool *value, const char *name, const char *help_str);
void flg_int_var(int *value, const char *name, int default_value, const char *help_str);
void flg_str_var(char **value, const char *name, const char *default_value, const char *help_str);
void _flg_free_mem();
void _flg_print_usage(const char *filename);
void flg_parse_flags(const int argc, const char *argv[]);

typedef struct {
    int *value;
    int default_value;
    char *name;
    char *help_str;
} flg_int_flag;

flg_int_flag *iflags  = NULL;
size_t        iflagsc = 0;

typedef struct {
    char **value;
    char *default_value;
    char *name;
    char *help_str;
} flg_str_flag;

flg_str_flag *sflags  = NULL;
size_t        sflagsc = 0;

typedef struct {
    bool *value;
    char *name;
    char *help_str;
} flg_bool_flag;

flg_bool_flag *bflags  = NULL;
size_t         bflagsc = 0;

/* Boolean flags are always false by default */
void flg_bool_var(bool *value, const char *name, const char *help_str)
{
    flg_bool_flag bool_flag;

    bool_flag.name = (char *)malloc(strlen(name) + 1);
    strcpy(bool_flag.name, name);

    bool_flag.help_str = (char *)malloc(strlen(help_str) + 1);
    strcpy(bool_flag.help_str, help_str);

    bool_flag.value = value;
    *value = false; 
    /* no default_value, boolean switches default to false */

    bflagsc++;
    bflags = (flg_bool_flag *)realloc(bflags, sizeof(flg_bool_flag) * bflagsc);
    bflags[bflagsc - 1] = bool_flag;
}

void flg_int_var(int *value, const char *name, int default_value, const char *help_str)
{
    flg_int_flag int_flag;
    
    int_flag.name = (char *)malloc(strlen(name) + 1);
    strcpy(int_flag.name, name);

    int_flag.help_str = (char *)malloc(strlen(help_str) + 1);
    strcpy(int_flag.help_str, help_str);

    int_flag.value = value;
    int_flag.default_value = default_value;
    *value = default_value;

    iflagsc++;
    iflags = (flg_int_flag *)realloc(iflags, sizeof(flg_int_flag) * iflagsc);
    iflags[iflagsc - 1] = int_flag;
}

void flg_str_var(char **value, const char *name, const char *default_value, const char *help_str)
{
    flg_str_flag str_flag;
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
    sflags = (flg_str_flag *)realloc(sflags, sizeof(flg_str_flag) * sflagsc);
    sflags[sflagsc - 1] = str_flag;
}

void _flg_print_usage(const char *filename)
{
    fprintf(stdout, "%s [args]\n", filename);
    size_t i;
    for (i = 0; i < sflagsc; i++) /* Print the str flags help */
    {
        if (sflags[i].default_value == NULL)
            fprintf(stdout, "\t%s <string>\n\t\t%s (default: NULL)\n\n", 
                sflags[i].name, 
                sflags[i].help_str);    
        else
            fprintf(stdout, "\t%s <string>\n\t\t%s (default: \"%s\")\n\n", 
                sflags[i].name, 
                sflags[i].help_str, 
                sflags[i].default_value
            );
    }

    for (i = 0; i < iflagsc; i++) /* Print the int flags help */
    {
        fprintf(stdout, "\t%s <n>\n\t\t%s (default: %i)\n\n", 
            iflags[i].name, 
            iflags[i].help_str, 
            iflags[i].default_value
        );
    }
    
    for (i = 0; i < bflagsc; i++) /* Print the boolean flags help */
    {
        fprintf(stdout, "\t%s\n\t\t%s (default: false)\n\n", 
            bflags[i].name, 
            bflags[i].help_str
        );
    }
}

void _flg_free_mem()
{
    size_t i;
    /* Clean up boolean flags */
    for (i = 0; i < bflagsc; i++)
    {
        if (bflags[i].name != NULL)
        {
            free(bflags[i].name);
            bflags[i].name = NULL;
        }        
        if (bflags[i].help_str != NULL)
        {
            free(bflags[i].help_str);
            bflags[i].help_str = NULL;
        }
    }

    if (bflags != NULL)
    {
        free(bflags);
        bflags = NULL;
    }
    bflagsc = 0;

    /* Clean up int flags */
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

    /* Clean up str flags */
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

/* Parses the flags. Call once.
 * At the end, some memory clean-up is performed making it impossible
 * to use parse the flags a second time */
void flg_parse_flags(const int argc, const char *argv[])
{
    bool print_usage;
    /* flg_bool_var(&print_usage, "--usage", "Print usage"); */
    flg_bool_var(&print_usage, "--help",  "Print usage");

    if ((iflags == NULL || iflagsc == 0) && 
        (sflags == NULL || sflagsc == 0) &&
        (bflags == NULL || bflagsc == 0))
    {
        fprintf(stderr, "No flags to parse.\n");
        fprintf(stderr, "Did you call flg_parse_flags twice?\n");
        exit(1);
    }

    size_t i;
    /* Iterate over all args. Start i=1 to exclude the original 
     * file name. */
    for (i = 1; i < (size_t)argc; i++)
    {
        const char *arg = argv[i];
        
        if (arg[0] != '-')
            continue; /* not a flag */
        
        size_t j;
        /* Parse boolean flags */
        for (j = 0; j < bflagsc; j++)
        {
            if (strcmp(bflags[j].name, arg) != 0)
                continue;

            /* If the boolean flag exists, enable it */
            *bflags[j].value = true;
        }

        /* Parse int flags */
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
                _flg_print_usage(argv[0]);
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
                _flg_print_usage(argv[0]);
                exit(2);
            }
        }
    }

    if (print_usage)
    {
        _flg_print_usage(argv[0]);
        exit(1);
    }

    /* Clean-up, you ain't printing usage after this
     * and you won't be able to call flg_parse_flags */
    _flg_free_mem();
}

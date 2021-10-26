
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

char * _flg_duplicate_str(const char *source);
void _flg_free_mem();
void flg_print_usage(const char *filename);
void flg_parse_flags(const int argc, const char *argv[]);

typedef struct {
    int *value;           /* store of the actual value */
    char *flag;           /* short form name "-n"      */
    char *long_form_flag; /* long form name "--name"   */
    int default_value;    /* default value             */
    char *help_str;       /* help string               */
} _flg_int_flag;

_flg_int_flag *_flg_iflags  = NULL;
size_t _flg_iflagsc = 0;

typedef struct {
    char **value;
    char *flag;           /* short from name "-n"    */
    char *long_form_flag; /* long form name "--name" */
    char *default_value;
    char *help_str;
} _flg_str_flag;

_flg_str_flag *_flg_sflags  = NULL;
size_t _flg_sflagsc = 0;

typedef struct {
    bool *value;
    char *flag;           /* short form flag "-n"    */
    char *long_form_flag; /* long form flag "--name" */
    char *help_str;
} _flg_bool_flag;

_flg_bool_flag *_flg_bflags = NULL;
size_t _flg_bflagsc = 0;


void flg_print_usage(const char *filename)
{
    fprintf(stdout, "%s [args]\n", filename);
    size_t i;
    for (i = 0; i < _flg_sflagsc; i++) /* Print the str flags help */
    {
        if (_flg_sflags[i].default_value == NULL)
            fprintf(stdout, "\t%s <string>\n\t\t%s (default: NULL)\n\n", 
                _flg_sflags[i].name, 
                _flg_sflags[i].help_str);    
        else
            fprintf(stdout, "\t%s <string>\n\t\t%s (default: \"%s\")\n\n", 
                _flg_sflags[i].name, 
                _flg_sflags[i].help_str, 
                _flg_sflags[i].default_value
            );
    }

    for (i = 0; i < _flg_iflagsc; i++) /* Print the int flags help */
    {
        fprintf(stdout, "\t%s <n>\n\t\t%s (default: %i)\n\n", 
            _flg_iflags[i].name, 
            _flg_iflags[i].help_str, 
            _flg_iflags[i].default_value
        );
    }
    
    for (i = 0; i < _flg_bflagsc; i++) /* Print the boolean flags help */
    {
        fprintf(stdout, "\t%s\n\t\t%s (default: false)\n\n", 
            _flg_bflags[i].name, 
            _flg_bflags[i].help_str
        );
    }
}

void _flg_free_mem()
{
    size_t i;
    /* Clean up boolean flags */
    for (i = 0; i < _flg_bflagsc; i++)
    {
        if (_flg_bflags[i].name != NULL)
        {
            free(_flg_bflags[i].name);
            _flg_bflags[i].name = NULL;
        }        
        if (_flg_bflags[i].help_str != NULL)
        {
            free(_flg_bflags[i].help_str);
            _flg_bflags[i].help_str = NULL;
        }
    }

    if (_flg_bflags != NULL)
    {
        free(_flg_bflags);
        _flg_bflags = NULL;
    }
    _flg_bflagsc = 0;

    /* Clean up int flags */
    for (i = 0; i < _flg_iflagsc; i++)
    {
        if (_flg_iflags[i].name != NULL)
        {
            free(_flg_iflags[i].name);
            _flg_iflags[i].name = NULL;
        }        
        if (_flg_iflags[i].help_str != NULL)
        {
            free(_flg_iflags[i].help_str);
            _flg_iflags[i].help_str = NULL;
        }
    }

    if (_flg_iflags != NULL)
    {
        free(_flg_iflags);
        _flg_iflags = NULL;
    }
    _flg_iflagsc = 0;

    /* Clean up str flags */
    for (i = 0; i < _flg_sflagsc; i++)
    {
        if (_flg_sflags[i].default_value != NULL)
        {
            free(_flg_sflags[i].default_value);
            _flg_sflags[i].default_value = NULL;
        }        
        if (_flg_sflags[i].help_str != NULL)
        {
            free(_flg_sflags[i].help_str);
            _flg_sflags[i].help_str = NULL;
        }
        if (_flg_sflags[i].name != NULL)
        {
            free(_flg_sflags[i].name);
            _flg_sflags[i].name = NULL;
        }
    }
    
    if (_flg_sflags != NULL)
    {
        free(_flg_sflags);
        _flg_sflags = NULL;
    }
    _flg_sflagsc = 0;
}

char * _flg_duplicate_str(const char *source)
{
    if (source == NULL)
        return NULL;
    
    char *dest = (char *)malloc(strlen(source) + 1);     
    strcpy(dest, source); /* strcpy copies over the null terminator */
    
    return dest;
}

char **
flg_string_arg(
    const char *flag, 
    const char *long_form_flag, 
    const char *default_value, 
    const char *help_str)
{
    char **value = (char **)malloc(sizeof(char *));
    _flg_str_flag str_flag;

    str_flag.flag           = _flg_duplicate_str(flag);
    str_flag.long_form_flag = _flg_duplicate_str(long_form_flag);
    str_flag.help_str       = _flg_duplicate_str(help_str);

    str_flag.value = value;
    *str_flag.value = _flg_duplicate_str(default_value);

    return value;
}

int * 
flg_int_arg(
    const char *flag,    /* short form flag */
    const char *long_form_flag, /* long form flag */ 
    const int default_value, 
    const char *help_str)
{
    int *value = (int *)malloc(sizeof(int));
    _flg_int_flag int_flag;
    size_t len = 0;
    
    int_flag.flag           = _flg_duplicate_str(flag);
    int_flag.long_form_flag = _flg_duplicate_str(long_form_flag);
    int_flag.help_str       = _flg_duplicate_str(help_str);

    int_flag.default_value = default_value;
    *value = default_value;

    _flg_iflagsc++;
    _flg_iflags = (_flg_int_flag *) realloc(
        _flg_iflags, 
        _flg_iflagsc * sizeof(_flg_int_flag)
    );
    _flg_iflags[_flg_iflagsc - 1] = int_flag;

    return value;
}

bool * 
flg_bool_arg(
    const char *flag, 
    const char *long_form_flag, 
    const char *help_str)
{
    bool *value = (bool *)malloc(sizeof(bool));
    _flg_bool_flag bool_flag;
    
    bool_flag.flag           = _flg_duplicate_str(flag);
    bool_flag.long_form_flag = _flg_duplicate_str(long_form_flag);
    bool_flag.help_str       = _flg_duplicate_str(help_str);

    *value = false; /* Boolean flags always default to false */

    _flg_bflagsc++;
    _flg_bflags = (_flg_bool_flag *) realloc(
        _flg_bflags, 
        _flg_bflagsc * sizeof(_flg_bool_flag)
    );
    _flg_bflags[_flg_bflagsc - 1] = bool_flag;

    return value;
}

/* Parses the flags. Call once.
 * At the end, some memory clean-up is performed making it impossible
 * to use parse the flags a second time */
void flg_parse_flags(const int argc, const char *argv[])
{
    bool print_usage;
    /* flg_bool_var(&print_usage, "--usage", "Print usage"); */
    flg_bool_var(&print_usage, "--help",  "Print usage");

    if ((_flg_iflags == NULL || _flg_iflagsc == 0) && 
        (_flg_sflags == NULL || _flg_sflagsc == 0) &&
        (_flg_bflags == NULL || _flg_bflagsc == 0))
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
        for (j = 0; j < _flg_bflagsc; j++)
        {
            if (strcmp(_flg_bflags[j].name, arg) != 0)
                continue;

            /* If the boolean flag exists, enable it */
            *_flg_bflags[j].value = true;
        }

        /* Parse int flags */
        for (j = 0; j < _flg_iflagsc; j++)
        {
            if (strcmp(_flg_iflags[j].name, arg) != 0)
                continue;

            i++; /* retrieve the next arg */
            if (i < (size_t)argc) 
            {   /* convert str to int */
                *_flg_iflags[j].value = atoi(argv[i]);
            }
            else
            {
                fprintf(stderr, "No value found for \"%s\"\n", arg);
                flg_print_usage(argv[0]);
                exit(2);
            }
        }

        /* Parse str flags */
        for (j = 0; j < _flg_sflagsc; j++)
        {
            if (strcmp(_flg_sflags[j].name, arg) != 0)
                continue;

            i++; /* Retrieve the next argument */
            if (i < (size_t)argc) 
            {   
                int arg_len = strlen(argv[i]);
                *_flg_sflags[j].value = (char *)realloc(
                    *_flg_sflags[j].value, 
                    arg_len + 1
                );
                strcpy(*_flg_sflags[j].value, argv[i]);
            }
            else
            {
                fprintf(stderr, "No value found for \"%s\"\n", arg);
                flg_print_usage(argv[0]);
                exit(2);
            }
        }
    }

    if (print_usage)
    {
        flg_print_usage(argv[0]);
         for (i = 0; i < _flg_sflagsc; i++)
         {   /* De-alloc the strings that are otherwise the
              * caller's responsibility. Because we are exiting early
              * we need to free them ourselves. */
             free(*_flg_sflags[i].value);
             *_flg_sflags[i].value = NULL;
         }
        _flg_free_mem(); /* Do regular clean-up before exit */
        exit(0);
    }

    /* Clean-up, you ain't printing usage after this
     * and you won't be able to call flg_parse_flags again. */
    _flg_free_mem();
}

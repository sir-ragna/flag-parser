
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

char * _flg_duplicate_str(const char *source);
void _flg_free_mem();
void flg_print_usage(const char *filename);

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


typedef struct {
    char *name;
    unsigned int minimum_amount;
    char *help_str;
} _flg_rest_collection;

_flg_rest_collection *_flg_rest_col = NULL;

void flg_define_rest_collection(const char *name, unsigned short minimum, const char *help_str)
{
    if (_flg_rest_col != NULL)
    {
        fprintf(stderr, "There is already a rest collection defined\n");
        exit(6);
    }
    _flg_rest_col = (_flg_rest_collection *)malloc(sizeof(_flg_rest_collection));
    _flg_rest_col->name = _flg_duplicate_str(name);
    _flg_rest_col->help_str = _flg_duplicate_str(help_str);
    _flg_rest_col->minimum_amount = minimum;
}

void flg_print_usage(const char *filename)
{
    fprintf(stdout, "%s [OPTIONS]...\n", filename);
    size_t i;
    for (i = 0; i < _flg_sflagsc; i++) /* Print the str flags help */
    {
        if (_flg_sflags[i].default_value == NULL)
            fprintf(stdout, "\t%s <string>\n\t\t%s (default: NULL)\n\n", 
                _flg_sflags[i].flag, 
                _flg_sflags[i].help_str);    
        else
            fprintf(stdout, "\t%s <string>\n\t\t%s (default: \"%s\")\n\n", 
                _flg_sflags[i].flag, 
                _flg_sflags[i].help_str, 
                _flg_sflags[i].default_value
            );
    }

    for (i = 0; i < _flg_iflagsc; i++) /* Print the int flags help */
    {
        fprintf(stdout, "\t%s <n>\n\t\t%s (default: %i)\n\n", 
            _flg_iflags[i].flag, 
            _flg_iflags[i].help_str, 
            _flg_iflags[i].default_value
        );
    }
    
    for (i = 0; i < _flg_bflagsc; i++) /* Print the boolean flags help */
    {
        fprintf(stdout, "\t%s\n\t\t%s (default: false)\n\n", 
            _flg_bflags[i].flag, 
            _flg_bflags[i].help_str
        );
    }
}

void _flg_free_bflag(_flg_bool_flag *bf)
{
    /* We don't free bf->value on purpose
     * that pointer has been given back to the caller
     * The caller might still use it and will be responsible
     * to free it themselves. */
    if (bf->flag != NULL)
    {
        free(bf->flag);
        bf->flag = NULL;
    }
    if (bf->long_form_flag != NULL)
    {
        free(bf->long_form_flag);
        bf->long_form_flag = NULL;
    }
    if (bf->help_str != NULL)
    {
        free(bf->help_str);
        bf->help_str = NULL;
    }
}

void _flg_free_iflag(_flg_int_flag *int_flag)
{
    /* We don't free int_flag->value on purpose
     * that pointer has been given back to the caller
     * The caller might still use it and will be responsible
     * to free it themselves. */
    if (int_flag->flag != NULL)
    {
        free(int_flag->flag);
        int_flag->flag = NULL;
    }
    if (int_flag->long_form_flag != NULL)
    {
        free(int_flag->long_form_flag);
        int_flag->long_form_flag = NULL;
    }
    if (int_flag->help_str != NULL)
    {
        free(int_flag->help_str);
        int_flag->help_str = NULL;
    }
}


void _flg_free_mem()
{
    size_t i;
    /* Clean up boolean flags */
    for (i = 0; i < _flg_bflagsc; i++)
    {
        _flg_free_bflag(&_flg_bflags[i]);
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
        _flg_free_iflag(&_flg_iflags[i]);
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
        if (_flg_sflags[i].flag != NULL)
        {
            free(_flg_sflags[i].flag);
            _flg_sflags[i].flag = NULL;
        }
        if (_flg_sflags[i].long_form_flag != NULL)
        {
            free(_flg_sflags[i].long_form_flag);
            _flg_sflags[i].long_form_flag = NULL;
        }
    }
    
    if (_flg_sflags != NULL)
    {
        free(_flg_sflags);
        _flg_sflags = NULL;
    }
    _flg_sflagsc = 0;

    /* remove the rest collection */
    if (_flg_rest_col != NULL)
    {
        if (_flg_rest_col->name != NULL)
        {
            free(_flg_rest_col->name);
            _flg_rest_col->name = NULL;
        }
        if (_flg_rest_col->help_str != NULL)
        {
            free(_flg_rest_col->help_str);
            _flg_rest_col->help_str = NULL;
        }
        free(_flg_rest_col);
        _flg_rest_col = NULL;
    }
}

char * _flg_duplicate_str(const char *source)
{
    if (source == NULL)
        return NULL;
    
    char *dest = (char *)malloc(strlen(source) + 1);     
    strcpy(dest, source); /* strcpy copies over the null terminator */
    
    return dest;
}


/* Check whether this flag has already been defined before */
bool _flg_check_existence(const char *flag, const char *long_form_flag)
{
    size_t i;
    for (i = 0; i < _flg_iflagsc; i++)
    {
        if (strcmp(_flg_iflags[i].flag, flag) == 0 ||
            strcmp(_flg_iflags[i].long_form_flag, long_form_flag) == 0)
            return true; /* found an int flag */
    }
    for (i = 0; i < _flg_sflagsc; i++)
    {
        if (strcmp(_flg_sflags[i].flag, flag) == 0 ||
            strcmp(_flg_sflags[i].long_form_flag, long_form_flag) == 0)
            return true;
    }
    for (i = 0; i < _flg_bflagsc; i++)
    {
        if (strcmp(_flg_bflags[i].flag, flag) == 0 ||
            strcmp(_flg_bflags[i].long_form_flag, long_form_flag) == 0)
            return true;
    }
    
    return false; /* no collisions with existing flags found */
}

/* Exit program upon validation failure
 * exit with code 4 when the flags are NULL or EMPTY
 * exit with code 5 when the flags are already defined */
void _flg_validate_flag_definition(const char *flag, const char *long_form_flag)
{
    if ((flag == NULL || flag[0] == '\0') /* NULL or empty */
        && 
        (long_form_flag == NULL || long_form_flag[0] == '\0')
       )
    {
        fprintf(stderr, "You need to define the flag, either in "
                "short form(-f) or in long form(--flag) format\n");
        exit(4);
    }

    if (_flg_check_existence(flag, long_form_flag))
    {
        fprintf(stderr, "Flag definition collision\n"
            "Please note the `-h` and `--help` flags are predefined\n");
        exit(5);
    }
}

char **
flg_string_arg(
    const char *flag, 
    const char *long_form_flag, 
    const char *default_value, 
    const char *help_str)
{
    _flg_validate_flag_definition(flag, long_form_flag);

    char **value = (char **)malloc(sizeof(char *));
    _flg_str_flag str_flag;

    str_flag.flag           = _flg_duplicate_str(flag);
    str_flag.long_form_flag = _flg_duplicate_str(long_form_flag);
    str_flag.help_str       = _flg_duplicate_str(help_str);
    str_flag.default_value  = _flg_duplicate_str(default_value);

    str_flag.value = value;
    *str_flag.value = NULL; /* _flg_duplicate_str(default_value); */

    _flg_sflagsc++;
    _flg_sflags = (_flg_str_flag *) realloc(
        _flg_sflags, 
        _flg_sflagsc * sizeof(_flg_str_flag)
    );
    _flg_sflags[_flg_sflagsc - 1] = str_flag;

    return value;
}

int * 
flg_int_arg(
    const char *flag,    /* short form flag */
    const char *long_form_flag, /* long form flag */ 
    const int default_value, 
    const char *help_str)
{
    _flg_validate_flag_definition(flag, long_form_flag);

    int *value = (int *)malloc(sizeof(int));
    _flg_int_flag int_flag;
    
    int_flag.flag           = _flg_duplicate_str(flag);
    int_flag.long_form_flag = _flg_duplicate_str(long_form_flag);
    int_flag.help_str       = _flg_duplicate_str(help_str);

    int_flag.value = value;
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
    _flg_validate_flag_definition(flag, long_form_flag);

    bool *value = (bool *)malloc(sizeof(bool));
    _flg_bool_flag bool_flag;

    bool_flag.flag           = _flg_duplicate_str(flag);
    bool_flag.long_form_flag = _flg_duplicate_str(long_form_flag);
    bool_flag.help_str       = _flg_duplicate_str(help_str);

    bool_flag.value = value;
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
 * to use parse the flags a second time 
 * Exits program with code 3 if an unrecognised option was found.
 * Exits program with code 2 if no value was provded for a flag. */
unsigned int flg_parse_flags(const int argc, const char *argv[])
{
    int exit_code = 0;

    if ((_flg_iflags == NULL || _flg_iflagsc == 0) && 
        (_flg_sflags == NULL || _flg_sflagsc == 0) &&
        (_flg_bflags == NULL || _flg_bflagsc == 0))
    {
        fprintf(stderr, "No flags to parse.\n"
                        "  Did you forget to define any args?\n"
                        "  Or did you call flg_parse_flags twice?\n");
        exit(1);
    }

    /* Define the built-in help flag */
    bool *print_usage = flg_bool_arg("-h", "--help", "Print usage");

    unsigned int offset = 0;
    size_t i;
    /* Iterate over all [OPTIONS]... Start i=1 to exclude the original 
     * file name. Stop parsing of no arguments are found any more */
    for (i = 1; i < (size_t)argc; i++)
    {
        const char *arg = argv[i];
        
        /* check if the strings starts with a dash */
        if (strncmp("-", arg, 1) != 0) 
        { 
            break; /* Break no more options */
        }

        /* End of [OPTIONS]... */ 
        if (strcmp("--", arg) == 0)
        {
            break;
        }
        
        size_t j;
        /* Parse boolean flags */
        for (j = 0; j < _flg_bflagsc; j++)
        {
            if (_flg_bflags[j].flag != NULL && 
                strcmp(_flg_bflags[j].flag, arg) == 0)
            {   /* short form match */
                *_flg_bflags[j].value = true;
                offset = i;
            }
            if (_flg_bflags[j].long_form_flag != NULL && 
                strcmp(_flg_bflags[j].long_form_flag, arg) == 0)
            {   /* long form match */
                *_flg_bflags[j].value = true;
                offset = i;
            }
        }

        /* Parse int flags */
        for (j = 0; j < _flg_iflagsc; j++)
        {
            if ((_flg_iflags[j].flag != NULL && 
                strcmp(_flg_iflags[j].flag, arg) == 0) 
                ||
                (_flg_iflags[j].long_form_flag != NULL && 
                strcmp(_flg_iflags[j].long_form_flag, arg) == 0)
               )
            {
                /* short form or long form match */
                i++; /* retrieve the next arg */
                offset = i;
                if (i < (size_t)argc) 
                {   /* convert str to int */
                    *_flg_iflags[j].value = atoi(argv[i]);
                }
                else
                {
                    fprintf(stderr, "No value found for \"%s\"\n", arg);
                    *print_usage = true;
                    exit_code = 2;
                    goto break_outer;
                }
            }
        }

        /* Parse str flags */
        for (j = 0; j < _flg_sflagsc; j++)
        {
            if ((_flg_sflags[j].flag != NULL && 
                strcmp(_flg_sflags[j].flag, arg) == 0)
               ||
               (_flg_sflags[j].long_form_flag != NULL && 
                strcmp(_flg_sflags[j].long_form_flag, arg) == 0))
            { 
                /* short or long form match */
                i++; /* Retrieve the next argument */
                offset = i;
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
                    *print_usage = true;
                    exit_code = 2;
                    goto break_outer;
                }
            }
        }
        
        
        if (offset != i) /* arg was not recognised */
        {
            fprintf(stderr, "Did not recognize the option \"%s\"\n", arg);
            *print_usage = true;
            exit_code = 3;
        }
    }
    break_outer: /* exit from loop */

    /* Check whether we have the minimum of rest items in our collection */
    if (_flg_rest_col->minimum_amount > (argc - (offset + 1)) 
        && !*print_usage /* Except --help, we always want the help print
                          * to work without extra args */
        )
    {
        fprintf(stderr, "The minimum amount of requested [%s]... "
            "args was not found\n", _flg_rest_col->name);
        exit(7);
    }

    if (*print_usage)
    {
        flg_print_usage(argv[0]);
        for (i = 0; i < _flg_sflagsc; i++)
        {   /* De-alloc the strings that are otherwise the
            * caller's responsibility. Because we are exiting early
            * we need to free them ourselves. */
            free(*_flg_sflags[i].value);
            *_flg_sflags[i].value = NULL;
            free(_flg_sflags[i].value);
            _flg_sflags[i].value = NULL;
        }
        for (i = 0; i < _flg_bflagsc; i++) /* same for bool flags */
        {
            free(_flg_bflags[i].value);
            _flg_bflags[i].value = NULL;
        }
        for (i = 0; i < _flg_iflagsc; i++) /* same for int flags */
        {
            free(_flg_iflags[i].value);
            _flg_iflags[i].value = NULL;
        }
        _flg_free_mem(); /* Do regular clean-up before exit */
        exit(exit_code);
    }

    /* Clean-up, you ain't printing usage after this
     * and you won't be able to call flg_parse_flags again. */
    _flg_free_mem();

    free(print_usage); /* We created this for internal use, 
                        * we need to clean it up as such */

    return offset + 1; /* return index+1 of the last found flag */
}

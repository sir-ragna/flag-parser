
# flagparser

Somewhat inspired by the golang flags package. 
The idea is to have a minimal amount of effort to define cmdline 
arguments. Parsing is taken care of and some minimal usage is 
generated.

Steps.

- Define the cmdline args
- [optionally] define a rest collection
- Parse the flags. This will populate the pointers.


# Example code

Below is an example definition of each type of variable.

```c

#include "flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    /* Example of an integer */
    int *amount = flg_int_arg(
        "-a",                /* short flag format */
        "--amount",          /* long flag format */
        1,                   /* default value */
        "Specify the amount" /* Help string */
    );

    /* Example of a boolean argument, also called a switch
     * The defaul value of switches is always false */
    bool *keep = flg_bool_arg(
        "-k",             /* short flag format */
        "--keep",         /* long flag format */
        "Keep temp files" /* help string */
    );

    /* Example a string argument */
    char **dns = flg_string_arg(
        "-d",                           /* short flag format */
        "--dns",                        /* long flag format */
        "1.1.1.1",                      /* default value */
        "Specify the target DNS server" /* help string */
    );
    
    /* You can optionally define a 'rest collection'.
     * This will help the documentation string to name what you
     * expect to be present after the [OPTIONS]. */
    flg_define_rest_collection(
        "FILE",            /* Name of the collection.
                            * ./example [OPTIONS] [FILE]... */
        0,                 /* minimum amount of args required */
        "Files to process" /* Help string */
    );

    /* flg_parse_flags will populate the pointers 
     * It will also return an offset you can use to know when the 
     * [OPTIONS] and where the 'rest collection' starts. */
    int offset = flg_parse_flags(argc, argv);

    /* Optional demonstration on how to deal with the non-flag strings
     * arguments. The _offset_ variable allows us to iterate over them.
     */
    int i;
    for (i = offset; i < argc; i++)
    {
        printf("Rest ARG: %i \"%s\"\n", i - offset, argv[i]);
    }
    
    /* Demonstration on flag usage */
    if (*keep)
        puts("Keep is enabled");
    else
        puts("Keep is not enabled");
    
    printf("DNS server: \"%s\"\n", *dns);
    printf("Amount: %d\n", *amount);

    /* Clean-up */
    free(amount);
    free(keep);
    free(*dns);
    free(dns);

    return 0;
}
```

## Built-in `--help`

The `--help` flag is built-in and will list the cmdline 
arguments when called together with their help string.

![built-in help example](images/example-help.png)


# Code guidelines

- Compiles under **c89** and **C++17**. (run `make test`)
- Identifiers use snake case
- Uses the prefix `flg_` for public identifiers
- Uses the prefix `_flg_` for private identifiers

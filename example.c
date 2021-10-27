
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
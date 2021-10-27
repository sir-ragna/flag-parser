
#include "stdio.h"
#include "../flagparser.h"

int main(int argc, const char *argv[])
{
    /* At least the first or the second arg cannot be NULL */
    char **kernel = flg_string_arg(NULL, NULL, NULL, NULL);
    
    flg_parse_flags(argc, argv); /* Should exit */

    if (kernel != NULL)
    {
        if (*kernel != NULL)
        {
            free(*kernel);
            *kernel = NULL;
        }
        free(kernel);
        kernel = NULL;
    }

    return 0;
}
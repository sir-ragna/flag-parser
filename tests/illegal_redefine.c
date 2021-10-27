
#include "stdio.h"
#include "../flagparser.h"

int main(int argc, const char *argv[])
{
    bool *keep = flg_bool_arg("-k", "--keep", "keep files");
    char **kernel = flg_string_arg("-k", "--kernel", NULL, 
        "kernel options"); /* triggers collision */
    
    flg_parse_flags(argc, argv);
    
    if (*keep)
        puts("Keep original file");    
    
    /* clean-up */
    if (keep != NULL)
    {
        free(keep);
        keep = NULL;
    }

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
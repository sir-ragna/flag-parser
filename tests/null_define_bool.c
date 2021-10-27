
#include "stdio.h"
#include "../flagparser.h"

int main(int argc, const char *argv[])
{
    bool *keep = flg_bool_arg(NULL, NULL, NULL);
    
    flg_parse_flags(argc, argv);
    
    if (*keep)
        puts("Keep original file");    
    
    /* clean-up */
    if (keep != NULL)
    {
        free(keep);
        keep = NULL;
    }
    
    return 0;
}
#include "../flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    char *target = NULL;

    flg_str_var(&target           /* char *value */,  
               "--target"         /* char *name */, 
               "http://localhost" /* char *default_value */, 
               "Specify target"   /* char *help_str */);

    flg_parse_flags(argc, argv);

    /* your code */    
    printf("Target %s\n", target);

    /* Free the memory after you are done. */
    free(target);

    return 0;
}
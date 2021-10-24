#include "../flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    bool *warnings = (bool *)malloc(sizeof(bool));

    /* bool *warnings = NULL;
     * ^ This would fail. flg_bool_var needs allocated memory. */
    
    flg_bool_var(warnings          /* bool *value */, 
                 "-w"              /* char *name */, 
                 "Enable warnings" /* char *help_str */);

    flg_parse_flags(argc, argv);
    
    if (*warnings)
        puts("Warnings are enabled");

    free(warnings); /* Free your memory after you are ready */

    return 0;
}
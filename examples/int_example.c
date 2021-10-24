#include "../flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    int number = 0;
    
    flg_int_var(&number/* int *value */, 
                "-n"   /* char *name */, 
                2      /* int default_value */, 
                "number of passes" /* char * help string */);

    flg_parse_flags(argc, argv);
    
    printf("number: %i\n", number);

    return 0;
}
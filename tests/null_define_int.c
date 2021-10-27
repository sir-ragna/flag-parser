
#include "stdio.h"
#include "../flagparser.h"

int main(int argc, const char *argv[])
{
    int *amount = flg_int_arg(NULL, NULL, 0, NULL);
        
    flg_parse_flags(argc, argv); /* should exit program with 4 */
    
    if (amount)
        free(amount);

    return 0;
}
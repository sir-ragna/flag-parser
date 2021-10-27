
#include "stdio.h"
#include "../flagparser.h"

int main(int argc, const char *argv[])
{
    bool *keep = flg_bool_arg("-k", "--keep", "keep files");
    
    flg_parse_flags(argc, argv);
    flg_parse_flags(argc, argv);
    
    if (*keep)
        puts("Keep original file");    
    
    if (keep)
        free(keep);

    return 0;
}
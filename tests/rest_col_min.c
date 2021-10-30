
#include "../flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    int *amount = flg_int_arg("-a", "--amount", 1, "Specify the amount");
    char **dns = flg_string_arg("-d", "--dns", "1.1.1.1", "Specify the target DNS server");
    bool *keep = flg_bool_arg("-k", "--keep", "Keep temp files");
    
    /* Define a rest collection with 1 minmum value */
    flg_define_rest_collection("FILE", 1, "Files to process");

    int offset = flg_parse_flags(argc, argv);

    /* process the rest collection. (The arguments after the OPTIONS) */
    int i;
    for (i = offset; i < argc; i++)
    {
        printf("Rest ARG: %i \"%s\"\n", i - offset, argv[i]);
    }

    free(amount);
    free(keep);
    free(*dns);
    free(dns);

    return 0;
}
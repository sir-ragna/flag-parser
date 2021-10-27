
#include "flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    int *amount = flg_int_arg("-a", "--amount", 1, "Specify the amount");
    char **dns = flg_string_arg("-d", "--dns", "1.1.1.1", "Specify the target DNS server");
    bool *keep = flg_bool_arg("-k", "--keep", "Keep temp files");
    
    /* Optionally define the non-option args */
    flg_define_rest_collection("FILE", 0, "Files to process");

    int offset = flg_parse_flags(argc, argv);

    /* process the rest collection. (The arguments after the OPTIONS) */
    int i;
    for (i = offset; i < argc; i++)
    {
        printf("Rest ARG: %i \"%s\"\n", i - offset, argv[i]);
    }
    
    if (*keep)
        printf("Keep is enabled ");
    printf("DNS server \"%s\", amount: %d\n", *dns, *amount);

    /* You still need to clean up your own pointers 
     * The reason I am not putting this in a separate clean up func
     * is because then you would NEED to call that function to free
     * this memory. This seems non-obvious. This way the API remains
     * minimal. You define the args, you call parseFlags and afterwards
     * you are responsible for the cleanup yourself. */
    free(amount);
    free(keep);
    free(*dns);
    free(dns);

    return 0;
}
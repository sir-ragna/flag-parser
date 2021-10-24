
#include "flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    int amount;
    int runners;
    char *target;
    char *test;
    char *abc;
    bool enable_hardmode;
    bool keep_temp_files;
    flg_bool_var(&enable_hardmode, "--hard", "Enable HM!");
    flg_bool_var(&keep_temp_files, "-k", "Keep tmp files");
    flg_str_var(&target, "-t", "http://localhost", "Specify target");
    flg_str_var(&test, "-z", NULL, "test");
    flg_str_var(&abc, "-a", "", "abc");
    flg_int_var(&amount, "-n", 10, "Amount of iterations");
    flg_int_var(&runners, "-r", 5, "Runners to start");
    flg_parse_flags(argc, argv);
    
    if (enable_hardmode)
        puts("HARD MODE enabled");
    if (keep_temp_files)
        puts("Keep temp files");
    
    printf("Test %s\n", test);
    printf("Abc %s\n", abc);
    printf("Target %s\n", target);
    printf("Amount %d\n", amount);
    printf("Runners %d\n", runners);

    /* You still need to clean up your own pointers 
     * The reason I am not putting this in a separate clean up func
     * is because then you would NEED to call that function to free
     * this memory. This seems non-obvious. This way the API remains
     * minimal. You define the args, you call parseFlags and afterwards
     * you are responsible for the cleanup yourself. */
    if (target)
        free(target);
    if (test)
        free(test);
    if (abc)
        free(abc);

    return 0;
}
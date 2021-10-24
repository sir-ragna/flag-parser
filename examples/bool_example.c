
#include "../flagparser.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
    bool enable_rw;
    bool keep;
    bool version;
    
    flg_bool_var(&enable_rw, "-e",          "Enable Read-Write");
    flg_bool_var(&enable_rw, "--enable-rw", "Enable Read-Write");
    flg_bool_var(&keep,      "-k",          "Keep original files");
    flg_bool_var(&version,   "--version",   "Print version");

    flg_parse_flags(argc, argv);
    
    if (enable_rw)
        puts("rw enabled");
    if (keep)
        puts("Keep original file");    
    if (version)
        printf("%s %s\n", argv[0], "v0.1");

    return 0;
}
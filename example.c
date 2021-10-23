
#include "flagparser.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    int amount;
    int runners;
    char *target;
    char *test;
    char *abc;
    parseStr(&target, "-t", "http://localhost", "Specify target");
    parseStr(&test, "-z", NULL, "test");
    parseStr(&abc, "-a", "", "abc");
    parseInt(&amount, "-n", 10, "Amount of iterations");
    parseInt(&runners, "-r", 5, "Runners to start");
    parseFlags(argc, argv);

    printf("Test %s\n", test);
    printf("Abc %s\n", abc);
    printf("Target %s\n", target);
    printf("Amount %d\n", amount);
    printf("Runners %d\n", runners);
    return 0;
}
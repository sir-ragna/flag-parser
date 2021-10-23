
#include "flagparser.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    initFlagParser(argc, argv);
    int amount;
    int runners;
    parseInt(&amount, "-n", 10, "Amount of iterations.");
    parseInt(&runners, "-r", 5, "Runners to start");
    parseFlags();
    printf("Amount %d\n", amount);
    printf("Runners %d\n", runners);
    return 0;
}
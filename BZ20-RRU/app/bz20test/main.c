#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include "rru_runtest.h"

int main(int argc, char *argv[])
{
    rru_run_test();
    WaitForCommand();
    return 0;
}

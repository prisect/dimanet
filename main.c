#include <stdio.h>
#include "strlib.h"

String *myexample;

int main()
{
    myexample = (String *)calloc(1, sizeof(String));
    *myexample = init_string("Hi. Something.");

    printf("%s\n", myexample->str); // print

    deinit_string(myexample);
    free(myexample);
    return 0;
}

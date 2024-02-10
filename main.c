#include <stdio.h>
#include "strlib.h"


String *string_example;

int main()
{
    string_example = (struct String *)calloc(1, sizeof (struct String));
    *string_example = init_string("Hi. Smth.");

    deinit_string(string_example);
    free(string_example);
    return 0;
}

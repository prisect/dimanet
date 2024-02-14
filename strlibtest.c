#include <stdio.h>
#include "strlibtest.h"
#include "strlib.h"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"

int main()
{
    t_str_cat();
    t_str_substring();
    t_str_length();
    t_str_split();
    t_strchr();
    t_strrchr();
    t_str_compare();
}

void t_str_cat()
{
    String* string_ptr = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr = init_string("Hello");

    String* string_ptr2 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr2 = init_string("World");

    String* string_ptr3 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr3  = init_string("HelloWorld");

    String w = str_cat(string_ptr->str, string_ptr2->str);

    if(str_compare(w, string_ptr3) == 0 && w.size == 10)
        printf(GRN"str_cat 1 PASSED\n"RESET);
    else
    {
        printf(RED"str_cat 1 NOT_PASSED\n"RESET);
    }

    deinit_string(&w);
    deinit_string(string_ptr2);
    free(string_ptr2);
    deinit_string(string_ptr3);
    free(string_ptr3);
    deinit_string(string_ptr);
    free(string_ptr);
}

void t_str_substring()
{
    String* string_ptr = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr  = init_string("HelloWorld");

    String actual_substring0_5 = init_string("Hello");
    String actual_substring5_10 = init_string("World");
    String actual_substring0_10 = init_string("HelloWorld");
    String actual_substring10_0 = {0, 0};

    String substring0_5 = str_substring(0, 5, string_ptr);
    String substring5_10 = str_substring(5, 10, string_ptr);
    String substring0_10 = str_substring(0, 10, string_ptr);
    String substring10_0 = str_substring(10, 0, string_ptr);

    if(str_compare(actual_substring0_5, substring0_5) == 0)
        printf(GRN"str_substring 1 \tPASSED\n"RESET);
    else
        printf(RED"str_substring 1 \tNOT_PASSED\n"RESET);

    if(str_compare(actual_substring5_10, substring5_10) == 0)
        printf(GRN"str_substring 2 \tPASSED\n"RESET);
    else
        printf(RED"str_substring 2 \tNOT_PASSED\n"RESET);
    
    if(str_compare(actual_substring0_10, substring0_10) == 0)
        printf(GRN"str_substring 3 \tPASSED\n"RESET);
    else
        printf(RED"str_substring 3 \tNOT_PASSED\n"RESET);
    
    if(str_compare(actual_substring10_0, substring10_0) == 0)
        printf(GRN"str_substring 4 \tPASSED\n"RESET);
    else
        printf(RED"str_substring 4 \tNOT_PASSED\n"RESET);
}

void t_str_length()
{
    String* string_ptr1 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr1 = init_string("Hello");

    String* string_ptr2 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr2 = init_string("World!!");

    String* string_ptr3 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr3 = init_string("HelloWorld!");

    String* string_ptr4 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr4  = init_string("Hd");

    String* string_ptr5 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr5  = init_string("d");

    String* string_ptr6 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr6  = init_string("");

    int actual_len1 = 5;
    int actual_len2 = 7;
    int actual_len3 = 11;
    int actual_len4 = 2;
    int actual_len5 = 1;
    int actual_len6 = 0;

    if (str_length(string_ptr1) == actual_len1)
        printf(GRN"str_length 1 \tPASSED\n"RESET);
    else
        printf(RED"str_length 1 \tNOT_PASSED\n"RESET);
    if (str_length(string_ptr2) == actual_len2)
        printf(GRN"str_length 2 \tPASSED\n"RESET);
    else
        printf(RED"str_length 2 \tNOT_PASSED\n"RESET);
    if (str_length(string_ptr3) == actual_len3)
        printf(GRN"str_length 3 \tPASSED\n"RESET);
    else
        printf(RED"str_length 3 \tNOT_PASSED\n"RESET);
    if (str_length(string_ptr4) == actual_len4)
        printf(GRN"str_length 4 \tPASSED\n"RESET);
    else
        printf(RED"str_length 4 \tNOT_PASSED\n"RESET);
    if (str_length(string_ptr5) == actual_len5)
        printf(GRN"str_length 5 \tPASSED\n"RESET);
    else
        printf(RED"str_length 5 \tNOT_PASSED\n"RESET);
    if (str_length(string_ptr6) == actual_len6)
        printf(GRN"str_length 6 \tPASSED\n"RESET);
    else
        printf(RED"str_length 6 \tNOT_PASSED\n"RESET);
}

void t_str_split()
{
    String* string_ptr = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr = init_string("Hello.World*Next.Another");

    String actual_split[4];
    actual_split[0] = init_string("Hello");
    actual_split[1] = init_string("World");
    actual_split[2] = init_string("Next");
    actual_split[3] = init_string("Another");

    int len = 0;
    String* split = str_split(string_ptr, ".*", &len);

    for (int i = 0; i < 4; i++)
    {
        if (str_compare(actual_split[i], split[i]) == 0)
            printf(GRN"str_split %d \tPASSED\n" RESET, i);
        else
            printf(RED"str_split %d \tNOT_PASSED\n" RESET, i);
    }
}

void t_strchr()
{
    String* string_ptr = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr = init_string("Hello.World*Next.Another");

    int to_find = '.';
    int actual_index = 5;

    char *found = str_strchr(string_ptr, to_find);

    if (found == string_ptr->str + actual_index)
        printf(GRN "strchr \tPASSED\n"RESET);
    else
        printf(RED"strchr \tNOT_PASSED\n"RESET);
}

void t_strrchr()
{
    String* string_ptr = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr = init_string("Hello.World*Next.Another");

    int to_find = '.';
    int actual_index = 16;

    char *found = str_strrchr(string_ptr, to_find);

    if (found == string_ptr->str + actual_index)
        printf(GRN "strrchr \tPASSED\n"RESET);
    else
        printf(RED"strrchr \tNOT_PASSED\n"RESET);
}

void t_str_compare()
{
    String* string_ptr1 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr1 = init_string("Hello");

    String* string_ptr2 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr2 = init_string("World");

    String* string_ptr3 = (struct String *)calloc(1, sizeof (struct String));
    *string_ptr3 = init_string("Hello");

    if (str_compare(string_ptr1, string_ptr2) < 0)
        printf(GRN"str_compare 1 \tPASSED\n"RESET);
    else
        printf(RED"str_compare 1 \tNOT_PASSED\n"RESET);

    if (str_compare(string_ptr1, string_ptr3) == 0)
        printf(GRN"str_compare 2 \tPASSED\n"RESET);
    else
        printf(RED"str_compare 2 \tNOT_PASSED\n"RESET);
}

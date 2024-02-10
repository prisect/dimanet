#include "strlib.h"
#include <stdio.h>
String init_string(char *str)
{
    String result;
    result.size = str_length(str);
    result.str = (char *)calloc(result.size + 1, sizeof (char));
    for (size_t i = 0; i < result.size; ++i)
    {
        result.str[i] = str[i];
    }
    return result;
}

int deinit_string(String *string)
{
    if (!string)
        return -1;

    free(string->str);
    string->str = NULL;
    string->size = 0;
    return 0;
}


// T = {String, String*, char*}
// String get_string(T string)
// Returns custom string

String _str__get_string(String string)
{
    return string;
}

String _str_ptr__get_string(String *string)
{
    return *string;
}

String _str_char__get_string(char *str)
{
    String string = {0, 0};
    string.size = str_length(str);
    string.str = str;
    return string;
}

/*
#define get_string(T)                   \
    _Generic((T),                       \
    String: _str__get_string,            \
    String*: _str_ptr__get_string,       \
    char*: _str_char__get_string   \
)(T)
*/



size_t _str__str_length(String string)
{
    return string.size;
}

size_t _str_ptr__str_length(String *string)
{
    return string->size;
}

size_t _str_char__str_length(char* string)
{
    size_t pos = 0;
    while (string[pos] != '\0')
    {
        ++pos;
    }
    return pos;
}

/*
#define str_length(T)              \
    _Generic((T),                  \
    String: _str__str_length,       \
    String*: _str_ptr__str_length,  \
    char*: _str_char__str_length    \
)(T)
*/


String _str__str_substring(int start, int end, String initial_str)
{
    String result_str = {0, 0};
    if (start < 0 || end > initial_str.size || start >= end)
        return result_str;

    int new_size = end - start;
    result_str.str = (char *)calloc(new_size + 1, sizeof (char));
    for (int i = 0; i < new_size; ++i)
    {
        result_str.str[i] = initial_str.str[i + start];
    }
    result_str.size = new_size;
    return result_str;
}

String _str_ptr__str_substring(int start, int end, String* initial_str)
{
    return _str__str_substring(start, end, get_string(initial_str));
}

String _str_char__substring(int start, int end, char* initial_str)
{
    return _str__str_substring(start, end, get_string(initial_str));
}

/*
#define str_length(T)                 \
    _Generic((T),                     \
    String: _str__str_substring,      \
    String*: _str_ptr__str_substring, \
    char*: _str_char__substring       \
)(T)
*/
String _str_str__str_cat(String string1, String string2)
{
    return _str_char_str_char__str_cat(string1.str, string2.str);
}
String _str_str_char__str_cat(String string1, char *string2)
{
    return _str_char_str_char__str_cat(string1.str, string2);
}
String _str_str_ptr__str_cat(String string1, String* string2)
{
    return _str_char_str_char__str_cat(string1.str, string2->str);
}
String _str_ptr_str__str_cat(String* string1, String string2)
{
    return _str_char_str_char__str_cat(string1->str, string2.str);
}
String _str_ptr_str_char__str_cat(String* string1, char *string2)
{
    return _str_char_str_char__str_cat(string1->str, string2);
}
String _str_ptr_str_ptr__str_cat(String* string1, String* string2)
{
    return _str_char_str_char__str_cat(string1->str, string2->str);
}
String _str_char_str_ptr__str_cat(char *string1, String* string2)
{
    return _str_char_str_char__str_cat(string1, string2->str);
}
String _str_char_str__str_cat(char *string1, String string2)
{
    return _str_char_str_char__str_cat(string1, string2.str);
}

String _str_char_str_char__str_cat(char *string1, char *string2)
{
    String result = {str_length(string1) + str_length(string2), 0};
    result.str = (char *)calloc(result.size + 1, sizeof (char));
    for (int i = 0; i < str_length(string1); ++i)
    {
        result.str[i] = string1[i];
    }
    for (int j = 0; j < str_length(string2); ++j)
    {
        result.str[str_length(string1) + j] = string2[j];
    }
    result.str[result.size] = '\0';
    return result;
}

int _str_str__str_compare(String string1, String string2)
{
    return _str_char_str_char__str_compare(string1.str, string2.str);
}

int _str_ptr_str_ptr__str_compare(String *string1, String *string2)
{
    return _str_char_str_char__str_compare(string1->str, string2->str);
}

int _str_str_ptr__str_compare(String string1, String *string2)
{
    return _str_char_str_char__str_compare(string1.str, string2->str);
}

int _str_ptr_str__str_compare(String *string1, String string2)
{
    return _str_char_str_char__str_compare(string1->str, string2.str);
}

int _str_ptr_str_char__str_compare(String *string1, char *string2)
{
    return _str_char_str_char__str_compare(string1->str, string2);
}

int _str_str_char__str_compare(String string1, char *string2)
{
    return _str_char_str_char__str_compare(string1.str, string2);
}

int _str_char_str__str_compare(char *string1, String string2)
{
    return _str_char_str_char__str_compare(string1, string2.str);
}

int _str_char_str_ptr__str_compare(char *string1, String *string2)
{
    return _str_char_str_char__str_compare(string1, string2->str);
}

int _str_char_str_char__str_compare(char* s1, char* s2)
{
    if(s1 == NULL && s2 == NULL)
        return 0;
    while(*s1 && (*s1 == *s2))
    {
        if (*s1 != *s2)
            break;
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


char* _str__strchr(String string, int ch)
{
    return _str_char__strchr(string.str, ch);
}

char* _str_ptr__strchr(String *string, int ch)
{
    return _str_char__strchr(string->str, ch);
}

char* _str_char__strchr(char *string, int ch)
{
    int pos = 0;
    while(string[pos] != ch)
    {
        if (++pos == str_length(string))
            break;
    }

    if (pos == str_length(string))
        return NULL;

    return string + pos;
}

char* _str__strrchr(String string, int ch)
{
    return _str_char__strrchr(string.str, ch);
}

char* _str_ptr__strrchr(String *string, int ch)
{
    return _str_char__strrchr(string->str, ch);
}

char* _str_char__strrchr(char *string, int ch)
{
    int pos = str_length(string);
    while(string[pos] != ch)
    {
        if (--pos == -1)
            break;
    }
    return string + pos;
}

unsigned int is_delim(char c, char *delim)
{
    while(*delim != '\0')
    {
        if(c == *delim)
            return 1;
        delim++;
    }
    return 0;
}

char* str_tok(char *srcString, char *delim)
{
    static char *backup_string;
    if(!srcString)
    {
        srcString = backup_string;
    }
    if(!srcString)
    {
        return NULL;
    }

    while(1)
    {
        if(is_delim(*srcString, delim))
        {
            srcString++;
            continue;
        }
        if(*srcString == '\0')
        {
            return NULL; 
        }
        break;
    }
    char *ret = srcString;
    while(1)
    {
        if(*srcString == '\0')
        {
            backup_string = srcString;
            return ret;
        }
        if(is_delim(*srcString, delim))
        {
            *srcString = '\0';
            backup_string = srcString + 1;
            return ret;
        }
        srcString++;
    }
}

String* _str_str__str_split(String string, String delim, int* len)
{
    return _str_char_str_char__str_split(string.str, delim.str, len);
}
String* _str_str_ptr__str_split(String  string, String* delim, int* len)
{
    return _str_char_str_char__str_split(string.str, delim->str, len);
}
String* _str_str_char__str_split(String string, char* delim, int* len)
{
    return _str_char_str_char__str_split(string.str, delim, len);
}
String* _str_ptr_str__str_split(String* string, String delim, int* len)
{
    return _str_char_str_char__str_split(string->str, delim.str, len);
}
String* _str_ptr_str_ptr__str_split(String* string, String* delim, int* len)
{
    return _str_char_str_char__str_split(string->str, delim->str, len);
}
String* _str_ptr_str_char__str_split(String* string, char* delim, int* len)
{
    return _str_char_str_char__str_split(string->str, delim, len);
}
String* _str_char_str__str_split(char* string, String delim, int* len)
{
    return _str_char_str_char__str_split(string, delim.str, len);
}
String* _str_char_str_ptr__str_split(char* string, String* delim, int* len)
{
    return _str_char_str_char__str_split(string, delim->str, len);
}
String* _str_char_str_char__str_split(char* string, char* delim, int* len)
{
    String* tokens = (String *)(malloc(*len * sizeof(String)));
    char *token = str_tok(string, delim);
    while (token)
    {
        (*len)++;
        tokens = (String *)realloc(tokens, *len * sizeof(String));
        tokens[*len - 1].str = token;
        token = str_tok(NULL, delim);
    }
    return tokens;
}

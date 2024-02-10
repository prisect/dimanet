#ifndef strlib_H
#define strlib_H

#include <stdlib.h>

typedef struct String{
    size_t size;
    char *str;
} String;

String init_string(char *str);
int deinit_string(String *string);

String _str__get_string(String string);
String _str_ptr__get_string(String *string);
String _str_char__get_string(char *str);

#define get_string(T)                    \
    _Generic((T),                        \
    String: _str__get_string,            \
    String*: _str_ptr__get_string,       \
    char*: _str_char__get_string         \
)(T)

size_t _str__str_length(String string);
size_t _str_ptr__str_length(String *string);
size_t _str_char__str_length(char* string);

#define str_length(T)              \
    _Generic((T),                  \
    String: _str__str_length,       \
    String*: _str_ptr__str_length,  \
    char*: _str_char__str_length    \
)(T)

String _str__str_substring(int start, int end, String initial_str);
String _str_ptr__str_substring(int  start, int end, String* initial_str);
String _str_char__substring(int start, int end, char* initial_str);

#define _str__str_substring_tmp2(X, Y, Z) _Generic((X),\
    int: _str__str_substring)

#define _str__str_substring_tmp(X, Y, Z) _Generic((Y),\
    int: _str__str_substring_tmp2(X, Y, Z))

#define _str_ptr__str_substring_tmp2(X, Y, Z) _Generic((X),\
    int: _str_ptr__str_substring)

#define _str_ptr__str_substring_tmp(X, Y, Z) _Generic((Y),\
    int: _str_ptr__str_substring_tmp2(X, Y, Z))

#define _str_char__substring_tmp2(X, Y, Z) _Generic((X),\
    int: _str_char__substring)

#define _str_char__substring_tmp(X, Y, Z) _Generic((Y),\
    int: _str_char__substring_tmp2(X, Y, Z))

#define str_substring(X, Y, Z)                              \
    _Generic((Z),                                           \
    String: _str__str_substring_tmp(X, Y, Z),               \
    String*: _str_ptr__str_substring_tmp(X, Y, Z),          \
    char*: _str_char__substring_tmp(X, Y, Z)                \
)(X, Y, Z)

String _str_str__str_cat(String string1, String string2);
String _str_str_char__str_cat(String string1, char *string2);
String _str_str_ptr__str_cat(String string1, String* string2);
String _str_ptr_str__str_cat(String* string1, String string2);
String _str_ptr_str_char__str_cat(String* string1, char *string2);
String _str_ptr_str_ptr__str_cat(String* string1, String* string2);
String _str_char_str_ptr__str_cat(char *string1, String* string2);
String _str_char_str__str_cat(char *string1, String string2);
String _str_char_str_char__str_cat(char *string1, char *string2);

#define _str__str_cat(X, Y) _Generic((Y),\
    String:  _str_str__str_cat,\
    String*: _str_str_ptr__str_cat,\
    char*: _str_str_char__str_cat\
)
  
#define _str_ptr__str_cat(X, Y) _Generic((Y),\
    String:  _str_ptr_str__str_cat,\
    String*: _str_ptr_str_ptr__str_cat,\
    char*: _str_ptr_str_char__str_cat\
)

#define _str_char__str_cat(X, Y) _Generic((Y),\
    String:  _str_char_str__str_cat,\
    String*: _str_char_str_ptr__str_cat,\
    char*: _str_char_str_char__str_cat\
)
  
#define str_cat(X, Y) _Generic((X),\
    String:  _str__str_cat(X, Y),\
    String*: _str_ptr__str_cat (X, Y),\
    char*: _str_char__str_cat(X, Y)\
)(X, Y)

int _str_str__str_compare(String string1, String string2);
int _str_ptr_str_ptr__str_compare(String *string1, String *string2);
int _str_str_ptr__str_compare(String string1, String *string2);
int _str_ptr_str__str_compare(String *string1, String string2);
int _str_ptr_str_char__str_compare(String *string1, char *string2);
int _str_str_char__str_compare(String string1, char *string2);
int _str_char_str__str_compare(char *string1, String string2);
int _str_char_str_ptr__str_compare(char *string1, String *string2);
int _str_char_str_char__str_compare(char* s1, char* s2);

#define _str__str_compare(X, Y) _Generic((Y),\
    String:  _str_str__str_compare,\
    String*: _str_str_ptr__str_compare,\
    char*: _str_str_char__str_compare\
)
  
#define _str_ptr__str_compare(X, Y) _Generic((Y),\
    String:  _str_ptr_str__str_compare,\
    String*: _str_ptr_str_ptr__str_compare,\
    char*: _str_ptr_str_char__str_compare\
)

#define _str_char__str_compare(X, Y) _Generic((Y),\
    String:  _str_char_str__str_compare,\
    String*: _str_char_str_ptr__str_compare,\
    char*: _str_char_str_char__str_compare\
)
  
#define str_compare(X, Y) _Generic((X),\
    String:  _str__str_compare(X, Y),\
    String*: _str_ptr__str_compare (X, Y),\
    char*: _str_char__str_compare(X, Y)\
)(X, Y)

char* _str__strchr(String string, int ch);
char* _str_ptr__strchr(String *string, int ch);
char* _str_char__strchr(char *string, int ch);

#define _str__str_strchr_tmp(X, Y) _Generic((Y),\
    int:  _str__strchr\
)
  
#define _str_ptr__str_strchr_tmp(X, Y) _Generic((Y),\
    int: _str_ptr__strchr\
)

#define _str_char__str_strchr_tmp(X, Y) _Generic((Y),\
    int: _str_char__strchr\
)
  
#define str_strchr(X, Y) _Generic((X),\
    String:  _str__str_strchr_tmp(X, Y),\
    String*: _str_ptr__str_strchr_tmp(X, Y),\
    char*: _str_char__str_strchr_tmp(X, Y)\
)(X, Y)

char* _str__strrchr(String string, int ch);
char* _str_ptr__strrchr(String *string, int ch);
char* _str_char__strrchr(char *string, int ch);

#define _str__str_strrchr_tmp(X, Y) _Generic((Y),\
    int:  _str__strrchr\
)
  
#define _str_ptr__str_strrchr_tmp(X, Y) _Generic((Y),\
    int: _str_ptr__strrchr\
)

#define _str_char__str_strrchr_tmp(X, Y) _Generic((Y),\
    int: _str_char__strrchr\
)
  
#define str_strrchr(X, Y) _Generic((X),\
    String:  _str__str_strrchr_tmp(X, Y),\
    String*: _str_ptr__str_strrchr_tmp(X, Y),\
    char*: _str_char__str_strrchr_tmp(X, Y)\
)(X, Y)

unsigned int is_delim(char c, char *delim);
char* str_tok(char *srcString, char *delim);

String* _str_str__str_split(String string, String delim, int* len);
String* _str_str_ptr__str_split(String  string, String* delim, int* len);
String* _str_str_char__str_split(String string, char* delim, int* len);
String* _str_ptr_str__str_split(String* string, String delim, int* len);
String* _str_ptr_str_ptr__str_split(String* string, String* delim, int* len);
String* _str_ptr_str_char__str_split(String* string, char* delim, int* len);
String* _str_char_str__str_split(char* string, String delim, int* len);
String* _str_char_str_ptr__str_split(char* string, String* delim, int* len);
String* _str_char_str_char__str_split(char* string, char* delim, int* len);

#define _str__str_split(X, Y, Z) _Generic((Y),  \
    String: _str_str__str_split,                \
    String*: _str_str_ptr__str_split,           \
    char*: _str_str_char__str_split             \
)

#define _str_ptr__str_split(X, Y, Z) _Generic((Y),  \
    String: _str_ptr_str__str_split,                \
    String*: _str_ptr_str_ptr__str_split,           \
    char*: _str_ptr_str_char__str_split             \
)

#define _str_char__str_split(X, Y, Z) _Generic((Y), \
    String: _str_char_str__str_split,               \
    String*: _str_char_str_ptr__str_split,          \
    char*: _str_char_str_char__str_split            \
)

#define _str_split_tmp(X, Y, Z) _Generic((X),       \
    String: _str__str_split(X, Y, Z),               \
    String*: _str_ptr__str_split(X, Y, Z),          \
    char*: _str_char__str_split(X, Y, Z)            \
)

#define str_split(X, Y, Z)                          \
    _Generic((Z),                                   \
    int*: _str_split_tmp(X, Y, Z)                   \
)(X, Y, Z)

#endif

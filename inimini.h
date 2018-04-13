#ifndef INIMINI_LIBRARY_H
#define INIMINI_LIBRARY_H

#define BUFFER_SIZE 128
#define INIMINI_KEY_LENGTH 16
#define INIMINI_VAL_LENGTH 64
#define INIMINI_COMMENT_PREFIX ";;"

typedef struct __iniminientry inientry;

struct __iniminientry {
    char *key; // DOH! 16 CHARACTERS LONG EXTENSIONS :D
    char *value; // well, maybe ?
    inientry *next; // linked lists
};

void ini_parse_file(char *, inientry **); // filename, head of list

void _ini_parse_line(char *, inientry *); // line: in, inientry: out

void ini_destroy(inientry *); // head of list

void ini_get(inientry *, char *, char **); // head in, ext in, value out

void remove_blank(char *); // str

void error(char *); // msg,

#endif
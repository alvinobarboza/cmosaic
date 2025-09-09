#pragma once

#define MAX_SOURCES 9
#define MAX_CHAR_LENGTH 255
#define NOT_LOADED "NOT LOADED"

enum MType {
    T_NULL = 0,
    T_1X1,
    T_2X2,
    T_3X3
};

enum ErrType {
    NO_ERR = 0,
    ERR_NO_FILE,
    ERR_FILE_TOO_BIG,
    ERR_PARSING,
    ERR_GETTING_TYPE,
    ERR_WRONG_TYPE
};

typedef struct Sources {
    char name[MAX_CHAR_LENGTH];
    char source[MAX_CHAR_LENGTH];
} Sources;

typedef struct ConfigFile {
    enum MType type;
    enum ErrType err;
    Sources sources[MAX_SOURCES];
} ConfigFile;

ConfigFile *read_config();

void configfile_source_strcpy(Sources *s, const char *name, const char *source);
void print_error_hint(enum ErrType type);
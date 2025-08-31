#pragma once

#include <stdint.h>

#define MAX_SOURCES 9

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
    char *name;
    char *source;
} Sources;

typedef struct ConfigFile {
    enum MType type;
    enum ErrType err;
    Sources sources[MAX_SOURCES];
} ConfigFile;

ConfigFile *read_config();
void print_error_hint(enum ErrType type); // TODO
#pragma once

#include <stdint.h>

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
    Sources sources[T_3X3*T_3X3];
} ConfigFile;

ConfigFile *read_config();
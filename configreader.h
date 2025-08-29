#pragma once

#include <stdint.h>

enum MType {
    T_NULL,
    T_1X1,
    T_2X2,
    T_3X3
};

enum ErrType {
    NO_ERR,
    ERR_NO_FILE,
    ERR_FILE_TOO_BIG,
    ERR_GETTING_TYPE
};

typedef struct Sources {
    char *name;
    char *source;
} Sources;

typedef struct ConfigFile {
    enum MType type;
    enum ErrType err;
    Sources sources[9];
} ConfigFile;

ConfigFile *read_config();
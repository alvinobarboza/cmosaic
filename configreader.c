#include "configreader.h"
#include "include/toml-c/toml-c.h"

#define MAX_FILE_SIZE_BYTES 2000

long get_file_size(const char* filename) {
    FILE* file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

ConfigFile *read_config() {
    char file_name[] = "config.toml";
    
    ConfigFile *cf = malloc(sizeof(ConfigFile));
    
    if (get_file_size(file_name) > MAX_FILE_SIZE_BYTES){
        cf->type = T_NULL;
        cf->err = ERR_FILE_TOO_BIG;
        return cf;
    }

    char errbuf[200];

    FILE *fptr;

    fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        cf->type = T_NULL;
        cf->err = ERR_NO_FILE;
        return cf;
    }
    
    toml_table_t *data = toml_parse_file(fptr, errbuf, sizeof(errbuf));
    if (!data){
        printf("%s\n\n", errbuf);
        fclose(fptr);
        return cf;
    }

    toml_value_t type = toml_table_int(data, "type");
    if (!type.ok) {
        cf->type = T_NULL;
        cf->err = ERR_GETTING_TYPE;
        
        fclose(fptr);
        toml_free(data);
        return cf;
    }

    if (type.u.i < T_1X1 || type.u.i > T_3X3) {
        cf->type = T_NULL;
        cf->err = ERR_GETTING_TYPE;
        
        fclose(fptr);
        toml_free(data);
        return cf;
    }

    cf->type = type.u.i;

    fclose(fptr);
    toml_free(data);

    return cf;
}
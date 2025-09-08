#include <stdint.h>

#include "configreader.h"
#include "include/toml-c/toml-c.h"

#define MAX_FILE_SIZE_BYTES 2000
#define NOT_FOUND -1

long get_file_size(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

void configfile_source_strcpy(Sources *s, const char *name, const char *source) {
    snprintf(s->name,MAX_CHAR_LENGTH, "%s",name);
    snprintf(s->source,MAX_CHAR_LENGTH,"%s",source);
}

ConfigFile *read_config() {
    char file_name[] = "config.toml";
    
    ConfigFile *cf = malloc(sizeof(ConfigFile));

    long file_check = get_file_size(file_name);

    if (file_check == NOT_FOUND) {
        cf->type = T_NULL;
        cf->err = ERR_NO_FILE;
        return cf;
    }
    
    if (file_check > MAX_FILE_SIZE_BYTES){
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
        cf->type = T_NULL;
        cf->err = ERR_PARSING;
        
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
        cf->err = ERR_WRONG_TYPE;
        
        fclose(fptr);
        toml_free(data);
        return cf;
    }

    cf->type = type.u.i;

    uint8_t total_videos = cf->type * cf->type;

    toml_array_t *videos = toml_table_array(data, "videos");

    uint8_t length = toml_array_len(videos);
    uint8_t t_loaded;
	for (t_loaded = 0; t_loaded < length; t_loaded++) {
        if (t_loaded == total_videos) {
            break;
        }

		toml_table_t *t = toml_array_table(videos, t_loaded);
		toml_value_t name = toml_table_string(t, "name");
		toml_value_t source = toml_table_string(t, "source");

        configfile_source_strcpy(
            &cf->sources[t_loaded], 
            name.ok ? name.u.s : "Error", 
            source.ok ? source.u.s : ""
        );

        if (name.ok) free(name.u.s);
        if (source.ok) free(source.u.s);
        
	}

    if (t_loaded < total_videos) {
        for(uint8_t i = t_loaded; i < total_videos; i++) {
            configfile_source_strcpy(&cf->sources[i], NOT_LOADED, NOT_LOADED);
        }
    }

    fclose(fptr);
    toml_free(data);

    cf->err = NO_ERR;
    return cf;
}
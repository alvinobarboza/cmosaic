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

    uint8_t total_videos = cf->type * cf->type;

    toml_array_t *videos = toml_table_array(data, "videos");

    int length = toml_array_len(videos);
	for (int i = 0; i < length; i++) {
        if (length == total_videos) {
            break;
        }

		toml_table_t *t = toml_array_table(videos, i);
		toml_value_t name = toml_table_string(t, "name");
		toml_value_t source = toml_table_string(t, "source");

        cf->sources[i].name = name.ok ? name.u.s : "Error";
        cf->sources[i].source = source.ok ? source.u.s : "";
	}

    for(uint8_t i = 0; i < total_videos; i++) {
        if (cf->sources[i].name == NULL){
            cf->sources[i].name = "NOT LOADED";
            cf->sources[i].source = "";
        }
    }

    fclose(fptr);
    toml_free(data);

    return cf;
}

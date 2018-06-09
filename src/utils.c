#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int parse_url_data(char *line, char *url, char *ch_id) {
    if (line == NULL || url == NULL || ch_id == NULL) return get_url_null_ptr;

    strcpy(ch_id, strtok(line, " "));
    strcpy(url,   strtok(NULL, " "));
    
    return 0;
}
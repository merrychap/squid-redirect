#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

typedef enum {
    get_url_null_ptr = -1
} utils_error_t;

int parse_url_data(char *line, char *url, char *ch_id);

#endif
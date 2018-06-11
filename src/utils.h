#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#include "hashtable.h"
#include "rewriter.h"

typedef enum {
    get_url_null_ptr    = -1,
    realloc_failed      = -2,
    failed_parse_json   = -3,
    top_elem_not_object = -4,
    open_file_error     = -5
} utils_error_t;

int   parse_url_data(char *line, char *url, char *ch_id);
void  reduce_https(char *url, char *new_url);

int json_load_file(rewriter_t *rw, char *filename);
int get_pair(jsmntok_t *tokens, char *content, size_t index, char *key, char *value);

/* Read file content into content variable.
 * It reallocates content dynamically. After reading
 * returns exact size of readed content
 */
int read_file(char *filename, char **content, size_t *content_size);

#endif
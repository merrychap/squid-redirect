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
    open_file_error     = -5,
    url_parse_error     = -6
} utils_error_t;

int parse_url_data(char *line, char *url, char *ch_id);
int parse_url_link(const char *url, char *pd_url);

int startswith(const char *str, const char *preffix);
int endswith  (const char *str, const char *suffix);

int json_load_file(rewriter_t *rw, const char *filename);
int get_pair(jsmntok_t *tokens, const char *content, size_t index, char *key, char *value);

/* Read file content into content variable.
 * It reallocates content dynamically. After reading
 * returns exact size of readed content
 */
int read_file(const char *filename, char **content, size_t *content_size);

#endif
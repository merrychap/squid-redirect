#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "jsmn.h"

#define START_CONTENT_SIZE   32
#define MAX_NUMBER_OF_TOKENS 65536*2

int parse_url_data(char *line, char *url, char *ch_id) {
    if (line == NULL || url == NULL || ch_id == NULL) return get_url_null_ptr;

    strcpy(ch_id, strtok(line, " "));
    strcpy(url,   strtok(NULL, " "));
    
    return 0;
}

void reduce_https(char *url, char *new_url) {
    size_t url_len = strlen(url);
    strncpy(new_url, url, url_len-4); // try to erase :443 at the end of url
    new_url[url_len-4] = '\x00';
}

int get_pair(jsmntok_t *tokens, char *content, size_t index, char *key, char *value) {
    if (tokens == NULL)  return null_tokens_error;
    if (content == NULL) return null_content_error;
    if (key == NULL)     return null_key_error;
    if (value == NULL)   return null_value_error;
    
    jsmntok_t json_key   = tokens[index];
    jsmntok_t json_value = tokens[index+1];

    size_t key_len   = json_key.end   - json_key.start;
    size_t value_len = json_value.end - json_value.start;

    strncpy(key,   content+json_key.start,   key_len);
    strncpy(value, content+json_value.start, value_len);

    key  [key_len]   = '\x00';
    value[value_len] = '\x00';
    return 0;
}

int json_load_file(rewriter_t *rw, char *filename) {
    size_t exact_size   = 0;
    size_t content_size = START_CONTENT_SIZE;
    char * content      = (char *) malloc(content_size * sizeof(char));

    if ((exact_size = read_file(filename, &content, &content_size)) < 0) { exit(1); /* TODO handle error */ }

    jsmn_parser jparser;
    jsmntok_t   tokens[MAX_NUMBER_OF_TOKENS];

    jsmn_init(&jparser);
    int count = jsmn_parse(&jparser, content, exact_size, tokens, sizeof(tokens) / sizeof(jsmntok_t));

    /* Error handling */
    if (count < 0) return failed_parse_json;
    if (count < 1 || tokens[0].type != JSMN_OBJECT) return top_elem_not_object;

    // handle_json_tokens(ht, content, tokens, count);
    rewriter_update(rw, content, tokens, count);

    return 0;
}

int read_file(char *filename, char **content, size_t *content_size) {
    char ch        = '\x00';
    char *rcontent = *content;
    size_t index   = 0;
    FILE *fp       = fopen(filename, "r");

    if (fp == NULL) return open_file_error;

    while ((ch = fgetc(fp)) != EOF) {
        if (index < *content_size) rcontent[index++] = ch;
        else {
            char *buf = realloc(rcontent, (*content_size * 2) * sizeof(char));
            if (buf == NULL) {
                free(rcontent);
                return realloc_failed;
            }
            rcontent = buf;
            *content_size = (*content_size) * 2;
            rcontent[index++] = ch;
        }
    }
    rcontent[index] = '\x00';
    *content = rcontent;
    
    fclose(fp);
    return index-1;
}


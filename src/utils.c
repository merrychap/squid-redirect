#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "jsmn.h"

#define START_CONTENT_SIZE   32
#define MAX_NUMBER_OF_TOKENS 65536*2
#define TOKEN_SIZE           1024

int parse_url_data(char *line, char *url, char *ch_id) {
    if (line == NULL || url == NULL || ch_id == NULL) return get_url_null_ptr;

    strcpy(ch_id, strtok(line, " "));
    strcpy(url,   strtok(NULL, " "));
    
    return 0;
}

static void handle_json_tokens(char *content, jsmntok_t *tokens, size_t count) {
    char key  [TOKEN_SIZE];
    char value[TOKEN_SIZE];

    for (int i = 1; i < count; i += 2) {
        jsmntok_t json_key   = tokens[i];
        jsmntok_t json_value = tokens[i+1];

        size_t key_len   = json_key.end   - json_key.start;
        size_t value_len = json_value.end - json_value.start;

        strncpy(key,   content+json_key.start,   key_len);
        strncpy(value, content+json_value.start, value_len);

        key  [key_len]   = '\x00';
        value[value_len] = '\x00';

        /* TODO DO SOMETHING WITH THIS VALUES */
        fprintf(stdout, "%s : %s\n", key, value);
    }
}

int json_load_file(char *filename) {
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

    handle_json_tokens(content, tokens, count);

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "rewriter.h"
#include "utils.h"

#define MAX_LEN           1000
#define TABLE_SIZE        655536 * 8
#define TOKEN_SIZE        1024
#define REDIRECTIONS_FILE "redirections.json"

int rewrite_url(rewriter_t *rw, char *line, char *resp, size_t resp_size) {
    char url  [MAX_LEN];
    char ch_id[MAX_LEN];

    memset(url,   0, MAX_LEN);
    memset(ch_id, 0, MAX_LEN);

    syslog(LOG_INFO, "new request: %s", line);

    json_load_file(rw, REDIRECTIONS_FILE);
    if (parse_url_data(line, url, ch_id) != 0) { /* TODO handle exception */ }

    char *new_url = ht_get(rw->table, url);

    if (new_url == NULL) {
        char red_url[MAX_LEN];
        reduce_https(url, red_url);

        char *new_url = ht_get(rw->table, red_url);

        if (new_url == NULL) snprintf(resp, resp_size, "%s OK\n", ch_id);
        else snprintf(resp, resp_size, "%s OK rewrite-url=%s\n", ch_id, new_url);
    }
    else snprintf(resp, resp_size, "%s OK rewrite-url=%s\n", ch_id, new_url);

    return 0;
}

int rewriter_init(rewriter_t *rw) {
    if (rw == NULL) return null_rewriter_error;
    rw->table = ht_create(TABLE_SIZE);
    return 0;
}

int rewriter_update(rewriter_t *rw, char *content, jsmntok_t *tokens, int count) {
    if (rw == NULL)      return null_rewriter_error;
    if (content == NULL) return null_content_error;
    if (tokens == NULL)  return null_tokens_error;

    if (rw->__content == NULL || rw->__tokens == NULL) { goto update_rw; }
    if (strcmp(rw->__content, content) == 0) return 0;

    char rw_key  [TOKEN_SIZE];
    char rw_value[TOKEN_SIZE];
    char ps_key  [TOKEN_SIZE];
    char ps_value[TOKEN_SIZE];

    for (size_t i = 0; i < rw->__count; i++) {
        int is_present = 0;
        get_pair(rw->__tokens, rw->__content, i, rw_key, rw_value);
        for (size_t j = 0; j < count; j++) {
            get_pair(tokens, content, j, ps_key, ps_value);
            if (strcmp(rw_key, ps_key) == 0) {
                is_present = 1;
                break;
            }
        }
        if (!is_present) ht_remove(rw->table, rw_key);
    }

    for (size_t i = 1; i < count; i += 2) {
        get_pair(tokens, content, i, ps_key, ps_value);
        // printf("%s: %s\n", ps_key, ps_value);
        ht_insert(rw->table, ps_key, ps_value);
    }

update_rw:
    rw->__tokens  = tokens;
    rw->__content = content;
    rw->__count   = count;
    
    return 0;
}
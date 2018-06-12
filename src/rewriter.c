#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "rewriter.h"
#include "utils.h"

#define MAX_LEN          1000
#define TABLE_SIZE       655536 * 8
#define TOKEN_SIZE       1024
#define REDIRECTION_FILE "/tmp/squid-redirect/redirections.json"

int rewrite_url(rewriter_t *rw, char *line, char *resp, size_t resp_size) {
    char url   [MAX_LEN];
    char pd_url[MAX_LEN];
    char ch_id [MAX_LEN];

    memset(url,   0, MAX_LEN);
    memset(ch_id, 0, MAX_LEN);

    syslog(LOG_INFO, "-> request:  %s", line);

    json_load_file(rw, REDIRECTION_FILE);
    if (parse_url_data(line, url, ch_id) != 0) { exit(1); /* TODO handle exception */ }
    if (parse_url_link(url, pd_url) != 0)      { exit(1); /* TODO handle exception */ }

    syslog(LOG_INFO, "parsed url:  %s", pd_url);

    char *new_url = ht_get(rw->table, pd_url);

    if (new_url == NULL) {
        snprintf(resp, resp_size, "%s OK\n", ch_id);
        syslog(LOG_INFO, "redirection: missing");
    } else {
        snprintf(resp, resp_size, "%s OK status=301 url=https://%s\n", ch_id, new_url);
        syslog(LOG_INFO, "redirection: %s -> %s", pd_url, new_url);
    }

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
        // syslog(LOG_INFO, "%s : %s", ps_key, ps_value);
        ht_insert(rw->table, ps_key, ps_value);
    }

update_rw:
    rw->__tokens  = tokens;
    rw->__content = content;
    rw->__count   = count;
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rewriter.h"
#include "utils.h"

#define MAX_LEN 1000

int rewrite_url(char *line, char *resp, size_t resp_size) {
    char url  [MAX_LEN];
    char ch_id[MAX_LEN];

    memset(url,   0, MAX_LEN);
    memset(ch_id, 0, MAX_LEN);
    
    if (parse_url_data(line, url, ch_id) != 0) { /* TODO handle exception */ }

    if (strstr(url, "sex") != NULL) snprintf(resp, resp_size, "%s OK rewrite-url=http://mindchasers.com/blocked\n", ch_id);
    else snprintf(resp, resp_size, "%s OK\n", ch_id);

    return 0;
}
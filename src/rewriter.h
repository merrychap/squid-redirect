#ifndef __REWRITER_H__
#define __REWRITER_H__

#include <stdio.h>

#include "jsmn.h"
#include "hashtable.h"

typedef struct rewriter_t rewriter_t;

typedef enum {
    null_rewriter_error = -1,
    null_content_error  = -2,
    null_tokens_error   = -3
} rewriter_error_t;

struct rewriter_t {
    /* Hashtable for matching redirections */
    hashtable_t *table;
    jsmntok_t   *__tokens;
    char        *__content;
    int          __count;
};

int rewrite_url(rewriter_t *rw, char *url, char *rw_url, size_t rw_url_size);

/* rewriter methods */
int rewriter_init  (rewriter_t *rw);
int rewriter_update(rewriter_t *rw, char *content, jsmntok_t *tokens, int count);

#endif
#ifndef __HASHTABLE_H_
#define __HASHTABLE_H_

#include <stdio.h>

typedef struct ht_entry_t  ht_entry_t;
typedef struct hashtable_t hashtable_t;

typedef enum {
    null_hashtable_error = -1,
    null_key_error       = -2,
    null_value_error     = -3,
    ht_remove_error      = -4
} hashtable_error_t;

struct ht_entry_t {
    char    *key;
    char    *value;
    ht_entry_t *next;
};

struct hashtable_t {
    ht_entry_t **table;
    size_t size;
};


hashtable_t * ht_create(size_t size);
size_t        ht_hash  (hashtable_t *ht, char *str);
ht_entry_t *  ht_entry (char *key, char *value);
int           ht_insert(hashtable_t *ht, char *key, char *value);
char *        ht_get   (hashtable_t *ht, char *key);
int           ht_remove(hashtable_t *ht, char *key);
int           ht_clear (hashtable_t *ht);

#endif
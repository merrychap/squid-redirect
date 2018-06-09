#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

hashtable_t * ht_create(size_t size) {
    if (size == 0) return NULL;
    
    hashtable_t *ht = (hashtable_t *)  malloc(sizeof(hashtable_t));
    if (ht == NULL) return NULL;

    ht->table = (ht_entry_t **) malloc(sizeof(ht_entry_t *) * size);
    if (ht->table == NULL) return NULL;
    
    for (int i = 0; i < size; i++) ht->table[i] = NULL;
    ht->size  = size;
    
    return ht;
}

size_t ht_hash(hashtable_t *ht, char *str) {
    size_t hash  = 5381;
    char   ch    = 0;

    while ((ch = *str++) != 0)
        hash = ((hash << 5) + hash) + ch; /* hash * 33 + c */
    return hash;
}

ht_entry_t * ht_pair(char *key, char *value) {
    ht_entry_t *pair = (ht_entry_t *) malloc(sizeof(ht_entry_t));
    if (pair == NULL) return NULL;
    if ((pair->key   = strdup(key))   == NULL) return NULL;
    if ((pair->value = strdup(value)) == NULL) return NULL;

    pair->next = NULL;

    return pair;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

#define _XOPEN_SOURCE 500

hashtable_t * ht_create(size_t size) {
    if (size == 0) return NULL;
    
    hashtable_t *ht = (hashtable_t *) malloc(sizeof(hashtable_t));
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
        hash += hash * 37 + ch;
    return hash % ht->size;
}

ht_entry_t * ht_entry(char *key, char *value) {
    if (key == NULL) return NULL;

    ht_entry_t *entry = (ht_entry_t *) malloc(sizeof(ht_entry_t));
    if (entry == NULL) return NULL;
    if ((entry->key   = strdup(key))   == NULL) return NULL;
    if ((entry->value = strdup(value)) == NULL) return NULL;

    entry->next = NULL;

    return entry;
}

int ht_insert(hashtable_t *ht, char *key, char *value) {
    if (ht == NULL)  return null_hashtable_error;
    if (key == NULL) return null_key_error;
    
    size_t key_hash  = ht_hash(ht, key);
    ht_entry_t *cur  = ht->table[key_hash];
    ht_entry_t *prev = NULL;

    while (cur != NULL && strcmp(key, cur->key) > 0) {
        prev = cur;
        cur  = cur->next;
    }

    if (cur != NULL && cur->key != NULL && strcmp(key, cur->key) == 0) {
        free(cur->value);
        cur->value = strdup(value);
    } else {
        ht_entry_t *entry = ht_entry(key, value);

        if (ht->table[key_hash] == cur) {
            ht->table[key_hash] = entry;
            entry->next         = cur;
        } else if (cur == NULL) {
            prev->next = entry;
        } else {
            prev->next  = entry;
            entry->next = cur;
        }
    }
    return 0;
}

char * ht_get(hashtable_t *ht, char *key) {
    if (ht == NULL || key == NULL) return NULL;

    size_t key_hash   = ht_hash(ht, key);
    ht_entry_t *entry = ht->table[key_hash];

    while (entry != NULL && entry->key != NULL && strcmp(key, entry->key) > 0)
        entry = entry->next;
    
    if (entry == NULL || entry->key == NULL || strcmp(key, entry->key) != 0) return NULL;
    else return entry->value;
}

int ht_remove(hashtable_t *ht, char *key) {
    if (ht == NULL)  return null_hashtable_error;
    if (key == NULL) return null_key_error;

    size_t key_hash   = ht_hash(ht, key);
    ht_entry_t *entry = ht->table[key_hash];
    ht_entry_t *prev  = NULL;

    while (entry != NULL && entry->key != NULL && strcmp(key, entry->key) > 0) {
        prev  = entry;
        entry = entry->next;
    }

    if (entry == NULL || entry->key == NULL || strcmp(key, entry->key) != 0) return ht_remove_error;
    else {
        prev->next = entry->next;
        free(entry->key);
        free(entry->value);
        free(entry);
    }
    return 0;
}

int ht_clear(hashtable_t *ht) {
    return 0;
}
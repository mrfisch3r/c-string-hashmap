#ifndef STRMAP_H
#define STRMAP_H

typedef struct strmap strmap_t;

strmap_t *strmap_create(int numbuckets);
void *strmap_put(strmap_t *m, char *key, void *value);
void *strmap_get(strmap_t *m, char *key);
void *strmap_remove(strmap_t *m, char *key);
int strmap_getsize(strmap_t *m);
int strmap_getnbuckets(strmap_t *m);
void strmap_dump(strmap_t *m);

#endif
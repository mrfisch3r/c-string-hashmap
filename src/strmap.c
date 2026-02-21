#include "strmap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum { STRMAP_MIN_BUCKETS = 2, STRMAP_MAX_BUCKETS = 4096 };

typedef struct smel {
    char *sme_key;
    void *sme_value;
    struct smel *sme_next;
} smel_t;

struct strmap {
    int strmap_size;
    int strmap_nbuckets;
    smel_t **strmap_buckets;
};

static char *strmap_strdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

//Hash function: using char * and numbuckets, return an index into the bucket array (0 <= int < strmap_nbuckets (size of array))
unsigned long hash(char *key, int numbuckets) {

    //initialize an unsigned long variable with a prime number > MAX_SIZE
    unsigned long hashValue = 11939;
    
    //iterate through string, for every character, multiply hashValue by the fixed constant 3, then add the character to it
    for (int i = 0; key[i] != 0; i++) {
        hashValue = hashValue * 3;
        hashValue = hashValue + key[i];
    }

    //modulo by # of buckets
    hashValue = hashValue % numbuckets;
    
    //return the hashValue
    return hashValue;
}

/* Creates a new strmap_t, initializes it to be empty, and returns a pointer to it.
The map's array will have numbuckets elements. If the numbuckets argument is larger
than the constant STRMAP_MAX_BUCKETS (defined in strmap.h), or smaller than STRMAP_MIN_BUCKETS,
it is 'clipped' to be within the appropriate range. For best performance, the
number of buckets should be chosen to be larger than the number of pairs the map is
expected to hold. */
strmap_t *strmap_create(int numbuckets) {

    // check if numbuckets is within bounds, 'clipping' if not
    if (numbuckets < STRMAP_MIN_BUCKETS) {
        numbuckets = STRMAP_MIN_BUCKETS;
    } 
    else if (numbuckets > STRMAP_MAX_BUCKETS) {
        numbuckets = STRMAP_MAX_BUCKETS;
    }

    // allocate memory for strmap_t structure
    strmap_t *temp = (strmap_t *) malloc(sizeof(strmap_t));

    // initialize # of elements in array to 0
    temp->strmap_size = 0;

    // initialize size of array
    temp->strmap_nbuckets = numbuckets;

    // set size of bucket array
    temp->strmap_buckets = (smel_t **)calloc(numbuckets, sizeof(smel_t*));
    
    // return initialized array
    return temp;
}

/* Stores the pair(key,value) in the map m. If the map already had a value bound to
the given key, it is replaced and the old value is returned. Otherwise (if the key 
is new to the map) NULL is returned.*/
void *strmap_put(strmap_t *m, char *key, void *value){

    //get index # from hash and initialized new smel_t element at that index in  the bucket array
    unsigned int index = hash(key, m->strmap_nbuckets);
    smel_t *newe = m->strmap_buckets[index];

    //search for key in linked list of smel_t elements, using the NULL terminator to stop the loop
    while (newe){

        //if key is found in list, overwrite value and return original value
        if (strcmp(newe->sme_key, key) == 0){
            void *prev = newe->sme_value;
            newe->sme_value = value;
            return prev;
        }

        //increment, moving through linked list searching for key or NULL
        newe = newe->sme_next;
    }

    //if key not found, allocate new data for new linked element
    newe = (smel_t *)malloc(sizeof(smel_t));

    //populate struct members of smel_t
    newe->sme_key = strmap_strdup(key);
    newe->sme_value = value;
    newe->sme_next = m->strmap_buckets[index];
    m->strmap_buckets[index] = newe;

    //increase size of string map
    m->strmap_size++;

    return NULL;

}

/* Returns the value bound to key in the map, or NULL if none */
void *strmap_get(strmap_t *m, char *key) {

    unsigned int index = hash(key, m->strmap_nbuckets);
    smel_t *gete = m->strmap_buckets[index];

    // walk through list, searching for input key
    while (gete) {
        if (strcmp(gete->sme_key, key) == 0) {
            return gete->sme_value;
        }
        gete = gete->sme_next;
    }

    // if key not found, return null
    return NULL;
}

/* Removes the element with the given key from the map and returns its value, or null if there was no such element.*/
void *strmap_remove(strmap_t *m, char *key) {

    // initialize smel_t element to search for and remove
    unsigned int index = hash(key, m->strmap_nbuckets);
    smel_t *reme = m->strmap_buckets[index];
    smel_t *prev = NULL;

    while (reme) {
        if (strcmp(reme->sme_key, key) == 0) {

            // if there is an element ahead of the element to remove
            if (prev) {
                prev->sme_next = reme->sme_next;
            }

            // if the element to remove is the header
            else {
                m->strmap_buckets[index] = reme->sme_next;
            }

            // decrement map size and return value from removed key
            void *value = reme->sme_value;
            m->strmap_size--;
            return value;
        }

        // iterate through the linked list to search for element to remove
        prev = reme;
        reme = reme->sme_next;
    }
    return NULL;
}

// Returns the # of key-value pairs currently stored in the map
int strmap_getsize(strmap_t *m) {
    return m->strmap_size;
}

// Return the number of buckets in the map's array
int strmap_getnbuckets(strmap_t *m) {
    return m->strmap_nbuckets;
}

/* Iterates over the buckets of the map. Empty bucket are ignored. For each nonempty
bucket i, a line containing 'Bucket i:' is printed, followed by the list of pairs
for that bucket, with each pair on a separate line. The value of the 'pointer'
bound to the key is printed in hexadecimal (using the %p format specifier).*/
void strmap_dump(strmap_t *m) {
    printf("total elements = %i\n", strmap_getsize(m));

    // iterate through the bucket array, searching for nonempty buckets
    for (int i = 0; i < m->strmap_nbuckets; i++) {
        smel_t *elem = m->strmap_buckets[i];
        
        // if nonempty, print key and value
        if (elem != NULL) {
            printf("bucket %u:\n ", i);
            while (elem) {
                printf("%s->%p ", elem->sme_key, elem->sme_value);
                elem = elem->sme_next;
            }
            printf("\n");
        }
    }
}
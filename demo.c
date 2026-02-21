#include <stdio.h>
#include "strmap.h"

int main(void) {
    strmap_t *m = strmap_create(8);

    strmap_put(m, "apple",  "red");
    strmap_put(m, "banana", "yellow");
    strmap_put(m, "grape",  "purple");

    printf("apple -> %s\n", (char *)strmap_get(m, "apple"));
    printf("banana -> %s\n", (char*)strmap_get(m, "banana"));
    printf("grape  -> %s\n", (char*)strmap_get(m, "grape"));
    strmap_dump(m);

    return 0;
}
//
// Created by easy on 18.10.23.
//

#include "axstack.h"
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef unsigned long ulong;

struct AXstack {
    void **items;
    ulong len;
    ulong cap;
    void (*destroy)(void *);
};

union Long {
    ulong u;
    long s;
};

static long len(AXstack *s);

static ulong toItemSize(ulong n) {
    return n * sizeof(void *);
}


static AXstack *sizedNew(ulong size) {
    size = MAX(1, size);
    AXstack *s = malloc(sizeof *s);
    if (s) s->items = malloc(toItemSize(size));

    if (!s || !s->items) {
        free(s);
        return NULL;
    }

    s->len = 0;
    s->cap = size;
    s->destroy = NULL;
    return s;
}


static AXstack *new(void) {
    return sizedNew(7);
}


static void destroy(AXstack *s) {
    if (s->destroy) while (s->len > 0) {
        s->destroy(s->items[--s->len]);
    }

    free(s->items);
    free(s);
}


static bool push(AXstack *s, void *val) {
    if (s->len >= s->cap) {
        ulong cap = (s->cap << 1) | 1;  // add another bit
        void **items = realloc(s->items, toItemSize(cap));
        if (!items) return true;
        s->items = items;
        s->cap = cap;
    }

    s->items[s->len++] = val;
    return false;
}


static void *pop(AXstack *s) {
    return s->len ? s->items[--s->len] : NULL;
}


static void *top(AXstack *s) {
    return s->len ? s->items[s->len - 1] : NULL;
}


static union Long normaliseIndex(ulong len, long index) {
    union Long i = {.s = index};
    i.u += (i.s < 0) * len;    // convert negative to positive index
    i.u = len - 1 - i.u;       // reverse index (0 points to top)
    return i;
}


static void *at(AXstack *s, long index) {
    ulong i = normaliseIndex(s->len, index).u;
    return i < s->len ? s->items[i] : NULL;    // get item if 0 <= index < len
}


static bool swap(AXstack *s, long index1, long index2) {
    ulong i1 = normaliseIndex(s->len, index1).u;
    ulong i2 = normaliseIndex(s->len, index2).u;
    if (i1 >= s->len || i2 >= s->len)
        return true;

    void *tmp = s->items[i1];
    s->items[i1] = s->items[i2];
    s->items[i2] = tmp;
    return false;
}


static AXstack *reverse(AXstack *s) {
    void **l = s->items;
    void **r = s->items + s->len - 1;

    while (l < r) {
        void *tmp = *l;
        *l = *r;
        *r = tmp;
        ++l; --r;
    }

    return s;
}


static AXstack *clear(AXstack *s) {
    if (s->destroy) while (s->len) {
        s->destroy(s->items[--s->len]);
    }

    s->len = 0;
    return s;
}


static AXstack *copy(AXstack *s) {
    AXstack *s2 = sizedNew(s->cap);
    if (!s2) return NULL;

    memcpy(s2->items, s->items, toItemSize(s->len));
    s2->len = s->len;
    return s2;
}


static bool resize(AXstack *s, ulong size) {
    size = MAX(1, size);

    if (size < s->len && s->destroy) while (s->len > size) {
        s->destroy(s->items[--s->len]);
    } else {
        s->len = MIN(s->len, size);
    }

    void **items = realloc(s->items, toItemSize(size));
    if (!items) return true;
    s->items = items;
    s->cap = size;
    return false;
}


static AXstack *destroyItem(AXstack *s, void *val) {
    if (s->destroy) s->destroy(val);
    return s;
}


static AXstack *setDestructor(AXstack *s, void (*destroy)(void *)) {
    s->destroy = destroy;
    return s;
}


static void (*getDestructor(AXstack *s))(void *) {
    return s->destroy;
}


static void **data(AXstack *s) {
    return s->items;
}


static long len(AXstack *s) {
    union Long len = {s->len};
    len.u = len.u << 1 >> 1;
    return len.s;
}


static long cap(AXstack *s) {
    union Long cap = {s->cap};
    cap.u = cap.u << 1 >> 1;
    return cap.s;
}


const AXstackFuncs axs = {
        new,
        sizedNew,
        destroy,
        push,
        pop,
        top,
        len,
        at,
        swap,
        reverse,
        clear,
        copy,
        resize,
        destroyItem,
        setDestructor,
        getDestructor,
        data,
        cap
};

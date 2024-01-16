//
// Created by easy on 18.10.23.
//

#ifndef AXSTACK_AXSTACK_H
#define AXSTACK_AXSTACK_H

#include <stdbool.h>

// simple and fast array-based stack implementation
typedef struct axstack axstack;

struct axstackFn {
    // create new stack with default capacity
    axstack *(*new)(void);
    // create new stack with custom capacity
    axstack *(*sizedNew)(unsigned long size);
    // destroy stack and optionally free all items
    void (*destroy)(axstack *s);
    // push value on stack; returns true iff OOM
    bool (*push)(axstack *s, void *val);
    // pop value from stack; returns NULL if stack empty
    void *(*pop)(axstack *s);
    // peek top of stack without removal; returns NULL if stack empty
    void *(*top)(axstack *s);
    // get length of stack
    long (*len)(axstack *s);
    // index stack; 0 top, -1 bottom etc.; returns NULL on index error
    void *(*at)(axstack *s, long index);
    // swap two items; 0 top, -1 bottom etc.; returns true iff index error
    bool (*swap)(axstack *s, long index1, long index2);
    // reverse stack order in-place; returns stack
    axstack *(*reverse)(axstack *s);
    // call destructor if any and remove all items
    axstack *(*clear)(axstack *s);
    // create new shallow copy of stack
    axstack *(*copy)(axstack *s);
    // set capacity to some value thereby calling the destructor on excess items when shrinking. True iff OOM,
    // changing length of stack and calling of destructors is done regardless of fail or not
    bool (*resize)(axstack *s, unsigned long size);
    // call destructor if available on item
    axstack *(*destroyItem)(axstack *s, void *val);
    // set destructor function (passing NULL will disable destructor)
    axstack *(*setDestructor)(axstack *v, void (*destroy)(void *));
    // get destructor function
    void (*(*getDestructor)(axstack *v))(void *);
    // get pointer to underlying array; points to bottom of stack
    void **(*data)(axstack *s);
    // get capacity of stack
    long (*cap)(axstack *s);
};

#ifdef AXSTACK_NAMESPACE
#define axs AXSTACK_NAMESPACE
#endif

extern const struct axstackFn axs;

#undef axs

#endif //AXSTACK_AXSTACK_H

//
// Created by easy on 18.10.23.
//

#ifndef AXSTACK_AXSTACK_H
#define AXSTACK_AXSTACK_H

#include <stdbool.h>

// simple and fast array-based stack implementation
typedef struct AXstack AXstack;

typedef struct AXstackFuncs {
    // create new stack with default capacity
    AXstack *(*new)(void);
    // create new stack with custom capacity
    AXstack *(*sizedNew)(unsigned long size);
    // destroy stack and optionally free all items
    void (*destroy)(AXstack *s);
    // push value on stack; returns true iff OOM
    bool (*push)(AXstack *s, void *val);
    // pop value from stack; returns NULL if stack empty
    void *(*pop)(AXstack *s);
    // peek top of stack without removal; returns NULL if stack empty
    void *(*top)(AXstack *s);
    // get length of stack
    long (*len)(AXstack *s);
    // index stack; 0 top, -1 bottom etc.; returns NULL on index error
    void *(*at)(AXstack *s, long index);
    // swap two items; 0 top, -1 bottom etc.; returns true iff index error
    bool (*swap)(AXstack *s, long index1, long index2);
    // reverse stack order in-place; returns stack
    AXstack *(*reverse)(AXstack *s);
    // call destructor if any and remove all items
    AXstack *(*clear)(AXstack *s);
    // create new shallow copy of stack
    AXstack *(*copy)(AXstack *s);
    // set capacity to some value thereby calling the destructor on excess items when shrinking. True iff OOM,
    // changing length of stack and calling of destructors is done regardless of fail or not
    bool (*resize)(AXstack *s, unsigned long size);
    // call destructor if available on item
    AXstack *(*destroyItem)(AXstack *s, void *val);
    // set destructor function (passing NULL will disable destructor)
    AXstack *(*setDestructor)(AXstack *v, void (*destroy)(void *));
    // get destructor function
    void (*(*getDestructor)(AXstack *v))(void *);
    // get pointer to underlying array; points to bottom of stack
    void **(*data)(AXstack *s);
    // get capacity of stack
    long (*cap)(AXstack *s);
} AXstackFuncs;

extern const AXstackFuncs axs;

#endif //AXSTACK_AXSTACK_H

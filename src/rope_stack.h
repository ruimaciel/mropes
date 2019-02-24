#ifndef MROPES_STACK_H
#define MROPES_STACK_H

#include <mropes/rope.h>

struct mrope_stack
{
	struct mrope_node **data;
	size_t size;
	size_t pos;
};

typedef struct mrope_stack mrope_stack_t;

mreturn_t mrope_stack_init(mrope_stack_t *stack);
void mrope_stack_free(mrope_stack_t *stack);

struct mrope_node * mrope_stack_top(mrope_stack_t *stack);
mreturn_t mrope_stack_grow_stack(mrope_stack_t *stack);
mreturn_t mrope_stack_push(mrope_stack_t *stack, struct mrope_node * node);
mreturn_t mrope_stack_pop(mrope_stack_t *stack, struct mrope_node **node);

#endif

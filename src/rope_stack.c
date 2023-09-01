#include "rope_stack.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

mreturn_t mrope_stack_init(mrope_stack_t *stack) {
  assert(stack != NULL);

  stack->pos = 0;
  stack->size = 8;
  stack->data = NULL;
  stack->data = malloc(stack->size);

  if (stack->data == NULL) {
    stack->size = 0;
    return MROPE_MALLOC_FAILED;
  }

  return MROPE_OK;
}

void mrope_stack_free(mrope_stack_t *stack) {
  assert(stack != NULL);

  if (stack->data != NULL) {
    free(stack->data);
    stack->data = NULL;
  }
  stack->pos = 0;
  stack->size = 0;
}

mreturn_t mrope_stack_push(mrope_stack_t *stack, struct mrope_node *node) {
  assert(stack != NULL);

  if (stack->pos == stack->size) {
    enum mrope_error_code error = mrope_stack_grow_stack(stack);
    if (error != MROPE_OK) {
      return error;
    }
  }

  stack->data[stack->pos++] = node;

  return MROPE_OK;
}

mreturn_t mrope_stack_grow_stack(mrope_stack_t *stack) {
  assert(stack != NULL);

  const size_t new_size = stack->size << 1;

  struct mrope_node **new_data = realloc(stack->data, new_size);
  if (new_data == NULL) {
    return MROPE_MALLOC_FAILED;
  }

  stack->data = new_data;
  stack->size = new_size;

  return MROPE_OK;
}

struct mrope_node *mrope_stack_top(mrope_stack_t *stack) {
  assert(stack != NULL);

  return (stack->pos == 0) ? NULL : stack->data[stack->pos - 1];
}

mreturn_t mrope_stack_pop(mrope_stack_t *stack, struct mrope_node **node) {
  assert(stack != NULL);

  *node = mrope_stack_top(stack);

  if (stack->pos > 0) stack->pos--;

  return MROPE_OK;
}

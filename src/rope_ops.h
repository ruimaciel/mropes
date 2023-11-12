#ifndef MROPES_OPS_H
#define MROPES_OPS_H

#include <mropes/rope.h>

void mrope_init_branch_node(struct mrope_branch_node *node,
                            struct mrope_node *lhs, struct mrope_node *rhs);
void mrope_init_leaf_node(struct mrope_leaf_node *node, char *text,
                          const size_t length);

struct mrope_branch_node *mrope_make_branch_node(struct mrope_node *lhs,
                                                 struct mrope_node *rhs);
struct mrope_leaf_node *mrope_make_leaf_node(void);

enum mrope_error_code mrope_free_branch_node(struct mrope_branch_node *node);
enum mrope_error_code mrope_free_leaf_node(struct mrope_leaf_node *node);
enum mrope_error_code mrope_free_node(struct mrope_node *node);

mreturn_t mrope_split_leaf_node(struct mrope_leaf_node *original,
                                const size_t index, struct mrope_leaf_node *lhs,
                                struct mrope_leaf_node *rhs);

mreturn_t mrope_clone_branch_node(struct mrope_branch_node *original,
                                  struct mrope_branch_node *clone);
mreturn_t mrope_clone_leaf_node(struct mrope_leaf_node *original,
                                struct mrope_leaf_node *clone);
mreturn_t mrope_clone_node(struct mrope_node *original,
                           struct mrope_node **clone);

char mrope_index_branch_node(struct mrope_branch_node *branch_node,
                             const size_t index);
char mrope_index_leaf_node(struct mrope_leaf_node *leaf_node,
                           const size_t index);
char mrope_index_node(struct mrope_node *node, const size_t index);

mreturn_t mrope_make_leaf_node_from_text(struct mrope_leaf_node **leaf_node_out,
                                         char *text);

size_t mrope_calculate_weights(struct mrope_node *node);

#endif /* MROPES_OPS_H */

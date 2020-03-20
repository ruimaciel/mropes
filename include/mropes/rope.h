#ifndef MROPES_H
#define MROPES_H

#include <stddef.h>

enum mrope_node_type {
	MROPE_NODE_UNKNOWN = 0,
	MROPE_NODE_LEAF,
	MROPE_NODE_BRANCH
};

struct mrope_node {
	enum mrope_node_type type;
	size_t weight;
};

struct mrope_branch_node {
	struct mrope_node node;

	struct mrope_node * left;
	struct mrope_node * right;
};

struct mrope_leaf_node {
	struct mrope_node node;

	char * text;
};

struct mrope {
	struct mrope_node * root_node;
};

enum mrope_error_code
{
	MROPE_OK = 0,
	MROPE_MALLOC_FAILED,
	MROPE_UNKNOWN_NODE_TYPE,
	MROPE_OUT_OF_RANGE
};

typedef enum mrope_error_code mreturn_t;

typedef struct mrope mrope_t;

mreturn_t mrope_init(mrope_t *rope_out);
mreturn_t mrope_free(mrope_t *rope_inout);
mreturn_t mrope_prepend_rope(mrope_t *rope_inout, mrope_t *rope_inout_lhs);
mreturn_t mrope_append_text(mrope_t *rope_inout, char *text_rhs);
mreturn_t mrope_append_rope(mrope_t *rope_inout, mrope_t *rope_inout_rhs);
mreturn_t mrope_clone(const mrope_t *rope_in, mrope_t *rope_out);
mreturn_t mrope_concat( const mrope_t *rope_in_lhs, const mrope_t *rope_in_rhs, mrope_t *rope_out);
mreturn_t mrope_split(const mrope_t *rope_in, const size_t index, mrope_t *rope_out_lhs, mrope_t *rope_out_rhs);
mreturn_t mrope_insert(mrope_t *rope_inout, const size_t index, mrope_t *rope_in);
mreturn_t mrope_delete(mrope_t *rope_inout, const size_t index, const size_t length);

size_t mrope_length(const mrope_t *rope_in);
char mrope_index(const mrope_t *rope_in, const size_t index);

#endif /* MROPES_H */

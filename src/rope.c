#include <mropes/rope.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rope_ops.h"
#include "rope_stack.h"


mreturn_t mrope_init(mrope_t *rope_out)
{
	assert(rope_out != NULL);

	rope_out->root_node = NULL;

	return MROPE_OK;
}


mreturn_t mrope_free(mrope_t *rope_inout)
{
	mreturn_t error = MROPE_OK;

	assert(rope_inout != NULL);

	if(rope_inout->root_node == NULL) {
		return MROPE_OK;
	}

	/*TODO replace with breadth-first search */
	if( (error = mrope_free_node(rope_inout->root_node)) != MROPE_OK) {
		return error;
	}

	rope_inout->root_node = NULL;

	return MROPE_OK;
}


mreturn_t mrope_prepend_rope(mrope_t * rope_inout, mrope_t * rope_inout_lhs)
{
	struct mrope_branch_node * new_root = NULL;

	assert(rope_inout != NULL);

	if(rope_inout_lhs == NULL) {
		/* nothing to concatenate */
		return MROPE_OK;
	}
	if(rope_inout_lhs->root_node == NULL) {
		/* nothing to concatenate */
		return MROPE_OK;
	}

	new_root = mrope_make_branch_node(rope_inout_lhs->root_node, rope_inout->root_node);
	if(new_root == NULL) {
		return MROPE_MALLOC_FAILED;
	}

	rope_inout->root_node = (struct mrope_node *)new_root;

	rope_inout_lhs->root_node = NULL;

	return MROPE_OK;
}


mreturn_t mrope_append_text(mrope_t *rope_inout, char *text_rhs)
{
	mreturn_t error = MROPE_OK;
	struct mrope_leaf_node * leaf_node = NULL;

	assert(rope_inout != NULL);
	assert(text_rhs != NULL);

	/* convert text into a leaf node */

	if( (error = mrope_make_leaf_node_from_text(&leaf_node, text_rhs)) != MROPE_OK) {
		return error;
	}

	if(rope_inout->root_node == NULL) {
		rope_inout->root_node = (struct mrope_node *)leaf_node;
		return MROPE_OK;
	}

	switch(rope_inout->root_node->type)
	{

	case MROPE_NODE_LEAF:
	{
		struct mrope_branch_node * branch_node = NULL;
		if( (branch_node = mrope_make_branch_node(rope_inout->root_node, (struct mrope_node *)leaf_node)) == NULL) {
			/* FIXIT free leaf node */
			mrope_free_leaf_node(leaf_node);
			return MROPE_MALLOC_FAILED;
		}

		rope_inout->root_node = (struct mrope_node *)branch_node;

		return MROPE_OK;
	}
		break;

	case MROPE_NODE_BRANCH:
	{
		struct mrope_branch_node * branch_root_node = (struct mrope_branch_node *) rope_inout->root_node;
		if(branch_root_node->left == NULL) {
			if(branch_root_node->right != NULL) {
				branch_root_node->left = branch_root_node->right;
				branch_root_node->right = (struct mrope_node *)leaf_node;
			}
			else {
				branch_root_node->left = (struct mrope_node *)leaf_node;
				branch_root_node->right = NULL;
			}
			branch_root_node->node.weight  = branch_root_node->left->weight;
			return MROPE_OK;
		}

		if(branch_root_node->right == NULL) {
			branch_root_node->right = (struct mrope_node *)leaf_node;
			branch_root_node->node.weight  = branch_root_node->left->weight + branch_root_node->right->weight;
			return MROPE_OK;
		}

		struct mrope_branch_node * branch = NULL;
		if( (branch = mrope_make_branch_node((struct mrope_node *)rope_inout->root_node, (struct mrope_node *)leaf_node)) == NULL) {
			mrope_free_leaf_node(leaf_node);
			return MROPE_MALLOC_FAILED;
		}

		/* update the rope to append the newly created leaf node */
		rope_inout->root_node = (struct mrope_node *)branch;

		return MROPE_OK;
	}
		break;

	default:
		return MROPE_UNKNOWN_NODE_TYPE;
	}
}


mreturn_t mrope_append_rope(mrope_t *rope_inout, mrope_t *rope_inout_rhs)
{
	struct mrope_branch_node * new_root = NULL;

	assert(rope_inout != NULL);

	if(rope_inout_rhs == NULL) {
		/* nothing to concatenate */
		return MROPE_OK;
	}
	if(rope_inout_rhs->root_node == NULL) {
		/* nothing to concatenate */
		return MROPE_OK;
	}

	new_root = mrope_make_branch_node(rope_inout->root_node, rope_inout_rhs->root_node);
	if(new_root == NULL) {
		return MROPE_MALLOC_FAILED;
	}

	rope_inout->root_node = (struct mrope_node *)new_root;

	rope_inout_rhs->root_node = NULL;


	return MROPE_OK;
}


mreturn_t mrope_concat(const mrope_t *rope_in_lhs, const mrope_t *rope_in_rhs, mrope_t *rope_out)
{
	mrope_t cloned_lhs;
	mrope_t cloned_rhs;
	struct mrope_branch_node *branch_node = NULL;

	assert(rope_out != NULL);
	assert(rope_in_lhs != NULL);
	assert(rope_in_rhs != NULL);

	mrope_init(&cloned_lhs);
	mrope_init(&cloned_rhs);

	mrope_clone(rope_in_lhs, &cloned_lhs);
	mrope_clone(rope_in_rhs, &cloned_rhs);

	branch_node = mrope_make_branch_node(cloned_lhs.root_node, cloned_rhs.root_node);

	rope_out->root_node = (struct mrope_node *)branch_node;

	return MROPE_OK;
}


mreturn_t mrope_split(const mrope_t *rope_in, const size_t index, mrope_t *rope_out_lhs, mrope_t *rope_out_rhs)
{
	mreturn_t error = MROPE_OK;
	mrope_stack_t node_stack;
	mrope_t out_lhs, out_rhs;
	size_t current_index = 0;

	assert(rope_in != NULL);
	assert(rope_out_lhs != NULL);
	assert(rope_out_rhs != NULL);

	if(index > mrope_length(rope_in)) {
		return MROPE_OUT_OF_RANGE;
	}

	/* Let's run a stack-based depth-first search through the mrope data structure */
	if( (error = mrope_stack_init(&node_stack)) != MROPE_OK) {
		return error;
	}

	mrope_stack_push(&node_stack, rope_in->root_node);

	error = mrope_init(&out_lhs);
	if(error != MROPE_OK) {
		return error;
	}

	error = mrope_init(&out_rhs);
	if(error != MROPE_OK) {
		return error;
	}

	/* the depth-first search will finish once the stack is empty */
	while(mrope_stack_top(&node_stack) != NULL) {

		struct mrope_node *node = NULL;

		error = mrope_stack_pop(&node_stack, &node);
		if(error != MROPE_OK) {
			goto error_cleanup;
		}

		switch(node->type)
		{
		case MROPE_NODE_BRANCH:
		{
			struct mrope_branch_node *branch_node = (struct mrope_branch_node *)(node);
			if(branch_node->left != NULL) {
				error = mrope_stack_push(&node_stack,branch_node->left);
				if(error != MROPE_OK) {
					goto error_cleanup;
				}
			}
			if(branch_node->right != NULL) {
				error = mrope_stack_push(&node_stack,branch_node->right);
				if(error != MROPE_OK) {
					goto error_cleanup;
				}
			}
			continue;
		}
			break;

		case MROPE_NODE_LEAF:
		{
			struct mrope_leaf_node *leaf_node = (struct mrope_leaf_node *)(node);

			/* Duplicates string */
			const size_t sz = leaf_node->node.weight+1;
			char * duplicate_string = NULL;
			if( ( duplicate_string = malloc(sz)) == NULL) {
				error = MROPE_MALLOC_FAILED;
				goto error_cleanup;
			}
			strncpy(duplicate_string, leaf_node->text, sz);

			if(index > current_index + leaf_node->node.weight) {
				error = mrope_append_text(&out_lhs, duplicate_string);
				if(error != MROPE_OK) {
					goto error_cleanup;
				}
			}
			else if(index <= current_index) {
				error = mrope_append_text(&out_rhs, duplicate_string);
				if(error != MROPE_OK) {
					mrope_free(&out_lhs);
					mrope_free(&out_rhs);
					return error;
				}
			}
			else {
				const size_t offset = index-current_index;
				char * lhs_string = NULL;
				char * rhs_string = NULL;

				duplicate_string[offset] = '\0';
				if((lhs_string = realloc(duplicate_string, offset*sizeof(char))) == NULL)  {
					error = MROPE_MALLOC_FAILED;
					goto error_cleanup;
				}

				if( (error = mrope_append_text(&out_lhs, lhs_string)) != MROPE_OK) {
					goto error_cleanup;
				}

				if( ( rhs_string = malloc(sz-offset)) == NULL) {
					error = MROPE_MALLOC_FAILED;
					goto error_cleanup;
				}
				strncpy(rhs_string, &leaf_node->text[offset], sz-offset);

				if( (error = mrope_append_text(&out_rhs, rhs_string)) != MROPE_OK) {
					goto error_cleanup;
				}
			}

			current_index += leaf_node->node.weight;
		}
			break;

		default:
		{
			error = MROPE_UNKNOWN_NODE_TYPE;
			goto error_cleanup;
		}
			break;
		}
	}

	rope_out_lhs->root_node = out_lhs.root_node;
	rope_out_rhs->root_node = out_rhs.root_node;

	return MROPE_OK;

error_cleanup:
	mrope_free(&out_lhs);
	mrope_free(&out_rhs);
	return error;

}


mreturn_t mrope_insert(mrope_t *rope_inout, const size_t index, mrope_t *rope_in)
{
	mreturn_t error = MROPE_OK;
	mrope_t temp_lhs, temp_rhs;

	assert(rope_inout != NULL);
	assert(rope_in != NULL);

	mrope_init(&temp_lhs);
	mrope_init(&temp_rhs);


	error = mrope_split(rope_inout, index, &temp_lhs, &temp_rhs);
	if(error != MROPE_OK) {
		return error;
	}

	error = mrope_append_rope(&temp_lhs, rope_in);
	if(error != MROPE_OK) {
		return error;
	}

	error = mrope_append_rope(&temp_lhs, &temp_rhs);
	if(error != MROPE_OK) {
		return error;
	}

	rope_inout->root_node = temp_lhs.root_node;
	temp_lhs.root_node = NULL;
	mrope_free(&temp_lhs);
	mrope_free(&temp_rhs);

	return MROPE_OK;
}


size_t mrope_length(const mrope_t *rope_in)
{
	assert(rope_in != NULL);

	if(rope_in->root_node == NULL) {
		return 0;
	}

	return rope_in->root_node->weight;
}


char mrope_index(const mrope_t *rope_in, const size_t index)
{
	assert(rope_in != NULL);

	return mrope_index_node(rope_in->root_node, index);

}


mreturn_t mrope_clone(const mrope_t *rope_in, mrope_t * rope_out)
{
	struct mrope_node * cloned_root_node = NULL;
	mreturn_t error = MROPE_OK;

	assert(rope_in != NULL);
	assert(rope_out != NULL);

	if(mrope_length(rope_in) == 0) {
		mrope_init(rope_out);
		return MROPE_OK;
	}

	cloned_root_node = (struct mrope_node *)malloc(sizeof (struct mrope_node));
	if(cloned_root_node == NULL) {
		return MROPE_MALLOC_FAILED;
	}

	if( (error = mrope_clone_node(rope_in->root_node, cloned_root_node)) != MROPE_OK) {
		return error;
	}

	rope_out->root_node = cloned_root_node;
	return MROPE_OK;
}


mreturn_t mrope_delete(mrope_t *rope_inout, const size_t index, const size_t length)
{
	mrope_t temp_lhs, temp_rhs;
	mreturn_t error = MROPE_OK;

	assert(rope_inout != NULL);

	mrope_init(&temp_lhs);
	mrope_init(&temp_rhs);

	if(mrope_length(rope_inout) < index || mrope_length(rope_inout) < index+length) {
		return MROPE_OUT_OF_RANGE;
	}

	error = mrope_split(rope_inout,index, &temp_lhs, &temp_rhs);
	if(error != MROPE_OK) {
		return error;
	}

	mrope_free(rope_inout);

	error = mrope_append_rope(rope_inout, &temp_lhs);
	if(error != MROPE_OK) {
		return error;
	}

	error = mrope_split(&temp_rhs, length, &temp_lhs, &temp_rhs);
	if(error != MROPE_OK) {
		return error;
	}


	error = mrope_append_rope(rope_inout, &temp_rhs);
	if(error != MROPE_OK) {
		return error;
	}

	error = mrope_free(&temp_lhs);
	if(error != MROPE_OK) {
		return error;
	}

	return MROPE_OK;
}

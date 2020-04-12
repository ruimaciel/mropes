#include "rope_ops.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rope_stack.h"


void mrope_init_branch_node(struct mrope_branch_node *node, struct mrope_node *lhs, struct mrope_node *rhs)
{
	assert(node != NULL);

	node->node.type = MROPE_NODE_BRANCH;
	node->left = lhs;
	node->right = rhs;
	node->node.weight = 0;
	node->node.weight += lhs == NULL? 0: lhs->weight;
	node->node.weight += rhs == NULL? 0: rhs->weight;
}

void mrope_init_leaf_node(struct mrope_leaf_node * node, char * text, const size_t length)
{
	assert(node != NULL);
	node->node.type = MROPE_NODE_LEAF;
	node->node.weight = length;
	node->text = text;
}

struct mrope_branch_node *mrope_make_branch_node(struct mrope_node *lhs, struct mrope_node *rhs)
{
	struct mrope_branch_node * node = NULL;

	if( (node = (struct mrope_branch_node *)malloc(sizeof(struct mrope_branch_node))) == NULL)
	{
		return NULL;
	}

	/* avoid having a null LHS  with a non-null RHS */
	if(lhs == NULL) {
		mrope_init_branch_node(node, rhs, NULL);
	}
	else {
		mrope_init_branch_node(node, lhs, rhs);
	}

	return node;
}

struct mrope_leaf_node * mrope_make_leaf_node()
{
	struct mrope_leaf_node * node = NULL;

	if( (node = (struct mrope_leaf_node *)malloc(sizeof(struct mrope_leaf_node))) == NULL)
	{
		return NULL;
	}

	mrope_init_leaf_node(node, NULL, 0);

	return node;
}

enum mrope_error_code mrope_free_branch_node(struct mrope_branch_node * branch_node)
{
	if(branch_node == NULL) {
		return MROPE_OK;
	}

	enum mrope_error_code error = MROPE_OK;

	if( (error = mrope_free_node(branch_node->left)) != MROPE_OK) {
		return error;
	}

	if( (error = mrope_free_node(branch_node->right)) != MROPE_OK) {
		return error;
	}

	free(branch_node);

	return MROPE_OK;
}

enum mrope_error_code mrope_free_leaf_node(struct mrope_leaf_node * leaf_node)
{
	if(leaf_node == NULL) {
		return MROPE_OK;
	}

	free(leaf_node->text);
	leaf_node->text = NULL;
	free(leaf_node);

	return MROPE_OK;
}


enum mrope_error_code mrope_free_node(struct mrope_node * node)
{
	if(node == NULL) {
		return MROPE_OK;
	}

	switch(node->type)
	{
	case MROPE_NODE_BRANCH:
		return mrope_free_branch_node( (struct mrope_branch_node *)node);

	case MROPE_NODE_LEAF:
		return mrope_free_leaf_node( (struct mrope_leaf_node *)node);

	default:
		return MROPE_UNKNOWN_NODE_TYPE;
	}

}


mreturn_t mrope_clone_branch_node(struct mrope_branch_node *original, struct mrope_branch_node *clone)
{
	struct mrope_branch_node * cloned_branch_node = NULL;
	mreturn_t error = MROPE_OK;
	struct mrope_node * lhs_node = NULL;
	struct mrope_node * rhs_node = NULL;

	assert(original != NULL);

	if( (error = mrope_clone_node(original->left, lhs_node)) != MROPE_OK) {
		return error;
	}

	if( (error = mrope_clone_node(original->right, rhs_node)) != MROPE_OK) {
		mrope_free_node(lhs_node);
		return error;
	}

	cloned_branch_node = mrope_make_branch_node(lhs_node, rhs_node);
	if(cloned_branch_node == NULL) {
		mrope_free_node(lhs_node);
		mrope_free_node(rhs_node);
		return MROPE_MALLOC_FAILED;
	}

	clone = cloned_branch_node;

	return MROPE_OK;
}


mreturn_t mrope_clone_leaf_node(struct mrope_leaf_node *original, struct mrope_leaf_node *clone)
{
	struct mrope_leaf_node cloned_leaf_node;
	char * cloned_text = NULL;
	size_t cloned_text_weight = 0;

	assert(original != NULL);
	assert(clone != NULL);

	cloned_text_weight = original->node.weight;

	if(cloned_text_weight > 0) {
		const size_t cloned_string_length = original->node.weight+1;

		cloned_text = (char *)malloc(cloned_string_length);
		strncpy(cloned_text, original->text, cloned_string_length);
		cloned_text[cloned_string_length] = '\0';
	}

	mrope_init_leaf_node(&cloned_leaf_node, cloned_text, cloned_text_weight);

	clone->node = cloned_leaf_node.node;
	clone->text = cloned_leaf_node.text;

	return MROPE_OK;
}

mreturn_t mrope_clone_node(struct mrope_node *original, struct mrope_node *clone)
{
	assert(original != NULL);
	assert(clone != NULL);

	switch(original->type)
	{
	case MROPE_NODE_BRANCH:
	{
		return mrope_clone_branch_node((struct mrope_branch_node *)original, (struct mrope_branch_node *)clone);
	}

	case MROPE_NODE_LEAF:
	{
		return mrope_clone_leaf_node((struct mrope_leaf_node *)original, (struct mrope_leaf_node *)clone);
	}

	default:
		return MROPE_UNKNOWN_NODE_TYPE;
	}
}


mreturn_t _mrope_index_node_range(struct mrope_node *node, char **buffer, size_t start, char *end)
{
	assert(node != NULL);

	mreturn_t error;

	while(*buffer != end)
	{

		char* old_buffer_ptr = *buffer;

		switch(node->type)
		{
		case MROPE_NODE_BRANCH:
		{
			error=mrope_index_branch_node_range( (struct mrope_branch_node *)node, buffer,  start, end);
			break;
		}

		case MROPE_NODE_LEAF:
		{
			error=mrope_index_leaf_node_range( (struct mrope_leaf_node *)node, buffer, start, end);
			break;
		}

		default:
			error=MROPE_OUT_OF_RANGE;
		}

		if(*buffer == old_buffer_ptr)
			break;

		start += *buffer - old_buffer_ptr;
	}

	return MROPE_OK;
}

char mrope_index_node(struct mrope_node *node, const size_t index)
{
	assert(node != NULL);

	switch(node->type)
	{
	case MROPE_NODE_BRANCH:
	{
		return mrope_index_branch_node( (struct mrope_branch_node *)node, index);
	}

	case MROPE_NODE_LEAF:
	{
		return mrope_index_leaf_node( (struct mrope_leaf_node *)node, index);
	}

	default:
		return '\0';
	}
}

mreturn_t mrope_index_leaf_node_range(struct mrope_leaf_node *leaf_node,char** buffer, size_t index, char *end)
{
	assert(leaf_node != NULL);
	assert(leaf_node->text != NULL);

	if(index >= leaf_node->node.weight)
		return MROPE_OUT_OF_RANGE;

	**buffer = leaf_node->text[index];

	(*buffer)++;

	return MROPE_OK;
}

char mrope_index_leaf_node(struct mrope_leaf_node *leaf_node, const size_t index)
{
	assert(leaf_node != NULL);
	assert(leaf_node->text != NULL);

	if(index >= leaf_node->node.weight) {
		return '\0';
	}

	return leaf_node->text[index];
}

mreturn_t mrope_index_branch_node_range(struct mrope_branch_node *branch_node, char **buffer, size_t index, char *end)
{
	size_t lhs_weight = 0;

	assert(branch_node != NULL);

	if(branch_node->left != NULL) {
		lhs_weight = branch_node->left->weight;
		if(lhs_weight > index) {
			_mrope_index_node_range( (struct mrope_node *) branch_node->left, buffer , index, end);
			return MROPE_OK;
		}
	}

	if(branch_node->right == NULL) {
		return MROPE_OUT_OF_RANGE;
	}

	if(index < lhs_weight + branch_node->right->weight) {
		index -= lhs_weight;
		_mrope_index_node_range( (struct mrope_node *) branch_node->right, buffer, index, end);
		return MROPE_OK;
	}

	return MROPE_OK;
}

char mrope_index_branch_node(struct mrope_branch_node *branch_node, const size_t index)
{
	size_t lhs_weight = 0;

	assert(branch_node != NULL);

	if(branch_node->left != NULL) {
		lhs_weight = branch_node->left->weight;
		if(lhs_weight > index) {
			return mrope_index_node( (struct mrope_node *) branch_node->left, index);
		}
	}

	if(branch_node->right == NULL) {
		return '\0';
	}

	if(index < lhs_weight + branch_node->right->weight) {
		return mrope_index_node( (struct mrope_node *) branch_node->right, index - lhs_weight);
	}

	return '\0';
}


mreturn_t mrope_make_leaf_node_from_text(struct mrope_leaf_node **leaf_node_out, char * text)
{
	struct mrope_leaf_node *leaf_node = NULL;
	size_t text_weight = 0;

	assert(leaf_node_out != NULL);

	leaf_node = mrope_make_leaf_node();
	if(leaf_node == NULL) {
		return MROPE_MALLOC_FAILED;
	}

	text_weight = (text == NULL) ? 0 : strlen(text);
	mrope_init_leaf_node(leaf_node, text, text_weight);

	(*leaf_node_out) = leaf_node;

	return MROPE_OK;
}


mreturn_t mrope_split_leaf_node(struct mrope_leaf_node *original, const size_t index, struct mrope_leaf_node *lhs, struct mrope_leaf_node *rhs)
{
	char * lhs_text = NULL;
	size_t lhs_text_length = 0;
	char * rhs_text = NULL;
	size_t rhs_text_length = 0;

	assert(original != NULL);
	assert(lhs != NULL);
	assert(rhs != NULL);

	if(index > original->node.weight) {
		return MROPE_OUT_OF_RANGE;
	}

	if(index == 0) {
		rhs_text_length = index;
		rhs_text = (char *)malloc(rhs_text_length+1);
		strncpy(rhs_text, original->text, rhs_text_length);
		rhs_text[rhs_text_length+1] = '\0';
	}
	else if (index == original->node.weight) {
		lhs_text_length = index;
		lhs_text = (char *)malloc(lhs_text_length+1);
		strncpy(lhs_text, original->text, lhs_text_length);
		lhs_text[lhs_text_length+1] = '\0';
	}
	else {
		lhs_text_length = index;
		lhs_text = (char *)malloc(lhs_text_length+1);
		strncpy(lhs_text, original->text, lhs_text_length);
		lhs_text[lhs_text_length+1] = '\0';

		rhs_text_length = original->node.weight - index;
		rhs_text = (char *)malloc(rhs_text_length + 1);
		strncpy(rhs_text, &original->text[index], rhs_text_length);
		rhs_text[rhs_text_length+1] = '\0';
	}

	mrope_init_leaf_node(lhs, lhs_text, lhs_text_length);
	mrope_init_leaf_node(rhs, rhs_text, rhs_text_length);

	return MROPE_OK;
}


size_t mrope_calculate_weights(struct mrope_node *node)
{
	if(node == NULL) {
		return 0;
	}

	if(node->type == MROPE_NODE_LEAF) {
		return node->weight;
	}

	if(node->type == MROPE_NODE_BRANCH) {
		struct mrope_branch_node* branch_node = (struct mrope_branch_node*)node;
		return mrope_calculate_weights(branch_node->left) + mrope_calculate_weights(branch_node->right);
	}

	/*FIXIT if this part was reached then we've stumbled on a bad node */
	return 0;
}

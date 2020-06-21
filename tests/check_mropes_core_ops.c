#include <stdlib.h>
#include <check.h>

#include "src/rope_ops.h"

START_TEST(test_rope_make_branch_node_Shall_return_node_branch)
{
	struct mrope_branch_node *node = mrope_make_branch_node(NULL, NULL);

	ck_assert_int_eq(node->node.type, MROPE_NODE_BRANCH);
}
END_TEST

START_TEST(test_rope_make_branch_node_Shall_return_null_LHS)
{
	struct mrope_branch_node *node = mrope_make_branch_node(NULL, NULL);

	ck_assert_ptr_eq(node->left, NULL);
	ck_assert_ptr_eq(node->right, NULL);
}
END_TEST

START_TEST(test_Given_null_lhs_and_nonnull_rhs_When_make_branch_node_Then_input_rhs_is_set_as_output_lhs)
{
	struct mrope_leaf_node * rhs = mrope_make_leaf_node();

	struct mrope_branch_node *node = mrope_make_branch_node(NULL, (struct mrope_node *)rhs);

	ck_assert_ptr_eq(node->left, rhs);
}
END_TEST

START_TEST(test_Given_null_lhs_and_nonnull_rhs_When_make_branch_node_Then_rhs_is_null)
{
	struct mrope_leaf_node * lhs = NULL;
	struct mrope_leaf_node * rhs = mrope_make_leaf_node();

	struct mrope_branch_node *node = mrope_make_branch_node((struct mrope_node *)lhs, (struct mrope_node *)rhs);

	ck_assert_ptr_eq(node->right, NULL);
}
END_TEST

START_TEST(test_rope_make_leaf_node)
{
	struct mrope_leaf_node *node = mrope_make_leaf_node();

	ck_assert_ptr_ne(node, NULL);
	ck_assert_int_eq(node->node.type, MROPE_NODE_LEAF);
	ck_assert_ptr_eq(node->text, NULL);
}
END_TEST

START_TEST(test_rope_make_leaf_node_from_text)
{
	char * text = strdup("foo");
	struct mrope_leaf_node *leaf_node = NULL;

	mreturn_t error = MROPE_OK;

	error = mrope_make_leaf_node_from_text(&leaf_node, text);

	ck_assert_int_eq(error, MROPE_OK);
	ck_assert_ptr_ne(leaf_node, NULL);
	ck_assert_int_eq(leaf_node->node.type, MROPE_NODE_LEAF);
	ck_assert_ptr_ne(leaf_node->text, NULL);
}
END_TEST

START_TEST(test_rope_make_leaf_node_from_text_SHALL_own_text)
{
	char * text = strdup("foo");
	struct mrope_leaf_node *leaf_node = NULL;

	mreturn_t error = MROPE_OK;

	error = mrope_make_leaf_node_from_text(&leaf_node, text);

	ck_assert_int_eq(error, MROPE_OK);
	ck_assert_ptr_eq(leaf_node->text, text);
}
END_TEST

START_TEST(test_rope_free_leaf_node)
{
	struct mrope_leaf_node *node = mrope_make_leaf_node();

	enum mrope_error_code error = MROPE_OK;

	error = mrope_free_node( (struct mrope_node *)node);

	ck_assert_int_eq(error, MROPE_OK);
}
END_TEST


START_TEST(test_rope_free_branch_node)
{
	struct mrope_branch_node *node = mrope_make_branch_node(NULL, NULL);

	enum mrope_error_code error = MROPE_OK;

	error = mrope_free_node((struct mrope_node *)node);

	ck_assert_int_eq(error, MROPE_OK);
}
END_TEST


START_TEST(test_rope_index_leaf_node)
{
	struct mrope_leaf_node *node = mrope_make_leaf_node();
	node->text = strdup("abcdef");
	node->node.weight = strlen(node->text);

	ck_assert_int_eq(mrope_index_leaf_node(node,0), 'a');
	ck_assert_int_eq(mrope_index_leaf_node(node,5), 'f');
	ck_assert_int_eq(mrope_index_leaf_node(node,6), '\0');
	ck_assert_int_eq(mrope_index_leaf_node(node,60), '\0');
}
END_TEST


START_TEST(test_mrope_split_leaf_node_Shall_return_lhs_with_index_length)
{
	/* Given */
	struct mrope_leaf_node original;
	char * text = "0123456789";
	mrope_init_leaf_node(&original, text, 10);

	struct mrope_leaf_node lhs;

	struct mrope_leaf_node rhs;

	const size_t split_length = 4;

	/* When */
	mrope_split_leaf_node(&original, split_length, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq(lhs.node.weight, split_length);
}
END_TEST


START_TEST(test_mrope_split_leaf_node_Shall_return_rhs_with_index_length)
{
	/* Given */
	struct mrope_leaf_node original;
	char * text = "0123456789";
	mrope_init_leaf_node(&original, text, 10);

	struct mrope_leaf_node lhs;

	struct mrope_leaf_node rhs;

	const size_t split_length = 4;

	/* When */
	mrope_split_leaf_node(&original, split_length, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq(rhs.node.weight, 10-split_length);
}
END_TEST


START_TEST(test_mrope_split_leaf_node_Shall_return_rhs_with_first_index)
{
	/* Given */
	struct mrope_leaf_node original;
	char * text = "0123456789";
	mrope_init_leaf_node(&original, text, 10);

	struct mrope_leaf_node lhs;

	struct mrope_leaf_node rhs;

	const size_t split_length = 4;

	/* When */
	mrope_split_leaf_node(&original, split_length, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq(rhs.text[0], original.text[split_length]);
}
END_TEST


START_TEST(test_rope_index_branch_node)
{
	struct mrope_leaf_node *left_node = mrope_make_leaf_node();
	left_node->text = strdup("abcdef");
	left_node->node.weight = strlen(left_node->text);

	struct mrope_leaf_node *right_node = mrope_make_leaf_node();
	right_node->text = strdup("ghijkl");
	right_node->node.weight = strlen(right_node->text);

	struct mrope_branch_node * branch_node = mrope_make_branch_node((struct mrope_node *)left_node, (struct mrope_node *)right_node);

	ck_assert_int_eq(mrope_index_branch_node(branch_node,0), 'a');
	ck_assert_int_eq(mrope_index_branch_node(branch_node,5), 'f');
	ck_assert_int_eq(mrope_index_branch_node(branch_node,6), 'g');
	ck_assert_int_eq(mrope_index_branch_node(branch_node,7), 'h');
	ck_assert_int_eq(mrope_index_branch_node(branch_node,11), 'l');
	ck_assert_int_eq(mrope_index_branch_node(branch_node,12), '\0');
}
END_TEST


START_TEST(test_Given_empty_leaf_node_When_clone_leaf_node_Then_error_MROPE_OK)
{
	/* Given */
	struct mrope_leaf_node * original_node = mrope_make_leaf_node();
	struct mrope_leaf_node cloned_node;
	mreturn_t error = MROPE_OK;

	/* When */
	error = mrope_clone_leaf_node(original_node, &cloned_node);

	/* Then */
	ck_assert_int_eq(error, MROPE_OK);
}
END_TEST


START_TEST(test_Given_empty_leaf_node_When_clone_leaf_node_Then_cloned_node_is_leaf_node)
{
	/* Given */
	struct mrope_leaf_node * original_node = mrope_make_leaf_node();
	struct mrope_leaf_node cloned_node;

	/* When */
	mrope_clone_leaf_node(original_node, &cloned_node);

	/* Then */
	ck_assert_int_eq(cloned_node.node.type, MROPE_NODE_LEAF);
}
END_TEST


START_TEST(test_Given_empty_leaf_node_When_clone_leaf_node_Then_cloned_node_weight_is_zero)
{
	/* Given */
	struct mrope_leaf_node * original_node = mrope_make_leaf_node();
	struct mrope_leaf_node cloned_node;

	/* When */
	mrope_clone_leaf_node(original_node, &cloned_node);

	/* Then */
	ck_assert_int_eq(cloned_node.node.weight, 0);
}
END_TEST


Suite * mropes_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("mropes core ops");

	/* Core test case */
	tc_core = tcase_create("Core ops");

	tcase_add_test(tc_core, test_rope_make_branch_node_Shall_return_node_branch);
	tcase_add_test(tc_core, test_rope_make_branch_node_Shall_return_null_LHS);
	tcase_add_test(tc_core, test_Given_null_lhs_and_nonnull_rhs_When_make_branch_node_Then_input_rhs_is_set_as_output_lhs);
	tcase_add_test(tc_core, test_Given_null_lhs_and_nonnull_rhs_When_make_branch_node_Then_rhs_is_null);
	tcase_add_test(tc_core, test_mrope_split_leaf_node_Shall_return_lhs_with_index_length);
	tcase_add_test(tc_core, test_mrope_split_leaf_node_Shall_return_rhs_with_index_length);
	tcase_add_test(tc_core, test_mrope_split_leaf_node_Shall_return_rhs_with_first_index);
	tcase_add_test(tc_core, test_rope_index_branch_node);
	tcase_add_test(tc_core, test_rope_make_leaf_node);
	tcase_add_test(tc_core, test_rope_make_leaf_node_from_text);
	tcase_add_test(tc_core, test_rope_make_leaf_node_from_text_SHALL_own_text);
	tcase_add_test(tc_core, test_rope_free_leaf_node);
	tcase_add_test(tc_core, test_rope_free_branch_node);
	tcase_add_test(tc_core, test_rope_index_leaf_node);
	tcase_add_test(tc_core, test_Given_empty_leaf_node_When_clone_leaf_node_Then_error_MROPE_OK);
	tcase_add_test(tc_core, test_Given_empty_leaf_node_When_clone_leaf_node_Then_cloned_node_is_leaf_node);
	tcase_add_test(tc_core, test_Given_empty_leaf_node_When_clone_leaf_node_Then_cloned_node_weight_is_zero);

	suite_add_tcase(s, tc_core);

	return s;
}

int main(void)
{
	int number_failed = 0;
	Suite *s;
	SRunner *sr;

	s = mropes_suite();
	sr = srunner_create(s);

	/*
	 * Adds nofork to enable debugging tests
	 * http://check.sourceforge.net/doc/check%5Fhtml/check%5F4.html#No-Fork-Mode
	 */
	srunner_set_fork_status (sr, CK_NOFORK);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


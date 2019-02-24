#include <stdlib.h>
#include <check.h>

#include "src/rope_ops.h"
#include "src/rope_stack.h"

START_TEST(test_rope_stack_init)
{
	struct mrope_stack stack;

	mrope_stack_init(&stack);

	ck_assert_ptr_ne(stack.data, NULL);
	ck_assert_uint_gt(stack.size, 0);
}
END_TEST


START_TEST(test_rope_stack_grow)
{
	struct mrope_stack stack;

	mrope_stack_init(&stack);

	size_t old_size = stack.size;

	mrope_stack_grow_stack(&stack);

	ck_assert_uint_gt(stack.size, old_size);
}
END_TEST


START_TEST(test_rope_stack_push)
{
	struct mrope_stack stack;

	mrope_stack_init(&stack);

	struct mrope_node *node = (struct mrope_node *)mrope_make_leaf_node();
	ck_assert_ptr_ne(node, NULL);

	enum mrope_error_code error = MROPE_OK;

	error = mrope_stack_push(&stack, node);
	ck_assert_int_eq(error, MROPE_OK);

	struct mrope_node *top_node = mrope_stack_top(&stack);
	ck_assert_ptr_eq(top_node, node);
}
END_TEST

START_TEST(test_rope_stack_pop)
{
	struct mrope_stack stack;

	mrope_stack_init(&stack);

	struct mrope_node *node = NULL;
	node = (struct mrope_node *)mrope_make_leaf_node();
	ck_assert_ptr_ne(node, NULL);
	enum mrope_error_code error = MROPE_OK;

	error = mrope_stack_push(&stack, node);
	ck_assert_int_eq(error, MROPE_OK);

	struct mrope_node *popped_node = NULL;
	error = mrope_stack_pop(&stack, &popped_node);
	ck_assert_int_eq(error, MROPE_OK);
	ck_assert_ptr_eq(node, popped_node);
}
END_TEST

START_TEST(test_rope_stack_top)
{
	struct mrope_stack stack;

	mrope_stack_init(&stack);

	struct mrope_node *node = (struct mrope_node *)mrope_make_leaf_node();
	ck_assert_ptr_ne(node, NULL);

	enum mrope_error_code error = MROPE_OK;

	struct mrope_node *top_node = NULL;

	top_node = mrope_stack_top(&stack);
	ck_assert_ptr_eq(top_node, NULL);

	error = mrope_stack_push(&stack, node);
	ck_assert_int_eq(error, MROPE_OK);

	top_node = mrope_stack_top(&stack);
	ck_assert_ptr_eq(top_node, node);
}
END_TEST


Suite * mropes_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("mropes stack");

	/* Core test case */
	tc_core = tcase_create("Core ops");

	tcase_add_test(tc_core, test_rope_stack_init);
	tcase_add_test(tc_core, test_rope_stack_grow);
	tcase_add_test(tc_core, test_rope_stack_push);
	tcase_add_test(tc_core, test_rope_stack_pop);
	tcase_add_test(tc_core, test_rope_stack_top);

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


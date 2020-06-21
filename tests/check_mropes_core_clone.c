#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <mropes/rope.h>

const char * const original_text = "0123456789";

START_TEST(Given_empty_rope_When_mrope_clone_Then_cloned_rope_has_equal_length)
{
	/* Given */
	mrope_t original_rope;
	mrope_init(&original_rope);

	/* When */
	mrope_t cloned_rope;
	mrope_clone(&original_rope, &cloned_rope);

	/* Then */
	ck_assert_int_eq(mrope_length(&original_rope), mrope_length(&cloned_rope));
}
END_TEST


START_TEST(Given_rope_with_one_leaf_node_When_mrope_clone_Then_cloned_rope_has_equal_length)
{
	/* Given */
	mrope_t original_rope;
	mrope_init(&original_rope);
	mrope_append_text(&original_rope, strdup(original_text));

	/* When */
	mrope_t cloned_rope;
	mrope_clone(&original_rope, &cloned_rope);

	/* Then */
	ck_assert_int_eq(mrope_length(&original_rope), mrope_length(&cloned_rope));
}
END_TEST

START_TEST(Given_rope_with_two_leaf_nodes_When_mrope_clone_Then_cloned_rope_has_equal_length)
{
	/* Given */
	mrope_t original_rope;
	mrope_init(&original_rope);
	mrope_append_text(&original_rope, strdup(original_text));
	mrope_append_text(&original_rope, strdup(original_text));

	/* When */
	mrope_t cloned_rope;
	mrope_clone(&original_rope, &cloned_rope);

	/* Then */
	ck_assert_int_eq(mrope_length(&original_rope), mrope_length(&cloned_rope));
}
END_TEST


Suite * mropes_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("mropes");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, Given_empty_rope_When_mrope_clone_Then_cloned_rope_has_equal_length);
	tcase_add_test(tc_core, Given_rope_with_one_leaf_node_When_mrope_clone_Then_cloned_rope_has_equal_length);
	tcase_add_test(tc_core, Given_rope_with_two_leaf_nodes_When_mrope_clone_Then_cloned_rope_has_equal_length);

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


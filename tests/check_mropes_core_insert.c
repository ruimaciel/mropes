#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <mropes/rope.h>

const char * const original_text = "0123456789";

START_TEST(test_mrope_insert_Given_insert_When_index_out_of_range_Then_throw_out_of_range_result)
{
	/* Given */
	mrope_t receiving_rope;
	mrope_init(&receiving_rope);

	mrope_t inserted_rope;
	mrope_init(&inserted_rope);

	/* When */
	mreturn_t error = mrope_insert(&receiving_rope, mrope_length(&receiving_rope)+1, &inserted_rope);

	/* Then */
	ck_assert_int_eq( error, MROPE_OUT_OF_RANGE);
}
END_TEST


START_TEST(test_mrope_insert_Given_insert_When_index_is_middle_of_leaf_node_Then_length_is_incremented)
{
	/* Given */
	mrope_t receiving_rope;
	mrope_init(&receiving_rope);
	mrope_append_text(&receiving_rope, strdup(original_text));

	mrope_t inserted_rope;
	mrope_init(&inserted_rope);
	mrope_append_text(&inserted_rope, strdup(original_text));

	/* When */
	mrope_insert(&receiving_rope, 4, &inserted_rope);

	/* Then */
	ck_assert_int_eq( mrope_length(&receiving_rope), 2*strlen(original_text));
}
END_TEST


START_TEST(test_mrope_insert_Given_insert_When_index_is_in_last_leaf_node_Then_length_is_incremented)
{
	/* Given */
	const size_t original_text_length = strlen(original_text);

	mrope_t receiving_rope;
	mrope_init(&receiving_rope);
	mrope_append_text(&receiving_rope, strdup(original_text));
	mrope_append_text(&receiving_rope, strdup(original_text));

	mrope_t inserted_rope;
	mrope_init(&inserted_rope);
	mrope_append_text(&inserted_rope, strdup(original_text));

	/* When */
	mrope_insert(&receiving_rope, original_text_length+4, &inserted_rope);

	/* Then */
	ck_assert_int_eq( mrope_length(&receiving_rope), 3*original_text_length);
}
END_TEST


Suite * mropes_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("mropes");

	/* Core test case */
	tc_core = tcase_create("Core mrope_insert");

	tcase_add_test(tc_core, test_mrope_insert_Given_insert_When_index_out_of_range_Then_throw_out_of_range_result);
	tcase_add_test(tc_core, test_mrope_insert_Given_insert_When_index_is_middle_of_leaf_node_Then_length_is_incremented);
	tcase_add_test(tc_core, test_mrope_insert_Given_insert_When_index_is_in_last_leaf_node_Then_length_is_incremented);

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


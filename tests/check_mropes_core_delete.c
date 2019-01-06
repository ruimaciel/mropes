#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <mropes/rope.h>

const char * const original_text = "0123456789";

START_TEST(test_Given_empty_rope_When_deleting_any_index_Then_return_OUT_OF_RANGE)
{
	/* Given */
	mreturn_t error = MROPE_OK;
	mrope_t rope;
	mrope_init(&rope);

	/* When */
	error = mrope_delete(&rope, 0, 1);

	/* Then */
	ck_assert_int_eq(error, MROPE_OUT_OF_RANGE);
}
END_TEST


START_TEST(test_Given_rope_with_text_When_deleting_index_out_of_range_Then_return_OUT_OF_RANGE)
{
	/* Given */
	mreturn_t error = MROPE_OK;
	mrope_t rope;
	mrope_init(&rope);
	mrope_append_text(&rope, strdup(original_text));

	/* When */
	error = mrope_delete(&rope, 20, 1);

	/* Then */
	ck_assert_int_eq(error, MROPE_OUT_OF_RANGE);
}
END_TEST


START_TEST(test_Given_rope_with_text_When_deleting_length_out_of_range_Then_return_OUT_OF_RANGE)
{
	/* Given */
	mreturn_t error = MROPE_OK;
	mrope_t rope;
	mrope_init(&rope);
	mrope_append_text(&rope, strdup(original_text));

	/* When */
	error = mrope_delete(&rope, 20, 1);

	/* Then */
	ck_assert_int_eq(error, MROPE_OUT_OF_RANGE);
}
END_TEST


START_TEST(test_Given_rope_with_text_When_deleting_length_in_range_Then_return_OK)
{
	/* Given */
	mreturn_t error = MROPE_OK;
	mrope_t rope;
	mrope_init(&rope);
	mrope_append_text(&rope, strdup(original_text));
	mrope_append_text(&rope, strdup(original_text));

	/* When */
	error = mrope_delete(&rope, 10, 4);

	/* Then */
	ck_assert_int_eq(error, MROPE_OK);
}
END_TEST


START_TEST(test_Given_rope_with_text_When_deleting_length_in_range_Then_length_is_smaller)
{
	/* Given */
	mreturn_t error = MROPE_OK;
	mrope_t rope;
	mrope_init(&rope);
	mrope_append_text(&rope, strdup(original_text));
	mrope_append_text(&rope, strdup(original_text));

	/* When */
	mrope_delete(&rope, 10, 4);

	/* Then */
	ck_assert_int_eq(mrope_length(&rope), 20-4);
}
END_TEST


Suite * mropes_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("mropes");

	/* Core test case */
	tc_core = tcase_create("Core mrope_delete");

	tcase_add_test(tc_core, test_Given_empty_rope_When_deleting_any_index_Then_return_OUT_OF_RANGE);
	tcase_add_test(tc_core, test_Given_rope_with_text_When_deleting_index_out_of_range_Then_return_OUT_OF_RANGE);
	tcase_add_test(tc_core, test_Given_rope_with_text_When_deleting_length_out_of_range_Then_return_OUT_OF_RANGE);
	tcase_add_test(tc_core, test_Given_rope_with_text_When_deleting_length_in_range_Then_return_OK);
	tcase_add_test(tc_core, test_Given_rope_with_text_When_deleting_length_in_range_Then_length_is_smaller);

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


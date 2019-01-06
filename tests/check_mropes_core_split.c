#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <mropes/rope.h>

START_TEST(test_mrope_split_When_index_out_of_range_Shall_return_out_of_range_error)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);

	/* When */
	mreturn_t error = mrope_split(&input, 10, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( error, MROPE_OUT_OF_RANGE);
}
END_TEST


START_TEST(test_mrope_split_When_index_out_of_range_Shall_return_lhs_length_zero)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);

	/* When */
	 mrope_split(&input, 10, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( mrope_length(&lhs), 0);
}
END_TEST


START_TEST(test_mrope_split_When_index_out_of_range_Shall_return_rhs_length_zero)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);

	/* When */
	 mrope_split(&input, 10, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( mrope_length(&rhs), 0);
}
END_TEST


START_TEST(test_mrope_split_When_index_zero_Shall_return_empth_lhs)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);
	mrope_append_text(&input,"foo");

	/* When */
	mrope_split(&input, 0, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( mrope_length(&lhs), 0);
}
END_TEST


START_TEST(test_mrope_split_When_index_zero_Shall_return_lhs_with_input_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);
	mrope_append_text(&input,"foo");

	/* When */
	mrope_split(&input, 0, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( mrope_length(&rhs), mrope_length(&input));
}
END_TEST


START_TEST(test_mrope_split_When_index_mid_node_Shall_return_lhs_with_split_index_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);
	mrope_append_text(&input,"01234567890123456789");
	mrope_append_text(&input,"01234567890123456789");

	/* When */
	const size_t split_index = 25;
	mrope_split(&input, split_index, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( mrope_length(&lhs), split_index);
	ck_assert_int_eq( mrope_length(&rhs), mrope_length(&input)-split_index);
}
END_TEST


START_TEST(test_mrope_split_When_index_mid_node_Shall_return_rhs_with_length_minus_split_index_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);
	mrope_append_text(&input,"01234567890123456789");
	mrope_append_text(&input,"01234567890123456789");

	/* When */
	const size_t split_index = 25;
	mrope_split(&input, split_index, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( mrope_length(&rhs), mrope_length(&input)-split_index);
}
END_TEST


START_TEST(test_mrope_split_When_index_second_node_start_Shall_return_rhs_with_length_minus_split_index_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	mrope_t input;
	mrope_init(&input);
	mrope_append_text(&input,"01234567890123456789");
	mrope_append_text(&input,"01234567890123456789");

	/* When */
	const size_t split_index = 20;
	mrope_split(&input, split_index, &lhs, &rhs);

	/* Then */
	ck_assert_int_eq( mrope_length(&rhs), mrope_length(&input)-split_index);
}
END_TEST



Suite * mropes_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("mropes");

	/* Core test case */
	tc_core = tcase_create("Core mrope_split");

	tcase_add_test(tc_core, test_mrope_split_When_index_out_of_range_Shall_return_out_of_range_error);
	tcase_add_test(tc_core, test_mrope_split_When_index_out_of_range_Shall_return_lhs_length_zero);
	tcase_add_test(tc_core, test_mrope_split_When_index_out_of_range_Shall_return_rhs_length_zero);
	tcase_add_test(tc_core, test_mrope_split_When_index_zero_Shall_return_empth_lhs);
	tcase_add_test(tc_core, test_mrope_split_When_index_zero_Shall_return_lhs_with_input_length);
	tcase_add_test(tc_core, test_mrope_split_When_index_mid_node_Shall_return_lhs_with_split_index_length);
	tcase_add_test(tc_core, test_mrope_split_When_index_mid_node_Shall_return_rhs_with_length_minus_split_index_length);
	tcase_add_test(tc_core, test_mrope_split_When_index_second_node_start_Shall_return_rhs_with_length_minus_split_index_length);

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


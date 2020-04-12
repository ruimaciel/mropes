#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <mropes/rope.h>

START_TEST(test_rope_init)
{
	mrope_t rope;
	mrope_init(&rope);

	ck_assert_ptr_eq(rope.root_node, NULL);
}
END_TEST


START_TEST(test_rope_when_prepend_rope_then_lhs_is_null)
{
	mrope_t rope_lhs;
	mrope_init(&rope_lhs);

	mrope_t rope_rhs;
	mrope_init(&rope_rhs);

	mrope_prepend_rope(&rope_rhs, &rope_lhs);

	ck_assert_ptr_eq(rope_lhs.root_node, NULL);
}
END_TEST


START_TEST(test_rope_prepend_rope)
{
	mrope_t rope_lhs;
	mrope_init(&rope_lhs);
	mrope_append_text(&rope_lhs, "foo");

	mrope_t rope_rhs;
	mrope_init(&rope_rhs);
	mrope_append_text(&rope_lhs, "bar");

	mrope_prepend_rope(&rope_lhs, &rope_rhs);

	ck_assert_ptr_ne(rope_lhs.root_node, NULL);
}
END_TEST

START_TEST(test_rope_When_prepend_rope_Then_length_is_equal_to_the_sum_of_both_ropes)
{
	/* Given */
	mrope_t rope_lhs;
	mrope_init(&rope_lhs);
	mrope_append_text(&rope_lhs, "foo");
	const size_t lhs_size = mrope_length(&rope_lhs);

	mrope_t rope_rhs;
	mrope_init(&rope_rhs);
	mrope_append_text(&rope_rhs, "bar");
	const size_t rhs_size = mrope_length(&rope_rhs);

	/* When */
	mrope_prepend_rope(&rope_lhs, &rope_rhs);

	/* Then */
	size_t size = mrope_length(&rope_lhs);
	ck_assert_int_eq(size, lhs_size+rhs_size);
}
END_TEST


START_TEST(test_rope_append_text)
{
	mrope_t rope;
	mrope_init(&rope);

	mrope_append_text(&rope, strdup("foo"));

	ck_assert_ptr_ne(rope.root_node, NULL);
}
END_TEST


START_TEST(test_rope_append_rope)
{
	mrope_t rope;
	mrope_init(&rope);

	mrope_t rope2;
	mrope_init(&rope2);

	mrope_append_rope(&rope, &rope2);

	ck_assert_ptr_eq(rope.root_node, NULL);
}
END_TEST


START_TEST(test_mrope_concat_When_empty_lhs_and_rhs_Shall_have_zero_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);

	/* When */
	mrope_t out;
	mrope_init(&out);

	mrope_concat(&lhs, &rhs, &out);

	/* Then */
	ck_assert_int_eq( mrope_length(&out), 0);
}
END_TEST


START_TEST(test_mrope_concat_When_empty_rhs_Shall_have_lhs_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);
	mrope_append_text(&lhs,"foo");

	mrope_t rhs;
	mrope_init(&rhs);

	/* When */
	mrope_t out;
	mrope_init(&out);

	mrope_concat(&lhs, &rhs, &out);

	/* Then */
	ck_assert_int_eq( mrope_length(&out), mrope_length(&lhs));
}
END_TEST


START_TEST(test_mrope_concat_When_nonempty_lhs_and_rhs_Shall_have_rhs_and_lhs_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);
	mrope_append_text(&lhs,"foo");

	mrope_t rhs;
	mrope_init(&rhs);
	mrope_append_text(&rhs,"bar");

	/* When */
	mrope_t out;
	mrope_init(&out);

	mrope_concat(&lhs, &rhs, &out);

	/* Then */
	ck_assert_int_eq( mrope_length(&out), mrope_length(&lhs)+mrope_length(&rhs));
}
END_TEST


START_TEST(test_mrope_concat_When_empty_lhs_Shall_have_rhs_length)
{
	/* Given */
	mrope_t lhs;
	mrope_init(&lhs);

	mrope_t rhs;
	mrope_init(&rhs);
	mrope_append_text(&rhs,"foo");

	/* When */
	mrope_t out;
	mrope_init(&out);

	mrope_concat(&lhs, &rhs, &out);

	/* Then */
	ck_assert_int_eq( mrope_length(&out), mrope_length(&rhs));
}
END_TEST


START_TEST(test_rope_length_when_init_shall_return_zero)
{
	mrope_t rope;
	mrope_init(&rope);

	ck_assert_uint_eq(mrope_length(&rope), 0);
}
END_TEST


START_TEST(test_rope_length_when_append_shall_return_input_text_length)
{
	mrope_t rope;
	mrope_init(&rope);
	char * text = strdup("0123456789");

	mrope_append_text(&rope, text);

	ck_assert_uint_eq(mrope_length(&rope), 10);
}
END_TEST


START_TEST(test_rope_length_when_append_twice_shall_return_sum_of_input_text_lengths)
{
	mrope_t rope;
	mrope_init(&rope);
	char * text = "0123456789";
	const size_t text_size = strlen(text);
	char * text_lhs = strdup(text);
	char * text_rhs = strdup(text);

	mrope_append_text(&rope, text_lhs);
	mrope_append_text(&rope, text_rhs);

	ck_assert_uint_eq(mrope_length(&rope), 2*text_size);
}
END_TEST


START_TEST(test_rope_free)
{
	mrope_t rope;
	mrope_init(&rope);

	mrope_t rope2;
	mrope_init(&rope2);

	mrope_append_rope(&rope, &rope2);

	mrope_free(&rope);

	ck_assert_ptr_eq(rope.root_node, NULL);
}
END_TEST


Suite * mropes_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("mropes");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_rope_init);
	tcase_add_test(tc_core, test_rope_when_prepend_rope_then_lhs_is_null);
	tcase_add_test(tc_core, test_rope_When_prepend_rope_Then_length_is_equal_to_the_sum_of_both_ropes);
	tcase_add_test(tc_core, test_rope_prepend_rope);
	tcase_add_test(tc_core, test_rope_append_text);
	tcase_add_test(tc_core, test_rope_append_rope);
	tcase_add_test(tc_core, test_mrope_concat_When_empty_lhs_and_rhs_Shall_have_zero_length);
	tcase_add_test(tc_core, test_mrope_concat_When_empty_rhs_Shall_have_lhs_length);
	tcase_add_test(tc_core, test_mrope_concat_When_nonempty_lhs_and_rhs_Shall_have_rhs_and_lhs_length);
	tcase_add_test(tc_core, test_mrope_concat_When_empty_lhs_Shall_have_rhs_length);
	tcase_add_test(tc_core, test_rope_free);
	tcase_add_test(tc_core, test_rope_length_when_init_shall_return_zero);
	tcase_add_test(tc_core, test_rope_length_when_append_shall_return_input_text_length);
	tcase_add_test(tc_core, test_rope_length_when_append_twice_shall_return_sum_of_input_text_lengths);

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


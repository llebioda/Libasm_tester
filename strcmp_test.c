#include "libasm_tester.h"

extern int ft_strcmp(const char *s1, const char *s2);
extern size_t ft_strlen(const char *s);

static t_bool all_success = TRUE;

static t_bool strcmp_is_valid(const char *s1, const char *s2, const int res, const int ft_res)
{
    if (res != ft_res)
    {
        printf(RED "[STRCMP] diff for [%s]_[%s] : %d != %d" RESET "\n",
            get_printable_str(s1), get_printable_str(s2), res, ft_res);
        return FALSE;
    }

    return TRUE;
}

static void strcmp_test(const char *s1, const char *s2)
{
    int res;
    if (s1 == NULL && s2 == NULL)
        res = 0;
    else if (s1 == NULL)
        res = -1;
    else if (s2 == NULL)
        res = 1;
    else
        res = strcmp(s1, s2);

    int ft_res = ft_strcmp(s1, s2);

    if (!strcmp_is_valid(s1, s2, res, ft_res))
        all_success = FALSE;
}

static void strcmp_benchmark(const char *s1, const char *s2)
{
    clock_t start, end;
    double time_lib = 0.0, time_ft = 0.0;

    for (int i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        start = clock();
        int res = strcmp(s1, s2);
        end = clock();
        time_lib += ((double)(end - start)) / 1000;

        start = clock();
        int ft_res = ft_strcmp(s1, s2);
        end = clock();
        time_ft += ((double)(end - start)) / 1000;

        if (!strcmp_is_valid(s1, s2, res, ft_res))
        {
            all_success = FALSE;
            printf(YELLOW "Benchmark cancelled" RESET "\n");
            return;
        }
    }

    calculate_efficiency("strcmp", time_lib, time_ft);
}

t_bool strcmp_tester(void)
{
    all_success = TRUE;
    printf("\n" PURPLE " ***** STRCMP *****" RESET "\n\n");

    /* TEST */
    strcmp_test("", "");
    strcmp_test("a", "a");
    strcmp_test("a", "b");
    strcmp_test("abc", "abc");
    strcmp_test("abc", "abd");
    strcmp_test("abc", "ab");
    strcmp_test("ab", "abc");

    strcmp_test(" ", " ");
    strcmp_test(" ", "");
    strcmp_test("abc ", "abc");
    strcmp_test(" abc", "abc");
    strcmp_test("abc", " abc");

    strcmp_test("abc", "ABC");
    strcmp_test("ABC", "abc");
    strcmp_test("aBc", "AbC");
    strcmp_test("abc", "aBc");

    strcmp_test("123", "123");
    strcmp_test("123", "124");
    strcmp_test("123", "12");
    strcmp_test("12", "123");
    strcmp_test("123", "023");

    strcmp_test("!", "!");
    strcmp_test("!", "?");
    strcmp_test("@#$%", "@#$%");
    strcmp_test("@#$%", "@#$&");
    strcmp_test("@#$%", "#$%");

    strcmp_test("hello\n", "hello");
    strcmp_test("hello\0", "hello");
    strcmp_test("hello", "hello\0\0");
    strcmp_test("hello", "hello\n");
    strcmp_test("\tHello", "Hello");
    strcmp_test("Hello\t", "Hello ");

    strcmp_test("abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz");
    strcmp_test("abcdefghijklmnoqrstuvwxyz", "abcdefghijklmnopqrstuvwxy");
    strcmp_test("A very long string with multiple characters", "A very long string with multiple character");

    strcmp_test("café", "cafe");
    strcmp_test("ñandú", "nandu");
    strcmp_test("你好", "你好");
    strcmp_test("你好", "您好");

    strcmp_test(NULL, "abc");
    strcmp_test("abc", NULL);
    strcmp_test(NULL, NULL);

    strcmp_test("hello", "hell\0o");
    strcmp_test("apple\0pie", "apple");
    strcmp_test("", "\0");
    strcmp_test("\0", "\0");
    strcmp_test("\0abc", "abc");

    strcmp_test(A_1_000, A_1_000);
    strcmp_test(A_1_000, B_1_000);
    strcmp_test(A_1_000"B", A_10_000);
    strcmp_test(A_10_000, A_10_000"A");
    strcmp_test(A_10_000, "A"B_1_000);
    strcmp_test(A_10_000"B", A_10_000"C");
    strcmp_test(A_10_000, B_1_000);
    strcmp_test(A_1_000_000_000, A_1_000_000_000);

    /* BENCHMARK */
    strcmp_benchmark("", "");
    strcmp_benchmark("a", "a");
    strcmp_benchmark("a", "b");
    strcmp_benchmark("hello\0", "hello");
    strcmp_benchmark("abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz");
    strcmp_benchmark("ñandú", "nandu");
    strcmp_benchmark(A_1_000, A_1_000);
    strcmp_benchmark(A_1_000, B_1_000);
    strcmp_benchmark(A_10_000, A_10_000"A");

    return all_success;
}
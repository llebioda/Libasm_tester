#include "libasm_tester.h"

extern char *ft_strdup(const char *s);

static t_bool all_success = TRUE;

static t_bool strdup_is_valid(const char *src, const char *res, const int res_errno)
{
    // Both are NULL, nothing to do
    if (src == NULL && res == NULL)
        return TRUE;

    if (src != NULL && res == NULL)
    {
        // Verify expected errno value
        if (ENOMEM != res_errno)
        {
            printf(RED "[STRDUP] Errno value mismatch on [%s]\nexpected : %d\ngot > ft : %d" RESET "\n",
                get_printable_str(src), ENOMEM, res_errno);
            return FALSE;
        }
        return TRUE;
    }

    if (src == NULL && res != NULL)
    {
        printf(RED "[STRDUP] Wanted NULL pointer on [%s], but got: %p (%s)" RESET "\n",
            get_printable_str(src), res, res);
        return FALSE;
    }

    if (src == res)
    {
        printf(RED "[STRDUP] Returned pointer value is the same as the one given as parameter : %p == %p" RESET "\n", src, res);
        return FALSE;
    }

    if (strcmp(src, res) != 0)
    {
        printf(RED "[STRDUP] Diff [%s] != [%s] (len : %zu | %zu)" RESET "\n",
            get_printable_str(src), get_printable_str(res), strlen(src), strlen(res));
        return FALSE;
    }

    return TRUE;
}

static void strdup_test(const char *s)
{
    char *res = ft_strdup(s);

    if (!strdup_is_valid(s, res, errno))
        all_success = FALSE;

    free(res);
}

static void strdup_benchmark(const char *s)
{
    clock_t start, end;
    double time_lib = 0.0, time_ft = 0.0;

    for (int i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        start = clock();
        char *res = strdup(s);
        end = clock();
        time_lib += ((double)(end - start)) / 1000;

        start = clock();
        char *ft_res = ft_strdup(s);
        end = clock();
        int ft_res_errno = errno;
        time_ft += ((double)(end - start)) / 1000;

        if (!strdup_is_valid(s, ft_res, ft_res_errno))
        {
            all_success = FALSE;
            printf(YELLOW "Benchmark cancelled" RESET "\n");
            free(res);
            free(ft_res);
            return;
        }
        free(res);
        free(ft_res);
    }

    calculate_efficiency("strdup", time_lib, time_ft);
}

t_bool strdup_tester(void)
{
    all_success = TRUE;
    printf("\n" PURPLE " ***** STRDUP *****" RESET "\n\n");

    /* TEST */
    strdup_test(NULL);
    strdup_test("");
    strdup_test("a");
    strdup_test("aa");
    strdup_test("ab");
    strdup_test("abc");
    strdup_test("abcdef");

    strdup_test("hello");
    strdup_test("Hello, world!");
    strdup_test("Hello");
    strdup_test("Hello, \n\n\n");
    strdup_test("Hello, \n\n\n\0salut");
    strdup_test("abc  \t..?.?>%<");
    strdup_test("abfewfewfwcweewfwefwefwexwedefec");

    strdup_test("The quick brown fox jumps over the lazy dog");
    strdup_test("Lorem ipsum dolor sit amet");
    strdup_test("Lorem ipsum");

    strdup_test("Test\0Hidden");
    strdup_test("Another test");
    strdup_test("Edge case: buffer equals string length");
    strdup_test("Buffer larger than needed");
    strdup_test("Line1\nLine2");
    strdup_test("Tab\tSeparated");
    strdup_test("Spaces are important");
    strdup_test("Trailing spaces   ");
    strdup_test(" ");
    strdup_test("  ");
    strdup_test("                                                                            ");
    strdup_test("Special chars: !@#$%^&*()");
    strdup_test("Non-ASCII: üñîçødé");

    {
        char s[] = "\0";
        strdup_test(s);
    }

    {
        char s[] = "\0Hello";
        strdup_test(s);
    }

    {
        char s[] = "Hello\0";
        strdup_test(s);
    }

    strdup_test("0123456789");
    strdup_test("12345678901234567890");
    strdup_test("Short");
    strdup_test("End");
    strdup_test("End with null");
    strdup_test("Another example");
    strdup_test("Trailing newline\n");
    strdup_test("Multiple\nnewlines\nin\nstring");
    strdup_test("This is a longer test string to push the boundaries of the function");
    strdup_test("A much longer than usual string which should be handled correctly even if the buffer is huge");
    strdup_test("With escape sequences: \n\t\r");
    strdup_test("Test#with#hashes");
    strdup_test("Numbers1234567890");
    strdup_test("MixedCASEand123");
    strdup_test("Symbols: []{}<>");

    strdup_test(A_1_000);
    strdup_test(B_1_000);
    strdup_test(A_10_000);
    strdup_test(A_1_000_000_000);

    /* BENCHMARK */
    strdup_benchmark("");
    strdup_benchmark("Hello");
    strdup_benchmark("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    strdup_benchmark("ñandú");
    strdup_benchmark(A_10_000);

    return all_success;
}
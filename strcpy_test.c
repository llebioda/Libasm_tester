#include "libasm_tester.h"

extern char *ft_strcpy(char *dest, const char *src);
extern size_t ft_strlen(const char *s);

static t_bool strcpy_is_valid(const char *src, size_t buf_size,
    const char *buf2, const char *res, const char *ft_res)
{
    t_bool result = TRUE;

    if (buf2 != ft_res)
    {
        printf(RED "[STRCPY] returned pointer value is not the same as the one given as parameters : %p != %p" RESET "\n", buf2, ft_res);
        result = FALSE;
    }

    if ((res == NULL && ft_res != NULL) || (res != NULL && ft_res == NULL)
        || (res != NULL && ft_res != NULL && strcmp(res, ft_res) != 0))
    {
        printf(RED "[STRCPY] diff for [%s] with buf_size = %zu : [%s] != [%s] (len : %zu | %zu)" RESET "\n",
            src, buf_size, res, ft_res, ft_strlen(res), ft_strlen(ft_res));
        result = FALSE;
    }

    return result;
}

static void strcpy_test(const char *src, const size_t buf_size)
{
    char *buf1 = NULL, *buf2 = NULL, *res = NULL, *ft_res = NULL;
    if (src != NULL)
    {
        buf1 = malloc(buf_size);
        buf2 = malloc(buf_size);
        res = strcpy(buf1, src);
        ft_res = ft_strcpy(buf2, src);
    }

    strcpy_is_valid(src, buf_size, buf2, res, ft_res);

    free(buf1);
    free(buf2);
}

static void strcpy_benchmark(const char *src, const size_t buf_size)
{
    clock_t start, end;
    double time_lib = 0.0, time_ft = 0.0, efficiency;

    for (int i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        char *buf1 = malloc(buf_size);
        char *buf2 = malloc(buf_size);

        start = clock();
        char *res = strcpy(buf1, src);
        end = clock();
        time_lib += ((double)(end - start)) / 1000;

        start = clock();
        char *ft_res = ft_strcpy(buf2, src);
        end = clock();
        time_ft += ((double)(end - start)) / 1000;

        if (!strcpy_is_valid(src, buf_size, buf2, res, ft_res))
        {
            printf(RED "Benchmark cancelled" RESET "\n");
            free(buf1);
            free(buf2);
            return;
        }

        free(buf1);
        free(buf2);
    }

    efficiency = (time_lib / time_ft) * 100;

    const char *eff_color = (efficiency > 97.5) ? GREEN : ((efficiency > 60) ? YELLOW : ORANGE);

    printf("%s[BENCHMARK] strcpy:%9.3f ms | ft_strcpy:%9.3f ms | Efficiency: %.2f%%" RESET "\n", 
           eff_color, time_lib, time_ft, efficiency);
}

void strcpy_tester(void)
{
    printf("\n" PURPLE " ***** STRCPY *****" RESET "\n\n");

    strcpy_test(NULL, 1);
    strcpy_test("", 1);
    strcpy_test("", 0);
    strcpy_test("a", 2);
    strcpy_test("aa", 3);
    strcpy_test("ab", 3);
    strcpy_test("abc", 4);
    strcpy_test("abcdef", 7);

    strcpy_test("hello", 6);
    strcpy_test("Hello, world!", 20);
    strcpy_test("Hello", 6);
    strcpy_test("Hello, \n\n\n", 11);
    strcpy_test("Hello, \n\n\n\0salut", 17);
    strcpy_test("abc  \t..?.?>%<", 15);
    strcpy_test("abfewfewfwcweewfwefwefwexwedefec", 33);

    strcpy_test("The quick brown fox jumps over the lazy dog", 44);
    strcpy_test("Lorem ipsum dolor sit amet", 27);
    strcpy_test("Lorem ipsum", 12);

    strcpy_test("Test\0Hidden", 10);
    strcpy_test("Another test", 13);
    strcpy_test("Edge case: buffer equals string length", 39);
    strcpy_test("Buffer larger than needed", 100);
    strcpy_test("Line1\nLine2", 12);
    strcpy_test("Tab\tSeparated", 14);
    strcpy_test("Spaces are important", 21);
    strcpy_test("Trailing spaces   ", 19);
    strcpy_test(" ", 2);
    strcpy_test("  ", 3);
    strcpy_test("                                                                            ", 77);
    strcpy_test("Special chars: !@#$%^&*()", 26);
    strcpy_test("Non-ASCII: üñîçødé", 50);

    {
        char s[] = "\0";
        strcpy_test(s, 2);
    }

    {
        char s[] = "\0Hello";
        strcpy_test(s, 10);
    }

    {
        char s[] = "Hello\0";
        strcpy_test(s, 7);
    }

    strcpy_test("0123456789", 11);
    strcpy_test("12345678901234567890", 21);
    strcpy_test("Short", 6);
    strcpy_test("End", 4);
    strcpy_test("End with null", 14);
    strcpy_test("Another example", 16);
    strcpy_test("Trailing newline\n", 18);
    strcpy_test("Multiple\nnewlines\nin\nstring", 28);
    strcpy_test("This is a longer test string to push the boundaries of the function", 68);
    strcpy_test("A much longer than usual string which should be handled correctly even if the buffer is huge", 93);
    strcpy_test("With escape sequences: \n\t\r", 30);
    strcpy_test("Test#with#hashes", 17);
    strcpy_test("Numbers1234567890", 18);
    strcpy_test("MixedCASEand123", 16);
    strcpy_test("Symbols: []{}<>", 16);

    strcpy_test(A_1_000, 1001);
    strcpy_test(B_1_000, 1001);
    strcpy_test(A_10_000, 10001);

    strcpy_benchmark("", 1);
    strcpy_benchmark("Hello", 6);
    strcpy_benchmark("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", 53);
    strcpy_benchmark("ñandú", 8);
    strcpy_benchmark(A_10_000, 10001);
}
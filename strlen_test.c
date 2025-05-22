#include "libasm_tester.h"

extern size_t ft_strlen(const char *s);

static t_bool strlen_is_valid(const char *s, const int res, const int ft_res)
{
    if (res != ft_res)
    {
        printf(RED "[STRLEN] Diff for [%s] : %d != %d" RESET "\n", s, res, ft_res);
        return FALSE;
    }

    return TRUE;
}

static void strlen_test(const char *s)
{
    int res = s == NULL ? 0 : strlen(s);
    int ft_res = ft_strlen(s);

    strlen_is_valid(s, res, ft_res);
}

static void strlen_benchmark(const char *s)
{
    clock_t start, end;
    double time_lib = 0.0, time_ft = 0.0, efficiency;

    for (int i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        start = clock();
        int res = strlen(s);
        end = clock();
        time_lib += ((double)(end - start)) / 1000;

        start = clock();
        int ft_res = ft_strlen(s);
        end = clock();
        time_ft += ((double)(end - start)) / 1000;

        if (!strlen_is_valid(s, res, ft_res))
        {
            printf(YELLOW "Benchmark cancelled" RESET "\n");
            return;
        }
    }

    efficiency = (time_lib / time_ft) * 100;

    const char *eff_color = (efficiency > 97.5) ? GREEN : ((efficiency > 60) ? YELLOW : ORANGE);

    printf("%s[BENCHMARK] strlen:%9.3f ms | ft_strlen:%9.3f ms | Efficiency: %.2f%%" RESET "\n", 
           eff_color, time_lib, time_ft, efficiency);
}

void strlen_tester(void)
{
    printf("\n" PURPLE " ***** STRLEN *****" RESET "\n\n");

    /* TEST */
    strlen_test(NULL);
    strlen_test("");
    strlen_test("a");
    strlen_test(" ");
    strlen_test("hello");
    strlen_test("hello ");
    strlen_test(" hello");
    strlen_test(" hello ");
    strlen_test("Hello, world!");
    strlen_test("Hello");
    strlen_test("Hello, \n\n\n");
    strlen_test("Hello, \n\n\n\0salut");
    strlen_test("abc  \t..?.?>%<");
    strlen_test("abfewfewfwcweewfwefwefwexwedefec");

    strlen_test("Line1\nLine2");
    strlen_test("Tab\tSeparated");
    strlen_test("This  string  has  multiple  spaces");
    strlen_test("Test\0Hidden");
    strlen_test("Special!@#$%^&*()");
    strlen_test("0123456789");
    strlen_test("a1b2c3d4");
    strlen_test("The quick brown fox jumps over the lazy dog");
    strlen_test("Sphinx of black quartz, judge my vow");
    strlen_test("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    strlen_test("MiXeDcAsE");
    strlen_test("Trailing newline\n");
    strlen_test("\nLeading newline");
    strlen_test("\"Quoted string\"");
    strlen_test("Backslash \\ test");
    strlen_test("{Curly Braces}");
    strlen_test("Dash-test-string");
    strlen_test("!@#$%^&*()_+-=[]{}|;':,.<>?");
    strlen_test("Non-ASCII: üñîçødé");
    strlen_test("こんにちは世界");
    strlen_test("Привет мир");
    strlen_test("مرحبا بالعالم");
    strlen_test("Emoji: 😊");
    strlen_test("This string is very long, intended to test the performance of the custom strlen function, "
                "and it goes on and on with multiple words and various punctuation marks to ensure everything works as expected.");
    strlen_test("Escape sequences:\n\t\r\\");
    strlen_test("abc123!@#XYZ");
    strlen_test("repeat");
    strlen_test("Control:\x01\x02\x03end");

    {
        char s[] = "Embedded\0Null\0Test";
        strlen_test(s);
    }
    {
        char s[] = "\0Embedded\0Null\0Test";
        strlen_test(s);
    }
    {
        char s[] = "Embedded\\0Null\0Test";
        strlen_test(s);
    }

    strlen_test("Part1 " "Part2 " "Part3");
    strlen_test("9876543210");
    strlen_test("This is another long string to further test the capability of the custom strlen implementation. "
                "It should correctly compute the length even if there are many characters in the string.");
    strlen_test("Test123, Test456!");
    strlen_test("Word");
    strlen_test("abcdefghijklmnopqrstuvwxyz");
    strlen_test("zyxwvutsrqponmlkjihgfedcba");
    strlen_test("Mixed 123, with spaces!");
    strlen_test(".,;:!?");
    strlen_test("abababababababababab");
    strlen_test("AaBbCcDdEeFfGg");
    strlen_test("Newline\n and    spaces");
    strlen_test("\xF0\x9F\x92\xAF"); // 💯
    strlen_test("The quick brown fox jumps over the lazy dog");
    strlen_test("Pack my box with five dozen liquor jugs");
    strlen_test("~`!@#$%^&*()_+-={}|[]\\:\";'<>?,./");
    strlen_test("1234567890");
    strlen_test("Random sentence to test the length function correctly.");

    strlen_test(A_1_000);
    strlen_test(B_1_000);
    strlen_test(B_1_000"A");
    strlen_test(A_10_000);

    /* BENCHMARK */
    strlen_benchmark("");
    strlen_benchmark("Hello");
    strlen_benchmark("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    strlen_benchmark(A_10_000);
}
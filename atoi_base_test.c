#include "libasm_tester.h"

#ifdef disable_atoi_base
t_bool atoi_base_tester(void) { return TRUE; }
#else

extern int ft_atoi_base(char *str, char *base);

static t_bool all_success = TRUE;

static void atoi_base_test(char *str, char *base, int expected)
{
    int result = ft_atoi_base(str, base);

    if (result != expected)
    {
        printf(RED "[ATOI_BASE] '%s', '%s' > %d != %d" RESET "\n",
            str, base, result, expected);
        all_success = FALSE;
    }
}

t_bool atoi_base_tester(void)
{
    all_success = TRUE;
    printf("\n" PURPLE " ***** ATOI BASE *****" RESET "\n\n");

    atoi_base_test(NULL, "0123456789", 0);
    atoi_base_test("42", NULL, 0);
    atoi_base_test(NULL, NULL, 0);

    atoi_base_test("", "0123456789", 0);
    atoi_base_test("2", "", 0);
    atoi_base_test("2", "*", 0);
    atoi_base_test("2", "2", 0);
    atoi_base_test("2", "22", 0);
    atoi_base_test("42", " 0123456789", 0);
    atoi_base_test("42", "\f0123456789", 0);
    atoi_base_test("42", "\n0123456789", 0);
    atoi_base_test("42", "\r0123456789", 0);
    atoi_base_test("42", "\t0123456789", 0);
    atoi_base_test("42", "\v0123456789", 0);
    atoi_base_test("42", "0123456789 ", 0);
    atoi_base_test("42", "0123456789\f", 0);
    atoi_base_test("42", "0123456789\n", 0);
    atoi_base_test("42", "0123456789\r", 0);
    atoi_base_test("42", "0123456789\t", 0);
    atoi_base_test("42", "0123456789\v", 0);
    atoi_base_test("42", "0123 456789", 0);
    atoi_base_test("42", "0123456789+", 0);
    atoi_base_test("42", "0123456789-", 0);
    atoi_base_test("", "01234567891", 0);
    atoi_base_test("", "01234567896", 0);
    atoi_base_test("", "01134567892", 0);
    atoi_base_test("+", "0123456789", 0);
    atoi_base_test("++", "0123456789", 0);
    atoi_base_test("++5", "0123456789", 5);
    atoi_base_test("-", "0123456789", 0);
    atoi_base_test("--", "0123456789", 0);
    atoi_base_test("--5", "0123456789", 5);

    // Basic decimal
    atoi_base_test("42", "0123456789", 42);
    atoi_base_test("-42", "0123456789", -42);
    atoi_base_test("+42", "0123456789", 42);
    atoi_base_test("42 \t   \n\t\n\v ", "0123456789", 42);
    atoi_base_test("-42  \t \r\v", "0123456789", -42);
    atoi_base_test("+42 \t", "0123456789", 42);
    atoi_base_test("                  42", "0123456789", 42);
    atoi_base_test("            -42", "0123456789", -42);
    atoi_base_test("                +42", "0123456789", 42);
    atoi_base_test("\n\n\t   \n\v\v  \t\t\n  \t\f\f\r \r\n  42", "0123456789", 42);
    atoi_base_test("\n\n\t   \n\v\v  \t\t\n  \t\f\f\r \r\n  -42", "0123456789", -42);
    atoi_base_test("\n\n\t   \n\v\v  \t\t\n  \t\f\f\r \r\n  +42", "0123456789", 42);
    atoi_base_test("\n\n\t   \n\v\v  \t\t\n  \t\f\f\r \r\n  ----------------42", "0123456789", 42);
    atoi_base_test("\n\n\t   \n\v\v  \t\t\n  \t\f\f\r \r\n  +++++++++++++42", "0123456789", 42);

    // Inverted decimal
    atoi_base_test("20", "9876543210", 79);
    atoi_base_test("-42", "9876543210", -57);

    // Binary
    atoi_base_test("   101010", "01", 42);
    atoi_base_test("1001", "01", 9);
    atoi_base_test("   -101010", "01", -42);
    atoi_base_test("   - 101010", "01", 0);
    atoi_base_test("-1001", "01", -9);
    atoi_base_test(" +121212", "21", 42);

    // Multiple sign handling
    atoi_base_test(" \f\f\f\n   --++-+-+-+42", "0123456789", -42);
    atoi_base_test(" \t  +--+++-+-42", "0123456789", 42);
    atoi_base_test("  \n\t  --++--+---42", "0123456789", -42);

    // Hexadecimal
    atoi_base_test("2A", "0123456789ABCDEF", 42);
    atoi_base_test("2a", "0123456789abcdef", 42);
    atoi_base_test("f", "0123456789abcdef", 15);
    atoi_base_test("ffff", "0123456789abcdef", 65535);
    atoi_base_test("ffaf", "0123456789abcdef", 65455);
    atoi_base_test("ffkf", "0123456789abcdef", 255);
    atoi_base_test("A2", "0123456789ABCDEF", 162);
    atoi_base_test("a2", "0123456789abcdef", 162);
    atoi_base_test("a2", "0123456789ABCDEF", 0);
    atoi_base_test("A2", "0123456789abcdef", 0);
    atoi_base_test("   \t\n-1F", "0123456789ABCDEF", -31);

    // Leading Zeros
    atoi_base_test("00042", "0123456789", 42);
    atoi_base_test("000", "0123456789", 0);
    atoi_base_test("-00042", "0123456789", -42);
    atoi_base_test("000101010", "01", 42);
    atoi_base_test("0000FF", "0123456789ABCDEF", 255);
    atoi_base_test("00052", "01234567", 42);

    // Conversion stopping at the first invalid character.
    atoi_base_test("4+2", "0123456789", 4);
    atoi_base_test("0123456a789", "0123456789", 123456);
    atoi_base_test("123abc456", "0123456789", 123);
    atoi_base_test("abc", "0123456789", 0);
    atoi_base_test("", "0123456789", 0);

    // Additional automated tests for all numbers from 0 to 100 in base 10.
    char buffer[20];
    for (int i = 0; i <= 10000; i++)
    {
        sprintf(buffer, "%d", i);
        atoi_base_test(buffer, "0123456789", i);
        sprintf(buffer, "%d", -i);
        atoi_base_test(buffer, "0123456789", -i);
        if (all_success == FALSE)
            break;
    }

    // Overflow
    atoi_base_test("2147483647", "0123456789", 2147483647);
    atoi_base_test("2147483648", "0123456789", -2147483648);
    atoi_base_test("-2147483647", "0123456789", -2147483647);
    atoi_base_test("-2147483648", "0123456789", -2147483648);
    atoi_base_test("-2147483649", "0123456789", 2147483647);
    atoi_base_test("999999999999", "0123456789", -727379969);
    atoi_base_test("-999999999999", "0123456789", 727379969);

    // Conversion in other bases.
    atoi_base_test("52", "01234567", 42);
    atoi_base_test("3734", "01234567", 2012);
    atoi_base_test("-771", "01234567", -505);
    atoi_base_test("-7717267", "01234567", -2072247);
    atoi_base_test("ifp", "poneyvif", 440);
    atoi_base_test("fed", "abcdef", 207);
    atoi_base_test("1@2", "0123456789@", 233);

    atoi_base_test("cba", "abcdefghijklmnopqrstuvwxyz", 1378);
    atoi_base_test("42", "abcd", 0);
    atoi_base_test("ab", "cab", 5);

    atoi_base_test("!@~", "~!@", 15);
    atoi_base_test("^*&", ")(*&^", 113);
    atoi_base_test("1%a", "a1%b", 24);
    atoi_base_test("!,:", ".,:;!", 107);
    atoi_base_test("@1..=}1", "@:d}=).1", 257311);

    if (all_success)
        printf(GREEN "[ATOI BASE] All test passed succesfully" RESET "\n");

    return all_success;
}

#endif
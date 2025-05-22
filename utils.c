#include "libasm_tester.h"

const char dot[] = "...";

const char *get_printable_str(const char *s)
{
    return strlen(s) > 1002 ? dot : s;
}
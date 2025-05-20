#include "libasm_tester.h"

void strlen_tester(void);
void strcpy_tester(void);
void strcmp_tester(void);
void write_tester(void);

static t_bool argv_contains(char **argv, int argc, const char *value)
{
    if (value == NULL)
        return FALSE;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], value) == 0)
            return TRUE;
    }

    return FALSE;
}

int main(int argc, char **argv)
{
    if (argc <= 1 || argv_contains(argv, argc, "strlen"))
        strlen_tester();

    if (argc <= 1 || argv_contains(argv, argc, "strcpy"))
        strcpy_tester();

    if (argc <= 1 || argv_contains(argv, argc, "strcmp"))
        strcmp_tester();

    if (argc <= 1 || argv_contains(argv, argc, "write"))
        write_tester();

    if (argc <= 1)
        printf("\n" BLUE " ***** All Test Ended *****" RESET "\n");
    return 0;
}

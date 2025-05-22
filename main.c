#include "libasm_tester.h"

void strlen_tester(void);
void strcpy_tester(void);
void strcmp_tester(void);
void write_tester(void);
void read_tester(void);
void strdup_tester(void);

char *A_1_000_000_000 = NULL;

void generate_long_string()
{
    A_1_000_000_000 = malloc(BILLION + 1);
    if (!A_1_000_000_000)
    {
        printf("[A_1_000_000_000] Memory allocation failed!\n");
        return;
    }

    for (size_t i = 0; i < BILLION; i++) {
        A_1_000_000_000[i] = 'A';
    }

    A_1_000_000_000[BILLION] = '\0';
}

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
    srand(time(NULL));
    generate_long_string();

    if (argc <= 1 || argv_contains(argv, argc, "strlen"))
        strlen_tester();

    if (argc <= 1 || argv_contains(argv, argc, "strcpy"))
        strcpy_tester();

    if (argc <= 1 || argv_contains(argv, argc, "strcmp"))
        strcmp_tester();

    if (argc <= 1 || argv_contains(argv, argc, "write"))
        write_tester();

    if (argc <= 1 || argv_contains(argv, argc, "read"))
        read_tester();

    if (argc <= 1 || argv_contains(argv, argc, "strdup"))
        strdup_tester();

    if (argc <= 1)
        printf("\n" BLUE " ***** All Test Ended *****" RESET "\n");

    free(A_1_000_000_000);
    A_1_000_000_000 = NULL;
    return 0;
}

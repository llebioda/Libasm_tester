#include "libasm_tester.h"

t_bool strlen_tester(void);
t_bool strcpy_tester(void);
t_bool strcmp_tester(void);
t_bool write_tester(void);
t_bool read_tester(void);
t_bool strdup_tester(void);

t_bool atoi_base_tester(void);
t_bool list_push_front_tester(void);
t_bool list_size_tester(void);
t_bool list_sort_tester(void);
t_bool list_remove_if_tester(void);

char *A_1_000_000_000 = NULL;

void generate_long_string()
{
    A_1_000_000_000 = malloc(BILLION + 1);
    if (!A_1_000_000_000)
    {
        printf(RED "[TESTER | A_1_000_000_000] Memory allocation failed!" RESET "\n");
        return;
    }

    for (size_t i = 0; i < BILLION; i++)
        A_1_000_000_000[i] = 'A';

    A_1_000_000_000[BILLION] = '\0';
}

static t_bool argv_contains(char **argv, int argc, const char *value)
{
    if (!argv || !value)
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

    t_bool run_all_tests = argc <= 1,
        run_strlen = run_all_tests,
        run_strcpy = run_all_tests,
        run_strcmp = run_all_tests,
        run_write = run_all_tests,
        run_read = run_all_tests,
        run_strdup = run_all_tests,
        run_atoi_base = run_all_tests,
        run_list_push_front = run_all_tests,
        run_list_size = run_all_tests,
        run_list_sort = run_all_tests,
        run_list_remove_if = run_all_tests;

    if (!run_all_tests)
    {
        run_strlen = argv_contains(argv, argc, "strlen");
        run_strcpy = argv_contains(argv, argc, "strcpy");
        run_strcmp = argv_contains(argv, argc, "strcmp");
        run_write = argv_contains(argv, argc, "write");
        run_read = argv_contains(argv, argc, "read");
        run_strdup = argv_contains(argv, argc, "strdup");
        run_atoi_base = argv_contains(argv, argc, "atoi_base");
        run_list_push_front = argv_contains(argv, argc, "list_push_front");
        run_list_size = argv_contains(argv, argc, "list_size");
        run_list_sort = argv_contains(argv, argc, "list_sort");
        run_list_remove_if = argv_contains(argv, argc, "list_remove_if");
    }

    if (run_strlen || run_strcpy || run_strcmp || run_strdup)
        generate_long_string();

    t_bool all_success = TRUE;

    if (run_strlen)
        all_success &= strlen_tester();

    if (run_strcpy)
        all_success &= strcpy_tester();

    if (run_strcmp)
        all_success &= strcmp_tester();

    if (run_write)
        all_success &= write_tester();

    if (run_read)
        all_success &= read_tester();

    if (run_strdup)
        all_success &= strdup_tester();

    if (run_atoi_base)
        all_success &= atoi_base_tester();

    if (run_list_push_front)
        all_success &= list_push_front_tester();

    if (run_list_size)
        all_success &= list_size_tester();

    if (run_list_sort)
        all_success &= list_sort_tester();

    if (run_list_remove_if)
        all_success &= list_remove_if_tester();

    printf("\n" BLUE " ***** All Test Ended *****\n Summary : " RESET);

    if (all_success)
        printf(GREEN "All tests passed successfully!" RESET "\n");
    else
        printf(RED "Some tests failed!" RESET "\n");

    free(A_1_000_000_000);
    A_1_000_000_000 = NULL;
    return 0;
}

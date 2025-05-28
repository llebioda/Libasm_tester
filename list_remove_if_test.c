#include "libasm_tester.h"

#ifdef disable_list_remove_if
t_bool list_remove_if_tester(void) { return TRUE; }
#else

extern void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

static t_bool all_success = TRUE;

static void TESTER_ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
{
    t_list *current;
    t_list *prev = NULL;
    t_list *next;

    if (begin_list == NULL || *begin_list == NULL || cmp == NULL)
        return;

    current = *begin_list;
    while (current != NULL)
    {
        next = current->next;
        if (cmp(current->data, data_ref) == 0)
        {
            if (prev == NULL)
                *begin_list = next;
            else
                prev->next = next;

            if (free_fct != NULL)
                free_fct(current->data);
            free(current);
        }
        else
            prev = current;

        current = next;
    }
}

static void list_remove_if_test(const void **input, int count, void *data_ref, int (*cmp)(), void (*free_fct)(void *), t_data_format data_format)
{
    size_t values_size = free_fct == NULL ? 0 : sizeof_data_format(data_format);

    t_list *list = create_list(input, count, values_size);
    ft_list_remove_if(&list, data_ref, cmp, free_fct);

    t_list *expected_list = create_list(input, count, values_size);
    TESTER_ft_list_remove_if(&expected_list, data_ref, cmp, free_fct);

    t_bool is_equal = list_equals(list, expected_list, cmp);
    int size = TESTER_ft_list_size(list);
    int expected_size = TESTER_ft_list_size(expected_list);

    if (size != expected_size)
        printf(RED "[LIST REMOVE IF] Size mismatch" RESET "\n");
    else if (!is_equal)
        printf(RED "[LIST REMOVE IF] List elements mismatch" RESET "\n");
    else
        printf(GREEN "[LIST REMOVE IF] OK" RESET "\n");

    printf("%sSize: %d (expected: %d)" RESET "\n", size == expected_size ? GREEN : RED, size, expected_size);

    if (!is_equal)
    {
        printf(RED "Data ref value: ");
        print_data_format(data_ref, data_format);
        printf("\nExpected: ");
        print_list(expected_list, data_format);
        printf("\nResult  : ");
        print_list(list, data_format);
        printf(RESET "\n\n");
        all_success = FALSE;
    }
    else
    {
        printf(GREEN "The list has been handled correctly" RESET "\n\n");
    }

    free_list(&list, free_fct);
    free_list(&expected_list, free_fct);
}

int return_0(const void *a, const void *b)
{
    (void)a;
    (void)b;
    return 0;
}

t_bool list_remove_if_tester(void)
{
    all_success = TRUE;
    printf("\n" PURPLE " ***** LIST REMOVE IF *****" RESET "\n\n");

    // Test 0: Empty list
    const char *test0_input[] = {};
    list_remove_if_test((const void **)test0_input, 0, NULL, cmp_str, NULL, STRING);

    // Test 1: Single element list
    const char *test1_input[] = {"solo"};
    list_remove_if_test((const void **)test1_input, 1, NULL, NULL, NULL, STRING);
    list_remove_if_test((const void **)test1_input, 1, "", NULL, NULL, STRING);
    list_remove_if_test((const void **)test1_input, 1, NULL, cmp_str, NULL, STRING);
    list_remove_if_test((const void **)test1_input, 1, "", cmp_str, NULL, STRING);
    list_remove_if_test((const void **)test1_input, 1, "solo", cmp_str, NULL, STRING);

    // Test 2: Already sorted list
    const char *test2_input[] = {"apple", "banana", "cherry"};
    list_remove_if_test((const void **)test2_input, 3, "cherry", cmp_str, NULL, STRING);

    // Test 3: Reverse order
    const char *test3_input[] = {"cherry", "banana", "apple"};
    list_remove_if_test((const void **)test3_input, 3, "banana", cmp_str, NULL, STRING);

    // Test 4: Randomized order
    const char *test4_input[] = {"delta", "alpha", "epsilon", "gamma", "zeta", "beta"};
    list_remove_if_test((const void **)test4_input, 6, "gamma", cmp_str, NULL, STRING);

    // Test 5: With NULL values
    const char *test5_input[] = {"delta", NULL, "alpha", "epsilon", NULL, "gamma", "zeta", "beta", NULL};
    list_remove_if_test((const void **)test5_input, 9, "zeta", cmp_str, NULL, STRING);
    list_remove_if_test((const void **)test5_input, 9, NULL, cmp_str, NULL, STRING);

    // Test 6: Duplicate values
    const char *test6_input[] = {"banana", "apple", "banana", "apple", "cherry"};
    list_remove_if_test((const void **)test6_input, 5, "apple", cmp_str, NULL, STRING);

    // Test 7: High number of random values
    const char *test7_possible_input[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    const int test7_possible_input_count = 26;

    const int test7_nodes_count = 200;
    const char **test7_input = malloc(test7_nodes_count * sizeof(char *));
    for (int i = 0; i < test7_nodes_count; i++)
        test7_input[i] = test7_possible_input[rand() % test7_possible_input_count];

    char *test7_data_ref = (char *)test7_input[rand() % test7_nodes_count];
    list_remove_if_test((const void **)test7_input, test7_nodes_count, test7_data_ref, cmp_str, NULL, STRING);
    test7_data_ref = (char *)test7_input[rand() % test7_nodes_count];
    list_remove_if_test((const void **)test7_input, test7_nodes_count, test7_data_ref, cmp_str, NULL, STRING);
    test7_data_ref = (char *)test7_input[rand() % test7_nodes_count];
    list_remove_if_test((const void **)test7_input, test7_nodes_count, test7_data_ref, cmp_str, NULL, STRING);

    list_remove_if_test((const void **)test7_input, test7_nodes_count, "the list should be empty", return_0, NULL, STRING);

    free(test7_input);

    // Test 8: Int values
    int values8[] = {42, 15, 42, -22, 99};
    int *test8_input[5];
    for (int i = 0; i < 5; i++)
        test8_input[i] = &(values8[i]);

    int test8_data_ref = 42;
    list_remove_if_test((const void **)test8_input, 5, &test8_data_ref, cmp_int, NULL, INT);

    // Test 9: High number of random int values
    const int test9_min_value = -50;
    const int test9_max_value = 50;

    const int test9_nodes_count = 200;
    int **test9_input = malloc(test9_nodes_count * sizeof(int *));
    for (int i = 0; i < test9_nodes_count; i++)
    {
        test9_input[i] = malloc(sizeof(int));
        *test9_input[i] = rand() % (test9_max_value - test9_min_value) + test9_min_value;
    }

    int *test9_data_ref = malloc(sizeof(int));
    *test9_data_ref = *(test9_input[rand() % test9_nodes_count]);

    list_remove_if_test((const void **)test9_input, test9_nodes_count, test9_data_ref, cmp_int, free, INT);

    for (int i = 0; i < test9_nodes_count; i++)
        free(test9_input[i]);
    free(test9_input);
    free(test9_data_ref);

    // Test 10: Float values
    float values10[] = {42.05, 15.1112, 42.0, -22.0, 99, 0.1+0.2, 42.0};
    float *test10_input[7];
    for (int i = 0; i < 7; i++)
        test10_input[i] = &(values10[i]);

    float test10_data_ref = 42.0;
    list_remove_if_test((const void **)test10_input, 6, &test10_data_ref, cmp_float, NULL, FLOAT);

    // Test 11: High number of random Float values
    const float test11_min_value = -250.0;
    const float test11_max_value = 250.0;

    const int test11_nodes_count = 200;
    float **test11_input = malloc(test11_nodes_count * sizeof(float *));
    for (int i = 0; i < test11_nodes_count; i++)
    {
        test11_input[i] = malloc(sizeof(float));
        *test11_input[i] = ((float)rand() / (float)RAND_MAX) * (test11_max_value - test11_min_value) + test11_min_value;
    }

    float *test11_data_ref = malloc(sizeof(float));
    *test11_data_ref = *(test11_input[rand() % test11_nodes_count]);

    list_remove_if_test((const void **)test11_input, test11_nodes_count, test11_data_ref, cmp_float, free, FLOAT);

    for (int i = 0; i < test11_nodes_count; i++)
        free(test11_input[i]);
    free(test11_input);
    free(test11_data_ref);

    return all_success;
}

#endif
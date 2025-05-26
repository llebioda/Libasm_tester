#ifdef disable_list_sort
void list_sort_tester(void) {}
#else

#include "libasm_tester.h"

extern void ft_list_sort(t_list **begin_list, int (*cmp)());

static void TESTER_ft_list_sort(t_list **begin_list, int (*cmp)())
{
    t_list *sorted;
    t_list *current;
    t_list *tmp;
    if (begin_list == NULL || *begin_list == NULL || cmp == NULL)
        return;
    sorted = NULL;
    while (*begin_list != NULL)
    {
        current = *begin_list;
        *begin_list = current->next;
        if (sorted == NULL || cmp(current->data, sorted->data) <= 0)
        {
            current->next = sorted;
            sorted = current;
            continue;
        }
        tmp = sorted;
        while (tmp->next != NULL && cmp(current->data, tmp->next->data) > 0)
            tmp = tmp->next;
        current->next = tmp->next;
        tmp->next = current;
    }
    *begin_list = sorted;
}

static t_list *create_list(const void **values, int count)
{
    t_list *head = NULL;
    t_list *tail = NULL;

    for (int i = 0; i < count; i++)
    {
        t_list *new_node = malloc(sizeof(t_list));
        if (new_node == NULL)
            return NULL;
        new_node->data = (void *)values[i];
        new_node->next = NULL;

        if (tail != NULL)
            tail->next = new_node;
        else
            head = new_node;

        tail = new_node;
    }
    return head;
}

static t_bool verify_list_sorted(t_list *list, t_list *sorted_list, int (*cmp)())
{
    while (list != NULL && sorted_list != NULL)
    {
        if (cmp(list->data, sorted_list->data) != 0)
            return FALSE; // If elements differ, sorting failed.

        list = list->next;
        sorted_list = sorted_list->next;
    }

    // Both lists should reach NULL at the same time.
    return (list == NULL && sorted_list == NULL);
}

static int cmp_str(const char *a, const char *b)
{
    if (a == b)
        return (0);
    if (a == NULL)
        return (-1);
    if (b == NULL)
        return (1);
    return strcmp(a, b);
}

static int cmp_int(const int *a, const int *b)
{
    if (a == b)
        return (0);
    if (a == NULL)
        return (-1);
    if (b == NULL)
        return (1);
    return *a - *b;
}

static int cmp_float(const float *a, const float *b)
{
    if (a == b)
        return (0);
    if (a == NULL)
        return (-1);
    if (b == NULL)
        return (1);
    if (*a == *b)
        return 0;
    return *a > *b ? 1 : -1;
}

static void list_sort_test(const void **input, int count, int (*cmp)(), t_data_format data_format)
{
    t_list *list = create_list(input, count);
    ft_list_sort(&list, cmp);

    t_list *sorted_list = create_list(input, count);
    TESTER_ft_list_sort(&sorted_list, cmp);

    t_bool is_sorted = verify_list_sorted(list, sorted_list, cmp);
    int size = TESTER_ft_list_size(list);

    if (size != count)
        printf(RED "[LIST SORT] Size mismatch" RESET "\n");
    else if (!is_sorted)
        printf(RED "[LIST SORT] Unsorted" RESET "\n");
    else
        printf(GREEN "[LIST SORT] OK" RESET "\n");

    printf("%sSize: %d (expected: %d)" RESET "\n", size == count ? GREEN : RED, size, count);

    if (!is_sorted)
    {
        printf(RED "Expected: ");
        print_list(sorted_list, data_format);
        printf("\nResult :  ");
        print_list(list, data_format);
        printf(RESET "\n\n");
    }
    else
    {
        printf(GREEN "The list is correctly sorted" RESET "\n\n");
    }

    free_list(&list);
    free_list(&sorted_list);
}

void list_sort_tester(void)
{
    printf("\n" PURPLE " ***** LIST SORT *****" RESET "\n\n");

    // Test 0: Empty list
    const char *test0_input[] = {};
    list_sort_test((const void **)test0_input, 0, cmp_str, STRING);

    // Test 1: Single element list
    const char *test1_input[] = {"solo"};
    list_sort_test((const void **)test1_input, 1, cmp_str, STRING);

    // Test 2: Already sorted list
    const char *test2_input[] = {"apple", "banana", "cherry"};
    list_sort_test((const void **)test2_input, 3, cmp_str, STRING);

    // Test 3: Reverse order
    const char *test3_input[] = {"cherry", "banana", "apple"};
    list_sort_test((const void **)test3_input, 3, cmp_str, STRING);

    // Test 4: Randomized order
    const char *test4_input[] = {"delta", "alpha", "epsilon", "gamma", "zeta", "beta"};
    list_sort_test((const void **)test4_input, 6, cmp_str, STRING);

    // Test 5: With NULL values
    const char *test5_input[] = {"delta", NULL, "alpha", "epsilon", NULL, "gamma", "zeta", "beta", NULL};
    list_sort_test((const void **)test5_input, 9, cmp_str, STRING);

    // Test 6: Duplicate values
    const char *test6_input[] = {"banana", "apple", "banana", "apple", "cherry"};
    list_sort_test((const void **)test6_input, 5, cmp_str, STRING);

    // Test 7: High number of random values
    const char *test7_possible_input[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    const int test7_possible_input_count = 26;

    const int test7_nodes_count = 200;
    const char **test7_input = malloc(test7_nodes_count * sizeof(char *));
    for (int i = 0; i < test7_nodes_count; i++)
        test7_input[i] = test7_possible_input[rand() % test7_possible_input_count];

    list_sort_test((const void **)test7_input, test7_nodes_count, cmp_str, STRING);
    free(test7_input);

    // Test 8: Int values
    int values8[] = {42, 15, 42, -22, 99};
    int *test8_input[5];
    for (int i = 0; i < 5; i++)
        test8_input[i] = &(values8[i]);

    list_sort_test((const void **)test8_input, 5, cmp_int, INT);

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

    list_sort_test((const void **)test9_input, test9_nodes_count, cmp_int, INT);

    for (int i = 0; i < test9_nodes_count; i++)
        free(test9_input[i]);
    free(test9_input);

    // Test 10: Float values
    float values10[] = {42.05, 15.1112, 42.0, -22.0, 99, 0.1+0.2};
    float *test10_input[6];
    for (int i = 0; i < 6; i++)
        test10_input[i] = &(values10[i]);

    list_sort_test((const void **)test10_input, 6, cmp_float, FLOAT);

    // Test 11: High number of random Float values
    const float test11_min_value = -250.0;
    const float test11_max_value = 250.0;

    const float test11_nodes_count = 200;
    float **test11_input = malloc(test11_nodes_count * sizeof(float *));
    for (int i = 0; i < test11_nodes_count; i++)
    {
        test11_input[i] = malloc(sizeof(float));
        *test11_input[i] = ((float)(rand()) / (float)RAND_MAX) * (test11_max_value - test11_min_value) + test11_min_value;
    }

    list_sort_test((const void **)test11_input, test11_nodes_count, cmp_float, FLOAT);

    for (int i = 0; i < test11_nodes_count; i++)
        free(test11_input[i]);
    free(test11_input);
}

#endif
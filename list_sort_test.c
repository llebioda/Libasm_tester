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

static t_list *create_list(const void *values[], int count)
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

static int cmp_str(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

static void list_sort_test(const void **input, int count, int (*cmp)())
{
    t_list *list = create_list(input, count);
    print_list(list);
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
        print_list(sorted_list);
        printf("\nResult :  ");
        print_list(list);
        printf(RESET "\n\n");
    }
    else
    {
        printf(GREEN "The list is correctly sorted" RESET "\n\n");
    }

    free_list(&list);
}

void list_sort_tester(void)
{
    printf("\n" PURPLE " ***** LIST SORT *****" RESET "\n\n");

    // Test 0: Empty list
    const char *test0_input[] = {};
    list_sort_test((const void **)test0_input, 0, cmp_str);

    // Test 1: Single element list
    const char *test1_input[] = {"solo"};
    list_sort_test((const void **)test1_input, 1, cmp_str);

    // Test 2: Already sorted list
    const char *test2_input[] = {"apple", "banana", "cherry"};
    list_sort_test((const void **)test2_input, 3, cmp_str);

    // Test 3: Reverse order
    const char *test3_input[] = {"cherry", "banana", "apple"};
    list_sort_test((const void **)test3_input, 3, cmp_str);

    // Test 4: Randomized order
    const char *test4_input[] = {"delta", "alpha", "epsilon", "gamma", "zeta", "beta"};
    list_sort_test((const void **)test4_input, 6, cmp_str);

    // Test 5: Duplicate values
    const char *test5_input[] = {"banana", "apple", "banana", "apple", "cherry"};
    list_sort_test((const void **)test5_input, 5, cmp_str);

    // Test 6: high number of random values
    const char *test6_possible_input[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    const int test6_possible_input_count = 26;

    const int test6_nodes_count = 200;
    const char **test6_input = malloc(test6_nodes_count * sizeof(char *));
    for (int i = 0; i < test6_nodes_count; i++)
        test6_input[i] = test6_possible_input[rand() % test6_possible_input_count];

    list_sort_test((const void **)test6_input, test6_nodes_count, cmp_str);
}

#endif
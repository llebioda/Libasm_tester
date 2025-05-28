#include "libasm_tester.h"

#ifdef disable_list_size
t_bool list_size_tester(void) { return TRUE; }
#else

extern int ft_list_size(t_list *begin_list);

static t_bool all_success = TRUE;

static t_list *create_node()
{
    return calloc(1, sizeof(t_list));
}

static void list_size_test(t_list *list, int expected)
{
    int size = ft_list_size(list);
    t_bool is_valid = size == expected;

    if (!is_valid)
        all_success = FALSE;

    printf("%s[LIST SIZE] ft_list_size: %3d %c= %-3d" RESET "\n",
        is_valid ? GREEN : RED, size, is_valid ? '=' : '!', expected);
}

t_bool list_size_tester(void)
{
    all_success = TRUE;
    printf("\n" PURPLE " ***** LIST SIZE *****" RESET "\n\n");

    // Test 1: Empty list
    list_size_test(NULL, 0);

    // Test 2: List with 1 element
    t_list *list = create_node();
    list_size_test(list, 1);
    free_list(&list, NULL);

    // Test 3: List with 2 elements
    list = create_node();
    list->next = create_node();
    list_size_test(list, 2);
    free_list(&list, NULL);

    // Test 4: List with 10 elements
    list = create_node();
    t_list *temp = list;
    for (int i = 0; i < 9; i++)
    {
        temp->next = create_node();
        temp = temp->next;
    }
    list_size_test(list, 10);
    free_list(&list, NULL);

    // Test 5: List with 30 elements
    list = create_node();
    temp = list;
    for (int i = 0; i < 29; i++)
    {
        temp->next = create_node();
        temp = temp->next;
    }
    list_size_test(list, 30);
    free_list(&list, NULL);

    // Test 6: Additional tests with different structures
    for (int i = 3; i <= 120; i += 2 + (int)(i * 0.1))
    {
        list = create_node();
        temp = list;
        for (int j = 1; j < i; j++)
        {
            temp->next = create_node();
            temp = temp->next;
        }
        list_size_test(list, i);
        free_list(&list, NULL);
    }

    return all_success;
}

#endif
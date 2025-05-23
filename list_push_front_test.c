#ifdef disable_list_push_front
void list_push_front_tester(void) {}
#else

#include "libasm_tester.h"

extern void ft_list_push_front(t_list **begin_list, void *data);

static t_bool verify_list_content(t_list *list, const char *expected[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (list == NULL)
            return FALSE; // List is shorter than expected.

        // Compare the data (assumed to be a C-string) with the expected value.
        if ((const char *)(list->data) != expected[i])
            return FALSE;

        list = list->next;
    }
    // The list must have no more nodes than expected.
    return (list == NULL);
}

static void list_push_front_test(const char *expected[], int count)
{
    t_list *list = NULL;

    // Push in the reverse order, since push_front adds to the beginning,
    for (int i = count - 1; i >= 0; i--)
        ft_list_push_front(&list, (void *)expected[i]);

    int size = TESTER_ft_list_size(list);
    t_bool is_valid = verify_list_content(list, expected, count);

    if (is_valid && size == count)
        printf(GREEN "[LIST PUSH FRONT] OK" RESET "\n");
    else
        printf(RED "[LIST PUSH FRONT] Mismatch detected" RESET "\n");

    printf("%ssize: %d (expected: %d)" RESET "\n", size == count ? GREEN : RED, size, count);

    if (!is_valid)
    {
        printf(RED "Expected: [");
        for (int i = 0; i < count; i++)
        {
            printf("'%s'", expected[i]);
            if (i < count - 1)
                printf(", ");
        }
        printf("]" RESET "\n");

        printf(RED "Result :  ");
        print_list(list);
        printf(RESET "\n\n");
    }
    else
    {
        printf(GREEN "The list is correct" RESET "\n\n");
    }

    free_list(&list);
}

void list_push_front_tester(void)
{
    printf("\n" PURPLE " ***** LIST PUSH FRONT *****" RESET "\n\n");

    // Test 0: Single element list
    const char *test0_values[] = {};
    list_push_front_test(test0_values, 0);

    // Test 1: Single element list
    const char *test1_values[] = {"solo"};
    list_push_front_test(test1_values, 1);

    // Test 2: 3 elements
    const char *test2_values[] = {"start", "middle", "end"};
    list_push_front_test(test2_values, 3);

    // Test 3: 10 elements
    const char *test3_values[] = {"node1", "node2", "node3", "node4", "node5",
                                  "node6", "node7", "node8", "node9", "node10"};
    list_push_front_test(test3_values, 10);

    // Test 4: 15 elements
    const char *test4_values[] = {
        "Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Helen",
        "Ivy", "Jack", "Kathy", "Leo", "Mona", "Nate", "Olivia"};
    list_push_front_test(test4_values, 15);

    // Test 5: multiple duplicate values.
    const char *test5_value = "repeat";
    const char *test5_values[] = {test5_value, test5_value, test5_value, test5_value, test5_value};
    list_push_front_test(test5_values, 5);

    // Test 6: high number of random values
    const char *test6_possible_values[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    const int test6_possible_values_count = 26;

    const int test6_nodes_count = 200;
    const char **expected_values = malloc(test6_nodes_count * sizeof(char *));
    for (int i = 0; i < test6_nodes_count; i++)
        expected_values[i] = test6_possible_values[rand() % test6_possible_values_count];

    list_push_front_test(expected_values, test6_nodes_count);
}

#endif
#include "libasm_tester.h"

const char dot[] = "...";

const char *get_printable_str(const char *s)
{
    return strlen(s) > 1002 ? dot : s;
}

void calculate_efficiency(const char *function_name, double time_lib, double time_ft)
{
    double efficiency = (time_lib / time_ft) * 100;

    const char *eff_color = (efficiency > 97.5) ? GREEN :
                            ((efficiency > 60) ? YELLOW : ORANGE);

    printf("%s[BENCHMARK] %s:%9.3f ms | ft_%s:%9.3f ms | Efficiency: %.2f%%" RESET "\n", 
           eff_color, function_name, time_lib, function_name, time_ft, efficiency);
}

size_t sizeof_data_format(t_data_format data_format)
{
    switch (data_format)
    {
        case STRING:
            return sizeof(char *);

        case INT:
            return sizeof(int);

        case FLOAT:
            return sizeof(float);

        default:
            return 0;
    }
}

void print_data_format(const void *data, t_data_format data_format)
{
    if (data == NULL)
    {
        printf("null");
        return;
    }

    switch (data_format)
    {
        case STRING:
            printf("'%s'", (char *)data);
            break;

        case INT:
            printf("%d", *((int *)data));
            break;

        case FLOAT:
            printf("%.6g", *((float *)data));
            break;
    }
}

t_list *create_list(const void **values, int count, size_t values_size)
{
    t_list *head = NULL;
    t_list *tail = NULL;

    for (int i = 0; i < count; i++)
    {
        t_list *new_node = malloc(sizeof(t_list));
        if (new_node == NULL)
        {
            printf(RED "[LIST_SORT] MALLOC ERROR while creating list" RESET "\n");
            free_list(&head, NULL);
            return NULL;
        }

        if (values_size == 0 || values[i] == NULL)
            new_node->data = (void *)values[i];
        else
        {
            new_node->data = malloc(values_size);
            if (new_node->data == NULL)
            {
                printf(RED "[LIST_SORT] MALLOC ERROR while creating list data" RESET "\n");
                free(new_node);
                free_list(&head, NULL);
                return NULL;
            }
            memcpy(new_node->data, values[i], values_size);
        }
        new_node->next = NULL;

        if (tail != NULL)
            tail->next = new_node;
        else
            head = new_node;

        tail = new_node;
    }
    return head;
}

void free_list(t_list **list, void (*free_fct)(void *))
{
    t_list *current;
    t_list *next;

    if (list == NULL || *list == NULL) return;

    current = *list;
    while (current)
    {
        next = current->next;
        if (free_fct != NULL)
            free_fct(current->data);
        free(current);
        current = next;
    }
    *list = NULL;
}

void print_list(t_list *list, t_data_format data_format)
{
    printf("[");
    while (list != NULL)
    {
        print_data_format(list->data, data_format);
        list = list->next;
        if (list != NULL)
            printf(", ");
    }
    printf("]");
}

t_bool list_equals(t_list *list, t_list *expected_list, int (*cmp)())
{
    while (list != NULL && expected_list != NULL)
    {
        if (cmp && cmp(list->data, expected_list->data) != 0)
            return FALSE; // If elements differ, list are not the same.

        list = list->next;
        expected_list = expected_list->next;
    }

    // Both lists should reach NULL at the same time.
    return (list == NULL && expected_list == NULL);
}

int TESTER_ft_list_size(t_list *begin_list)
{
    int i = 0;
    while (begin_list != NULL)
    {
        begin_list = begin_list->next;
        i++;
    }
    return (i);
}

int cmp_str(const char *a, const char *b)
{
    printf("%s", ""); // could make crash
    if (a == b)
        return (0);
    if (a == NULL)
        return (-1);
    if (b == NULL)
        return (1);
    return strcmp(a, b);
}

int cmp_int(const int *a, const int *b)
{
    printf("%s", ""); // could make crash
    if (a == b)
        return (0);
    if (a == NULL)
        return (-1);
    if (b == NULL)
        return (1);
    return *a - *b;
}

int cmp_float(const float *a, const float *b)
{
    printf("%s", ""); // could make crash
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
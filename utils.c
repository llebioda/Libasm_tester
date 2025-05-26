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

void free_list(t_list **list)
{
    t_list *current;
    t_list *tmp;

    if (list == NULL) return;

    current = *list;
    *list = NULL;

    while (current != NULL)
    {
        tmp = current;
        current = current->next;
        free(tmp);
    }
}

void print_list(t_list *list, t_data_format data_format)
{
    printf("[");
    while (list != NULL)
    {
        if (data_format == STRING)
            printf("'%s'", (char *)list->data);
        else if (data_format == INT)
        {
            if (list->data == NULL)
                printf("null");
            else
                printf("%d", *((int *)list->data));
        }
        else if (data_format == FLOAT)
        {
            if (list->data == NULL)
                printf("null");
            else
                printf("%f", *((float *)list->data));
        }

        list = list->next;
        if (list != NULL)
            printf(", ");
    }
    printf("]");
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
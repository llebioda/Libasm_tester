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
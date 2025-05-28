#include "libasm_tester.h"

#ifdef disable_list_remove_if
t_bool list_remove_if_tester(void) { return TRUE; }
#else

extern void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

static t_bool all_success = TRUE;

t_bool list_remove_if_tester(void)
{
    all_success = TRUE;
    printf("\n" PURPLE " ***** LIST REMOVE IF *****" RESET "\n\n");

    return all_success;
}

#endif
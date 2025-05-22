#ifdef disable_list_remove_if
void list_remove_if_tester(void) {}
#else

#include "libasm_tester.h"

extern void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

void list_remove_if_tester(void)
{
    printf("\n" PURPLE " ***** LIST REMOVE IF *****" RESET "\n\n");
}

#endif
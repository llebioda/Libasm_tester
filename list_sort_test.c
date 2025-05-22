#ifdef disable_list_sort
void list_sort_tester(void) {}
#else

#include "libasm_tester.h"

extern void ft_list_sort(t_list **begin_list, int (*cmp)());

void list_sort_tester(void)
{
    printf("\n" PURPLE " ***** LIST SORT *****" RESET "\n\n");
}

#endif
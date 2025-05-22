#ifdef disable_list_push_front
void list_push_front_tester(void) {}
#else

#include "libasm_tester.h"

extern void ft_list_push_front(t_list **begin_list, void *data);

void list_push_front_tester(void)
{
}

#endif
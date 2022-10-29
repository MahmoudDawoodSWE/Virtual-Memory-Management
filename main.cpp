#include "stdio.h"
#include "sim_mem.h"
/*
 * 322943408
 * Mahmoud Dawood
 */
int main()
{
    char val;
    sim_mem mem_sm((char*)"exec_file", (char*)"exec_file1", (char*)"swap_file" ,25, 50, 25,25, 25, 5,2);
    /*fill the ram with page
     * d-> data ,b -> bss,h->heap-stack
     */
    /*i could do it in loop */
    mem_sm.store(1, 25,'d');
    mem_sm.store(1, 30,'d');
    mem_sm.store(1, 35,'d');
    mem_sm.store(1, 40,'d');
    mem_sm.store(1, 45,'d');
    mem_sm.store(1, 50,'d');
    mem_sm.store(1, 55,'d');
    mem_sm.store(1, 60,'d');
    mem_sm.store(1, 65,'d');
    mem_sm.store(1, 70,'d');
    mem_sm.store(1, 75,'b');
    mem_sm.store(1, 80,'b');
    mem_sm.store(1, 85,'b');
    mem_sm.store(1, 90,'b');
    mem_sm.store(1, 95,'b');
    mem_sm.store(1, 100,'h');
    mem_sm.store(1, 105,'h');
    mem_sm.store(1, 110,'h');
    mem_sm.store(1, 115,'h');
    mem_sm.store(1, 120,'h');
    mem_sm.store(2, 25,'d');
    mem_sm.store(2, 30,'d');
    mem_sm.store(2, 35,'d');
    mem_sm.store(2, 40,'d');
    mem_sm.store(2, 45,'d');
    mem_sm.store(2, 50,'d');
    mem_sm.store(2, 55,'d');
    mem_sm.store(2, 60,'d');
    mem_sm.store(2, 65,'d');
    mem_sm.store(2, 70,'d');
    mem_sm.store(2, 75,'b');
    mem_sm.store(2, 80,'b');
    mem_sm.store(2, 85,'b');
    mem_sm.store(2, 90,'b');
    mem_sm.store(2, 95,'b');
    mem_sm.store(2, 100,'h');
    mem_sm.store(2, 105,'h');
    mem_sm.store(2, 110,'h');
    mem_sm.store(2, 115,'h');
    mem_sm.store(2, 120,'h');
    // now the ram is full bring more badge it will be placed in the oldest place this means frame 0 and 1
    val = mem_sm.load (1, 0);
    printf(" load from pro 1 address 0 = %c\n",val);
    val = mem_sm.load (2, 0);
    printf("load from pro 1 address 0 = %c\n",val);

    // now pages in the swap bring them again it will be placed in the oldest place this means frame 2 and 3
    mem_sm.store(1, 25,'d');
    mem_sm.store(1, 30,'d');

    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();
}



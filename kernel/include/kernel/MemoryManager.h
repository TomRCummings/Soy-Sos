#ifndef _KERNEL_MEMORY_MANAGER_H
#define _KERNEL_MEMORY_MANAGER_H

void initialize_memory_manager(void* endOfKernelAddress);
void parse_memory_map(void* mmmtAddress);
void *allocate_page();
void free_page(void* pageAddress);

#endif
#include <kernel/MemoryManager.h>
#include <kernel/Multiboot.h>

#include <stddef.h>
#include <stdint.h>

void* nextFreePage = NULL;

void initialize_memory_manager(void* endOfKernelAddress) {
    nextFreePage = endOfKernelAddress;
}

//TODO: This implementation tacks the page list onto the end of the kernel. This won't work for higher half and isn't very smart.
void parse_memory_map(void* mmmtAddress) {
    multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mmmtAddress);

    uint32_t addrCounter;
    if (mmmt->addr_low <= nextFreePage) {
        addrCounter = nextFreePage;
    } else if (mmmt->addr_low % 1024 == 0) {
        addrCounter = mmmt->addr_low;  
    } else {
        addrCounter = mmmt->addr_low + (1024 - (mmmt->addr_low % 1024));
    }

    uint32_t addrLimit = mmmt->addr_low + mmmt->len_low;
    if (addrLimit % 1024 != 1023) {
        addrLimit = addrLimit - (addrLimit % 1024);
    }

    while (addrCounter < addrLimit) {
        free_page((void*) (addrCounter));
        addrCounter += 1024;
    }

}

void *allocate_page() {
    void* pageToAllocate = nextFreePage;
    nextFreePage = *(uint32_t*)nextFreePage;
    return (void*)pageToAllocate;
}

void free_page(void* pageAddress) {
    *(uint32_t*)pageAddress = (uint32_t)nextFreePage;
    nextFreePage = pageAddress;
}
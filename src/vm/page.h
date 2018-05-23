#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <hash.h>
#include "vm/frame.h"
#include "vm/swap.h"
#include "filesys/off_t.h"


#define MAX_STACKSIZE (1<<23)

enum page_cons{
    FILE,
    SWAP,
    MMAP,
    HASHERR
};

struct supplement_pagetable_entry{
    uint8_t type;
    void *upage;
    bool loaded;
    bool pinned;
    struct hash_elem elem;

    struct file *file;
    off_t offset;
    uint32_t readbytes;
    uint32_t zerobytes;
    bool writable;

    swapindex_t swap_index;
};


#endif /* vm/page.h */
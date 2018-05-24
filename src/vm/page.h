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
    HASH_ERR
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
void page_table_init (struct hash *spt);
void page_table_destroy (struct hash *spt);

bool load_page (struct supplement_pagetable_entry *spte);
bool load_mmap (struct supplement_pagetable_entry *spte);
bool load_swap (struct supplement_pagetable_entry *spte);
bool load_file (struct supplement_pagetable_entry *spte);
bool add_file_to_page_table (struct file *file, int32_t ofs, uint8_t *upage,
			     uint32_t read_bytes, uint32_t zero_bytes,
			     bool writable);
bool add_mmap_to_page_table(struct file *file, int32_t ofs, uint8_t *upage,
			    uint32_t read_bytes, uint32_t zero_bytes);
bool grow_stack (void *uva);
struct supplement_pagetable_entry* get_spte (void *uva);

#endif /* vm/page.h */
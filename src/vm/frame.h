#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <stdint.h>
#include "threads/synch.h"
#include "threads/palloc.h"
#include "vm/page.h"

struct frametable_entry{
    void *frame;
    struct supplement_pagetable_entry *spe;
    struct thread *thread;
    struct list_elem elem;
};

void frame_init(void);
void *frame_allocate(enum palloc_flags flags, struct supplement_pagetable_entry *spe);
void frame_free (void *kpage);
void *frame_evict (enum palloc_flags flags);
void add_frame_to_table (void *frame_page, struct supplement_pagetable_entry *spe);

#endif /* vm/frame.h */
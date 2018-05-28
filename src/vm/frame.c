#include "vm/frame.h"
#include "vm/page.h"
#include "vm/swap.h"
#include "filesys/file.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/palloc.h"
#include "userprog/pagedir.h"
#include "userprog/syscall.h"

struct list frame_table;
struct lock frame_table_lock;

void 
frame_init(void)
{
    list_init(&frame_table);
    lock_init(&frame_table_lock);
}

void * 
frame_allocate(enum palloc_flags flags, struct supplement_pagetable_entry *spe)
{
    if ((flags & PAL_USER) == 0){
        return NULL;
    }
    void *frame_page = palloc_get_page(flags);
    if (frame_page == NULL){
        while (frame_page == NULL){
            frame_page = frame_evict(flags);
            lock_release(&frame_table_lock);
        }
        ASSERT (frame_page != NULL)
    } 
    add_frame_to_table(frame_page, spe);
    return frame_page;
}

void 
frame_free (void *kpage)
{
    lock_acquire(&frame_table_lock);


    ASSERT (is_kernel_vaddr(kpage))
    struct list_elem *e;
    for (e = list_begin(&frame_table); e != list_end(&frame_table); e = list_next(e)){
        struct frametable_entry *fe = list_entry(e, struct frametable_entry, elem);
        if (fe ->frame == kpage){
            list_remove(e);
            palloc_free_page(kpage);
            free(fe);
            break;
        }
    }

    lock_release(&frame_table_lock);
}

void * 
frame_evict (enum palloc_flags flags)
{
    lock_acquire(&frame_table_lock);
    struct list_elem *e = list_begin(&frame_table);

    while (true){
        struct frametable_entry *fe = list_entry(e, struct frametable_entry, elem);
        if (! (fe ->spe ->pinned)){
            struct thread *t = fe ->thread;
            uint32_t *pd = t ->pagedir;
            void* upage = fe ->spe ->upage;
            if (pagedir_is_accessed(pd, upage)){
                pagedir_set_accessed(pd, upage, false);
            }
            else{
                if (pagedir_is_dirty(pd, upage) || fe ->spe ->type == 1){
                    if (fe ->spe ->type != 2){
                        fe ->spe ->type = 1;
                        fe ->spe ->swap_index = swap_out(fe ->frame);
                    }
                    else{
                        lock_acquire(&filesys_lock);
                        file_write_at(fe ->spe ->file, fe -> frame, fe ->spe -> readbytes, fe ->spe ->offset);
                        lock_release(&filesys_lock);
                    }
                }
                fe ->spe -> loaded = false;
                list_remove (&fe ->elem);
                pagedir_clear_page(pd, upage);
                palloc_free_page(fe ->frame);
                free(fe);
                return palloc_get_page(flags);
            }
        }
        e = list_next(e);
        if (e == list_end(&frame_table)){
            e = list_begin(&frame_table);
        }
    }
}

void 
add_frame_to_table (void *frame_page, struct supplement_pagetable_entry *spe)
{
  struct frametable_entry *fe = malloc(sizeof(struct frametable_entry));
  fe->frame = frame_page;
  fe->spe = spe;
  fe->thread = thread_current();
  lock_acquire(&frame_table_lock);
  list_push_back(&frame_table, &fe->elem);
  lock_release(&frame_table_lock);
}
#include <string.h>
#include <stdbool.h>
#include "threads/interrupt.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "userprog/syscall.h"
#include "vm/frame.h"
#include "vm/page.h"
#include "vm/swap.h"
#include "filesys/file.h"

static unsigned 
page_hash_func (const struct hash_elem *e, void *aux UNUSED)
{
  struct supplement_pagetable_entry *spe = hash_entry(e, struct supplement_pagetable_entry, elem);
  return hash_int((int) spe->upage);
}

static bool 
page_less_func (const struct hash_elem *a, const struct hash_elem *b, void *aux UNUSED)
{
  struct supplement_pagetable_entry *sa = hash_entry(a, struct supplement_pagetable_entry, elem);
  struct supplement_pagetable_entry *sb = hash_entry(b, struct supplement_pagetable_entry, elem);
  if (sa->upage < sb->upage)
    {
      return true;
    }
  return false;
}

static void 
page_action_func (struct hash_elem *e, void *aux UNUSED)
{
  struct supplement_pagetable_entry *spte = hash_entry(e, struct supplement_pagetable_entry,
					   elem);
  if (spte->loaded)
    {
      frame_free(pagedir_get_page(thread_current()->pagedir, spte->upage));
      pagedir_clear_page(thread_current()->pagedir, spte->upage);
    }
  free(spte);
}

void 
page_table_init (struct hash *spt)
{
  hash_init (spt, page_hash_func, page_less_func, NULL);
}

void 
page_table_destroy (struct hash *spt)
{
  hash_destroy (spt, page_action_func);
}

struct supplement_pagetable_entry* 
get_spte (void *upage)
{
  struct supplement_pagetable_entry spte;
  spte.upage = pg_round_down(upage);

  struct hash_elem *e = hash_find(&thread_current()->spt, &spte.elem);
  if (!e)
    {
      return NULL;
    }
  return hash_entry (e, struct supplement_pagetable_entry, elem);
}

bool 
load_page (struct supplement_pagetable_entry *spte)
{
  bool success = false;
  spte->pinned = true;
  if (spte->loaded)
    {
      return success;
    }
  switch (spte->type)
    {
    case 0:
      success = load_file(spte);
      break;
    case 1:
      success = load_swap(spte);
      break;
    case 2:
      success = load_file(spte);
      break;
    }
  return success;
}

bool 
load_swap (struct supplement_pagetable_entry *spte)
{
  uint8_t *frame = frame_allocate (PAL_USER, spte);
  if (!frame)
    {
      return false;
    }
  if (!install_page(spte->upage, frame, spte->writable))
    {
      frame_free(frame);
      return false;
    }
  swap_in(spte->swap_index, spte->upage);
  spte->loaded = true;
  return true;
}

bool 
load_file (struct supplement_pagetable_entry *spte)
{
  enum palloc_flags flags = PAL_USER;
  if (spte->readbytes == 0)
    {
      flags |= PAL_ZERO;
    }
  uint8_t *frame = frame_allocate(flags, spte);
  if (!frame)
    {
      return false;
    }
  if (spte->readbytes > 0)
    {
      lock_acquire(&filesys_lock);
      if ((int) spte->readbytes != file_read_at(spte->file, frame,
						 spte->readbytes,
						 spte->offset))
	{
	  lock_release(&filesys_lock);
	  frame_free(frame);
	  return false;
	}
      lock_release(&filesys_lock);
      memset(frame + spte->readbytes, 0, spte->zerobytes);
    }

  if (!install_page(spte->upage, frame, spte->writable))
    {
      frame_free(frame);
      return false;
    }

  spte->loaded = true;  
  return true;
}

bool 
add_file_to_page_table (struct file *file, int32_t ofs, uint8_t *upage, uint32_t readbytes, uint32_t zerobytes, bool writable)
{
  struct supplement_pagetable_entry *spte = malloc(sizeof(struct supplement_pagetable_entry));
  if (!spte)
    {
      return false;
    }
  spte->file = file;
  spte->offset = ofs;
  spte->upage = upage;
  spte->readbytes = readbytes;
  spte->zerobytes = zerobytes;
  spte->writable = writable;
  spte->loaded = false;
  spte->type = FILE;
  spte->pinned = false;

  return (hash_insert(&thread_current()->spt, &spte->elem) == NULL);
}

bool 
add_mmap_to_page_table(struct file *file, int32_t ofs, uint8_t *upage,
			     uint32_t readbytes, uint32_t zerobytes)
{
  struct supplement_pagetable_entry *spte = malloc(sizeof(struct supplement_pagetable_entry));
  if (!spte)
    {
      return false;
    }
  spte->file = file;
  spte->offset = ofs;
  spte->upage = upage;
  spte->readbytes = readbytes;
  spte->zerobytes = zerobytes;
  spte->loaded = false;
  spte->type = MMAP;
  spte->writable = true;
  spte->pinned = false;

  if (!process_add_mmap(spte))
    {
      free(spte);
      return false;
    }

  if (hash_insert(&thread_current()->spt, &spte->elem))
    {
      spte->type = HASH_ERR;
      return false;
    }
  return true;
}

bool 
grow_stack (void *upage)
{
  if ( (size_t) (PHYS_BASE - pg_round_down(upage)) > MAX_STACKSIZE)
    {
      return false;
   }
 struct supplement_pagetable_entry *spte = malloc(sizeof(struct supplement_pagetable_entry));
  if (!spte)
   {
      return false;
    }
  spte->upage = pg_round_down(upage);
  spte->loaded = true;
  spte->writable = true;
  spte->type = SWAP;
  spte->pinned = true;

  uint8_t *frame = frame_allocate (PAL_USER, spte);
  if (!frame)
    {
      free(spte);
      return false;
    }

  if (!install_page(spte->upage, frame, spte->writable))
    {
      free(spte);
      frame_free(frame);
      return false;
    }

  if (intr_context())
    {
      spte->pinned = false;
    }

  return (hash_insert(&thread_current()->spt, &spte->elem) == NULL);
}
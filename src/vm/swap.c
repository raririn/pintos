#include "vm/swap.h"

void
swap_init(void)
{
    ASSERT (SECTORS_PER_PAGE > 0);
    swap_block = block_get_role(BLOCK_SWAP);
    if (swap_block == NULL){
        return;
        PANIC("Can't get swap blcok.");
    }
    swap_map = bitmap_create(block_size(swap_block)/SECTORS_PER_PAGE);
    if (swap_map == NULL){
        return;
        PANIC("Swap map is still NULL!");
    }
    bitmap_set_all(swap_map, 0);  /* SWAP_FREE */
    lock_init(&swap_lock);
}

size_t 
swap_out(void *frame_page)
{
    if (!swap_block || !swap_map){
      PANIC("Need swap partition but no swap partition present!");
    }
    lock_acquire(&swap_lock);
    size_t swap_index = bitmap_scan_and_flip(swap_map, 0, 1, SWAP_FREE);

    ASSERT(swap_index != BITMAP_ERROR)

    size_t i;
    for (i = 0; i < SECTORS_PER_PAGE; i++){ 
      block_write(swap_block, swap_index * SECTORS_PER_PAGE + i, frame_page + BLOCK_SECTOR_SIZE * i);
    }
    lock_release(&swap_lock);
    return swap_index;
}

void
swap_in(size_t swap_index, void* frame_page)
{
    /* ASSERT (frame_page >= PHYS_BASE); */
    if (bitmap_test(swap_map, swap_index) == false){
        PANIC("Ilegal swap.");
    }
    if (!swap_block || !swap_map){
        return;
    }

    lock_acquire(&swap_lock);
    size_t i;
    bitmap_flip(swap_map, swap_index);
    for (i = 0; i < SECTORS_PER_PAGE; i++){
        block_read(swap_block, swap_index * SECTORS_PER_PAGE + i, frame_page + BLOCK_SECTOR_SIZE * i);
    }
    lock_release(&swap_lock);
}
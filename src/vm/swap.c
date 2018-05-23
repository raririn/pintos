#include "vm/swap.h"

void
swap_init(void)
{
    ASSERT (SECTORS_PER_PAGE > 0);
    swap_block = block_get_role(BLOCK_SWAP);
    if (swap_block == NULL){
        PANIC("Can't get swap blcok.");
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
    size_t free_index = bitmap_scan_and_flip(swap_map, 0, 1, SWAP_FREE);

    if (free_index == BITMAP_ERROR){
      PANIC("Swap partition is full!");
    }

    size_t i;
    for (i = 0; i < SECTORS_PER_PAGE; i++){ 
      block_write(swap_block, free_index * SECTORS_PER_PAGE + i, (uint8_t *) frame_page + i * BLOCK_SECTOR_SIZE);
    }
    lock_release(&swap_lock);
    return free_index;
}
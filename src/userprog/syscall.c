#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "devices/shutdown.h"
#include "devices/input.h"
#include "userprog/process.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"
#include "threads/synch.h"
#include "lib/kernel/list.h"

static void syscall_handler (struct intr_frame *f);

static void check_user (const uint8_t *uaddr);
static int32_t get_user (const uint8_t *uaddr);
static bool put_user (uint8_t *udst, uint8_t byte);
static int read_from_usermem (void *src, void *des, size_t bytes);

static struct file_descriptor* find_file_desc(struct thread *, int fd);
void mmap_getarg(struct intr_frame *f, int *arg, int n);

void unpin_ptr (void* vaddr);
void unpin_string (void* str);
void unpin_buffer (void* buffer, unsigned size);

/*struct lock filesys_lock;*/

/* 
    SYS_HALT     = 0                   
    SYS_EXIT     = 1                
    SYS_EXEC     = 2               
    SYS_WAIT     = 3            
    SYS_CREATE   = 4               
    SYS_REMOVE   = 5             
    SYS_OPEN     = 6             
    SYS_FILESIZE = 7              
    SYS_READ     = 8           
    SYS_WRITE    = 9              
    SYS_SEEK     = 10           
    SYS_TELL     = 11          
    SYS_CLOSE    = 12  

    -- VM -- 
    SYS_MMAP     = 13
    SYS_MUNMAP   = 14        
*/

void
syscall_init (void)
{
  lock_init (&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* Handling invalid access by user program.*/
static void 
fail_invalid_access(void) {
  if (lock_held_by_current_thread(&filesys_lock)){
    lock_release (&filesys_lock);
  }
  sys_exit(-1);
}

static void
syscall_handler (struct intr_frame *f)
{
  int syscall_ID; /* See <syscall-nr.h> for ID reference. */
  int arg[MAX_ARGS];
  read_from_usermem(f->esp, &syscall_ID, sizeof(syscall_ID));
  check_valid_ptr((const void*)f->esp, f->esp);

  switch (syscall_ID) {
  case SYS_HALT:
    {
      sys_halt();
      break;
    }

  case SYS_EXIT:
    {
      int exitcode;
      read_from_usermem(f->esp + 4, &exitcode, sizeof(exitcode));

      sys_exit(exitcode);
      break;
    }

  case SYS_EXEC:
    {
      void* cmdline;
      read_from_usermem(f->esp + 4, &cmdline, sizeof(cmdline));
      check_valid_str((const void *)cmdline, f->esp);

      int return_value = sys_exec((const char*) cmdline);
      f->eax = (uint32_t) return_value;
      unpin_string((void *) cmdline);
      break;
    }

  case SYS_WAIT:
    {
      pid_t pid;
      read_from_usermem(f->esp + 4, &pid, sizeof(pid_t));

      int ret = sys_wait(pid);
      f->eax = (uint32_t) ret;
      break;
    }

  case SYS_CREATE:
    {
      const char* filename;
      unsigned initial_size;
      bool return_value;

      read_from_usermem(f->esp + 4, &filename, sizeof(filename));
      read_from_usermem(f->esp + 8, &initial_size, sizeof(initial_size));
      check_valid_str((const void*)filename, f->esp);

      return_value = sys_create(filename, initial_size);
      f->eax = return_value;
      unpin_string((void *)filename);
      break;
    }

  case SYS_REMOVE:
    {
      const char* filename;
      bool return_value;

      read_from_usermem(f->esp + 4, &filename, sizeof(filename));
      check_valid_str((const void*) filename, f->esp);

      return_value = sys_remove(filename);
      f->eax = return_value;
      break;
    }

  case SYS_OPEN:
    {
      const char* filename;
      int return_value;

      read_from_usermem(f->esp + 4, &filename, sizeof(filename));
      check_valid_str((const void*) filename, f->esp);

      return_value = sys_open(filename);
      f->eax = return_value;
      unpin_string((void *)filename);
      break;
    }

  case SYS_FILESIZE:
    {
      int fd, return_value;
      read_from_usermem(f->esp + 4, &fd, sizeof(fd));

      return_value = sys_filesize(fd);
      f->eax = return_value;
      break;
    }

  case SYS_READ:
    {
      int fd, return_value;
      void *buffer;
      unsigned size;

      read_from_usermem(f->esp + 4, &fd, sizeof(fd));
      read_from_usermem(f->esp + 8, &buffer, sizeof(buffer));
      read_from_usermem(f->esp + 12, &size, sizeof(size));
      check_valid_buffer((void*) buffer, (unsigned) size, f->esp, true);

      return_value = sys_read(fd, buffer, size);
      f->eax = (uint32_t) return_value;
      unpin_buffer((void *)buffer, (unsigned)size);
      break;
    }

  case SYS_WRITE:
    {
      int fd;
      int return_value;
      const void *buffer;
      unsigned size;

      read_from_usermem(f->esp + 4, &fd, sizeof(fd));
      read_from_usermem(f->esp + 8, &buffer, sizeof(buffer));
      read_from_usermem(f->esp + 12, &size, sizeof(size));
      check_valid_buffer((void*) buffer, (unsigned) size, f->esp, false);

      return_value = sys_write(fd, buffer, size);
      f->eax = (uint32_t) return_value;
      unpin_buffer((void *)buffer, (unsigned)size);
      break;
    }

  case SYS_SEEK:
    {
      int fd;
      unsigned position;

      read_from_usermem(f->esp + 4, &fd, sizeof(fd));
      read_from_usermem(f->esp + 8, &position, sizeof(position));

      sys_seek(fd, position);
      break;
    }

  case SYS_TELL:
    {
      int fd;
      unsigned return_value;

      read_from_usermem(f->esp + 4, &fd, sizeof(fd));

      return_value = sys_tell(fd);
      f->eax = (uint32_t) return_value;
      break;
    }

  case SYS_CLOSE:
    {
      int fd;
      read_from_usermem(f->esp + 4, &fd, sizeof(fd));

      sys_close(fd);
      break;
    }

  case SYS_MMAP:
    {
	mmap_getarg(f, &arg[0], 2);
	f->eax = sys_mmap(arg[0], (void *) arg[1]);
	break;
    }

  case SYS_MUNMAP:
    {
      int fd;
      read_from_usermem(f->esp + 4, &fd, sizeof(fd));
      sys_munmap(fd);
      break;
    }
  unpin_ptr(f ->esp);
  default:
    printf("default exit -1.\n");
    sys_exit(-1);
    break;
  }
}


void 
sys_halt(void) 
{
  shutdown_power_off();
}

void 
sys_exit(int status) 
{
  struct process_status *ps = thread_current()->p_status;
  if(ps != NULL) {
    /*ps->is_exited = true; */
    ps->exitcode = status;
  }
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}

pid_t
sys_exec(const char *cmdline)
{
  check_user((const uint8_t*) cmdline);
  lock_acquire (&filesys_lock);
  pid_t pid = process_execute(cmdline);
  lock_release (&filesys_lock);
  return pid;
}

int
 sys_wait(pid_t pid)
{
  return process_wait(pid);
}

bool
sys_create(const char* filename, unsigned initial_size)
 {
  bool return_value;
  check_user((const uint8_t*) filename);

  lock_acquire (&filesys_lock);
  return_value = filesys_create(filename, initial_size);
  lock_release (&filesys_lock);
  return return_value;
}

bool
sys_remove(const char* filename)
 {
  bool return_value;
  check_user((const uint8_t*) filename);

  lock_acquire (&filesys_lock);
  return_value = filesys_remove(filename);
  lock_release (&filesys_lock);
  return return_value;
}

int 
sys_open(const char* file)
 {
  check_user((const uint8_t*) file);

  struct file* file_opened;
  struct file_descriptor* fd = palloc_get_page(0);
  if (!fd) {
    return -1;
  }

  lock_acquire (&filesys_lock);
  file_opened = filesys_open(file);
  if (!file_opened) {
    palloc_free_page (fd);
    lock_release (&filesys_lock);
    return -1;
  }

  fd->file = file_opened;

  struct list* fd_list = &thread_current()->file_descriptors;
  if (list_empty(fd_list)) {
    fd->id = 3;
  }
  else {
    fd->id = (list_entry(list_back(fd_list), struct file_descriptor, elem)->id) + 1;
  }
  list_push_back(fd_list, &(fd->elem));

  lock_release (&filesys_lock);
  return fd->id;
}

int 
sys_filesize(int fd)
 {
  struct file_descriptor* file_d;

  lock_acquire (&filesys_lock);
  file_d = find_file_desc(thread_current(), fd);

  if(file_d == NULL) {
    lock_release (&filesys_lock);
    return -1;
  }

  int return_value = file_length(file_d->file);
  lock_release (&filesys_lock);
  return return_value;
}

void 
sys_seek(int fd, unsigned position)
 {
  lock_acquire (&filesys_lock);
  struct file_descriptor* file_d = find_file_desc(thread_current(), fd);

  if(file_d && file_d->file) {
    file_seek(file_d->file, position);
    lock_release (&filesys_lock);
  }
}

unsigned 
sys_tell(int fd)
 {
  lock_acquire (&filesys_lock);
  struct file_descriptor* file_d = find_file_desc(thread_current(), fd);

  unsigned return_value;
  if(file_d && file_d->file) {
    return_value = file_tell(file_d->file);
  }
  else
    return_value = -1;

  lock_release (&filesys_lock);
  return return_value;
}

void 
sys_close(int fd)
 {
  lock_acquire (&filesys_lock);
  struct file_descriptor* file_d = find_file_desc(thread_current(), fd);

  if(file_d && file_d->file) {
    file_close(file_d->file);
    list_remove(&(file_d->elem));
    palloc_free_page(file_d);
  }
  lock_release (&filesys_lock);
}

int 
sys_read(int fd, void *buffer, unsigned size)
{
  check_user((const uint8_t*) buffer);
  check_user((const uint8_t*) buffer + size - 1);

  lock_acquire (&filesys_lock);
  int return_value;

  if(fd == 0) {
    unsigned i;
    for(i = 0; i < size; ++i) {
      if(!put_user(buffer + i, input_getc())) {
        lock_release (&filesys_lock);
        sys_exit(-1);
      }
    }
    return_value = size;
  }
  else {
    struct file_descriptor* file_d = find_file_desc(thread_current(), fd);
    if(file_d && file_d->file) {
      return_value = file_read(file_d->file, buffer, size);
    }
    else
      return_value = -1;
  }

  lock_release (&filesys_lock);
  return return_value;
}

int 
sys_write(int fd, const void *buffer, unsigned size)
{
  check_user((const uint8_t*) buffer);
  check_user((const uint8_t*) buffer + size - 1);

  lock_acquire (&filesys_lock);
  int return_value;

  if(fd == 1) { 
    /* Write to system console: First to implement. */
    putbuf(buffer, size);
    return_value = size;
  }
  else {
    struct file_descriptor* file_d = find_file_desc(thread_current(), fd);
    if(file_d && file_d->file) {
      return_value = file_write(file_d->file, buffer, size);
    }
    else
      return_value = -1;
  }

  lock_release (&filesys_lock);
  return return_value;
}

int 
sys_mmap (int fd, void *addr)
{
  /*if (fd <= 1){
      return -1;
  }*/
  struct file_descriptor *file_d = find_file_desc(thread_current(), fd);
  struct file *old_file = file_d ->file;
  if (!old_file || !is_user_vaddr(addr) || addr < US_VADDR_BTM || ((uint32_t) addr % PGSIZE) != 0){
      return -1;
    }
  struct file *file = file_reopen(old_file);
  if (!file || file_length(old_file) == 0){
      return -1;
    }
  thread_current()->mapid++;
  off_t ofs = 0;
  uint32_t read_bytes = file_length(file);
  while (read_bytes > 0){
      uint32_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
      uint32_t page_zero_bytes = PGSIZE - page_read_bytes;
      if (!add_mmap_to_page_table(file, ofs, addr, page_read_bytes, page_zero_bytes)){
	    sys_munmap(thread_current()->mapid);
	    return -1;
	  }
      read_bytes -= page_read_bytes;
      ofs += page_read_bytes;
      addr += PGSIZE;
  }
  return thread_current()->mapid;
}

void
sys_munmap(int fd)
{
  process_remove_mmap(fd);
}

static void
check_user (const uint8_t *uaddr)
{
  /* God, it seems it important to call that assembly code before calling fail_invalid_access
     But I DON'T KNOW WHY */
  if(get_user (uaddr) == -1){
      fail_invalid_access();
  }
}

/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the byte value if successful, -1 if a segfault
   occurred. */
static int32_t
get_user (const uint8_t *uaddr)
{
  /* If UADDR >= PHYS_BASE. then there is a segfault. */
  if (! ((void*)uaddr < PHYS_BASE)){
    return -1;
  }

  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
      : "=&a" (result) : "m" (*uaddr));
  return result;
}

/* Writes BYTE to user address UDST.
   UDST must be below PHYS_BASE.
   Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  /* UDST >= PHYS_BASE => Segfault */
  if (! ((void*)udst < PHYS_BASE)) {
    return false;
  }

  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
      : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

/*  Read specific bytes of user memory with given starting address,
    write to a given address, return the bytes. If invalid memory 
    encountered, -1 is returned instread. */
static int
read_from_usermem(void *src, void *dst, size_t bytes) {
  int32_t val;
  size_t i;
  for(i=0; i<bytes; i++){
    /*check_valid_ptr((const void*)( (int *)src - 4 + i + 1 ), src );*/
    if(get_user(src + i) == -1) {
      fail_invalid_access();
    }
    else {
      val = get_user(src + i);
      *(char*)(dst + i) = val & 0xff;
    }
  }
  return (int)bytes;
}

void
mmap_getarg(struct intr_frame *f, int *arg, int n){
    int i;
    int *ptr;
    for (i = 0; i < n; i++){
        ptr = (int *) f->esp + i + 1;
        check_valid_ptr((const void *)ptr, f->esp);
        arg[i] = *ptr;
    }
}


static struct file_descriptor*
find_file_desc(struct thread *t, int fd)
{
  if (fd < 3) {
    return NULL;
  }
  struct list_elem *e;
  if (! list_empty(&t->file_descriptors)) {
    for(e = list_begin(&t->file_descriptors); e != list_end(&t->file_descriptors); e = list_next(e)){
      struct file_descriptor *desc = list_entry(e, struct file_descriptor, elem);
      if(desc->id == fd) {
        return desc;
      }
    }
  }

  return NULL;
}


void 
unpin_ptr (void* vaddr)
{
  struct supplement_pagetable_entry *spte = get_spte(vaddr);
  if (spte){
      spte->pinned = false;
    }
}

void 
unpin_string (void* str)
{
  unpin_ptr(str);
  while (* (char *) str != 0){
      str = (char *) str + 1;
      unpin_ptr(str);
    }
}

void 
unpin_buffer (void* buffer, unsigned size)
{
  unsigned i;
  char* localbuf = (char *) buffer;
  for (i = 0; i < size; i++){
      unpin_ptr(localbuf);
      localbuf++;
    }
}

struct supplement_pagetable_entry *
check_valid_ptr(const void *vaddr, void* esp)
{
  if(!is_user_vaddr(vaddr) || vaddr < US_VADDR_BTM){
      sys_exit(-1);
  }
  bool load = false;
  struct supplement_pagetable_entry *spe = get_spte((void *) vaddr);
  if (spe){
      load_page(spe);
      load = spe ->loaded;
  }
  else if (vaddr >= esp - 32){
      load = grow_stack((void *)vaddr);
  }
  if (!load){
      sys_exit(-1);
  }
  return spe;
}

void 
check_valid_buffer(void* buffer, unsigned size, void* esp, bool write)
{
  unsigned i;
  char* localbuf = (char *)buffer;
  for (i = 0; i < size; i++){
      struct supplement_pagetable_entry *spe = check_valid_ptr((const void*) localbuf, esp);
      if (spe && write){
          if (! spe ->writable){
              sys_exit(-1);
          }
      }
      localbuf++;
  }
}

void
check_valid_str(const void* str, void* esp)
{
  check_valid_ptr(str, esp);
  while(* (char*)str != 0){
      str = (char*) str + 1;
      check_valid_ptr(str, esp);
  }
}
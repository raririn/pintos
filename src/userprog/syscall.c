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

#define MAX_ARGS 3
#define US_VADDR_BTM ((void *) 0x08048000)

static void syscall_handler (struct intr_frame *);

static void check_user (const uint8_t *uaddr);
static int32_t get_user (const uint8_t *uaddr);
static bool put_user (uint8_t *udst, uint8_t byte);
static int read_from_usermem (void *src, void *des, size_t bytes);

static struct file_desc* find_file_desc(struct thread *, int fd);

/* Syscall functions. */
void sys_halt(void);
void sys_exit(int);
pid_t sys_exec(const char *cmdline);
int sys_wait(pid_t pid);
bool sys_create(const char* filename, unsigned initial_size);
bool sys_remove(const char* filename);
int sys_open(const char* file);
int sys_filesize(int fd);
int sys_read(int fd, void *buffer, unsigned size);
int sys_write(int fd, const void *buffer, unsigned size);
void sys_seek(int fd, unsigned position);
unsigned sys_tell(int fd);
void sys_close(int fd);

struct lock filesys_lock;

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
  read_from_usermem(f->esp, &syscall_ID, sizeof(syscall_ID));

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

      int return_value = sys_exec((const char*) cmdline);
      f->eax = (uint32_t) return_value;
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

      return_value = sys_create(filename, initial_size);
      f->eax = return_value;
      break;
    }

  case SYS_REMOVE:
    {
      const char* filename;
      bool return_value;

      read_from_usermem(f->esp + 4, &filename, sizeof(filename));

      return_value = sys_remove(filename);
      f->eax = return_value;
      break;
    }

  case SYS_OPEN:
    {
      const char* filename;
      int return_value;

      read_from_usermem(f->esp + 4, &filename, sizeof(filename));

      return_value = sys_open(filename);
      f->eax = return_value;
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

      return_value = sys_read(fd, buffer, size);
      f->eax = (uint32_t) return_value;
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

      return_value = sys_write(fd, buffer, size);
      f->eax = (uint32_t) return_value;
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

  default:
    sys_exit(-1);
    break;
  }

}


void sys_halt(void) {
  shutdown_power_off();
}

void sys_exit(int status) {
  printf("%s: exit(%d)\n", thread_current()->name, status);
  struct process_control_block *pcb = thread_current()->pcb;
  if(pcb != NULL) {
    pcb->exited = true;
    pcb->exitcode = status;
  }

  thread_exit();
}

pid_t sys_exec(const char *cmdline) {
  check_user((const uint8_t*) cmdline);

  lock_acquire (&filesys_lock);
  pid_t pid = process_execute(cmdline);
  lock_release (&filesys_lock);
  return pid;
}

int sys_wait(pid_t pid) {
  return process_wait(pid);
}

bool sys_create(const char* filename, unsigned initial_size) {
  bool return_value;
  check_user((const uint8_t*) filename);

  lock_acquire (&filesys_lock);
  return_value = filesys_create(filename, initial_size);
  lock_release (&filesys_lock);
  return return_value;
}

bool sys_remove(const char* filename) {
  bool return_value;
  check_user((const uint8_t*) filename);

  lock_acquire (&filesys_lock);
  return_value = filesys_remove(filename);
  lock_release (&filesys_lock);
  return return_value;
}

int sys_open(const char* file) {
  check_user((const uint8_t*) file);

  struct file* file_opened;
  struct file_desc* fd = palloc_get_page(0);
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
    fd->id = (list_entry(list_back(fd_list), struct file_desc, elem)->id) + 1;
  }
  list_push_back(fd_list, &(fd->elem));

  lock_release (&filesys_lock);
  return fd->id;
}

int sys_filesize(int fd) {
  struct file_desc* file_d;

  lock_acquire (&filesys_lock);
  file_d = find_file_desc(thread_current(), fd);

  if(file_d == NULL) {
    lock_release (&filesys_lock);
    return -1;
  }

  int ret = file_length(file_d->file);
  lock_release (&filesys_lock);
  return ret;
}

void sys_seek(int fd, unsigned position) {
  lock_acquire (&filesys_lock);
  struct file_desc* file_d = find_file_desc(thread_current(), fd);

  if(file_d && file_d->file) {
    file_seek(file_d->file, position);
  }
  else
    return;

  lock_release (&filesys_lock);
}

unsigned sys_tell(int fd) {
  lock_acquire (&filesys_lock);
  struct file_desc* file_d = find_file_desc(thread_current(), fd);

  unsigned ret;
  if(file_d && file_d->file) {
    ret = file_tell(file_d->file);
  }
  else
    ret = -1;

  lock_release (&filesys_lock);
  return ret;
}

void sys_close(int fd) {
  lock_acquire (&filesys_lock);
  struct file_desc* file_d = find_file_desc(thread_current(), fd);

  if(file_d && file_d->file) {
    file_close(file_d->file);
    list_remove(&(file_d->elem));
    palloc_free_page(file_d);
  }
  lock_release (&filesys_lock);
}

int sys_read(int fd, void *buffer, unsigned size) {
  check_user((const uint8_t*) buffer);
  check_user((const uint8_t*) buffer + size - 1);

  lock_acquire (&filesys_lock);
  int ret;

  if(fd == 0) {
    unsigned i;
    for(i = 0; i < size; ++i) {
      if(!put_user(buffer + i, input_getc())) {
        lock_release (&filesys_lock);
        sys_exit(-1);
      }
    }
    ret = size;
  }
  else {
    struct file_desc* file_d = find_file_desc(thread_current(), fd);
    if(file_d && file_d->file) {
      ret = file_read(file_d->file, buffer, size);
    }
    else // no such file or can't open
      ret = -1;
  }

  lock_release (&filesys_lock);
  return ret;
}

int sys_write(int fd, const void *buffer, unsigned size) {
  check_user((const uint8_t*) buffer);
  check_user((const uint8_t*) buffer + size - 1);

  lock_acquire (&filesys_lock);
  int ret;

  if(fd == 1) { 
    /* Write to system console: First to implement. */
    putbuf(buffer, size);
    ret = size;
  }
  else {
    struct file_desc* file_d = find_file_desc(thread_current(), fd);
    if(file_d && file_d->file) {
      ret = file_write(file_d->file, buffer, size);
    }
    else
      ret = -1;
  }

  lock_release (&filesys_lock);
  return ret;
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
read_from_usermem(void *src, void *dst, size_t bytes)
{
  int32_t value;
  size_t i;
  for(i=0; i<bytes; i++) {
    value = get_user(src + i);
    if(value == -1) // segfault or invalid memory access
      fail_invalid_access();

    *(char*)(dst + i) = value & 0xff;
  }
  return (int)bytes;
}

/****************** Helper Functions on File Access ********************/

static struct file_desc*
find_file_desc(struct thread *t, int fd)
{
  ASSERT (t != NULL);

  if (fd < 3) {
    return NULL;
  }

  struct list_elem *e;

  if (! list_empty(&t->file_descriptors)) {
    for(e = list_begin(&t->file_descriptors);
        e != list_end(&t->file_descriptors); e = list_next(e))
    {
      struct file_desc *desc = list_entry(e, struct file_desc, elem);
      if(desc->id == fd) {
        return desc;
      }
    }
  }

  return NULL; // not found
}

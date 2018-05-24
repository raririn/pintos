#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "userprog/process.h"

#define MAX_ARGS 3
#define US_VADDR_BTM ((void *) 0x08048000)

void syscall_init (void);


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
int sys_mmap(int fd, void *addr);
void sys_munmap(int fd);

struct lock filesys_lock;

#endif /* userprog/syscall.h */

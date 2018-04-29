#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"

typedef int pid_t;

#define PID_ERROR         ((pid_t) -1)
#define PID_INITIALIZING  ((pid_t) -2)


pid_t process_execute (const char *cmdline);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct process_status* get_child(tid_t child_tid);
void remove_single_child_process(tid_t child_tid);
void remove_multiple_child_process(void);
void process_close_file(void);

/* A more detailed info of process. */
struct process_status{
  /* Basic info */
  pid_t pid;
  const char* intr;      /* The instruction the process is running. */
  bool waiting;             /* If there is a parent process waiting for it. */
  bool is_exited;              /* If it has exited. */
  bool is_parent_exited;    /* If its parent has exited. */
  int32_t exitcode;         /* Get from process_exit() */
  /* Shared info */
  struct list_elem elem;    /* child_list element */
  /* Locks */
  struct semaphore process_lock;    /* Lock between start_process() and process_execute() */
  struct semaphore wait_lock;       /* Lock in process_wait() */
};

/* File descriptor */
struct file_descriptor {
  int id;
  struct list_elem elem;
  struct file* file;
};

#endif /* userprog/process.h */


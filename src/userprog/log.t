In file included from ../../threads/init.c:26:0:
../../userprog/process.h:19:6: warning: function declaration isn’t a prototype [-Wstrict-prototypes]
 void remove_single_child_process()
      ^
../../userprog/process.h: In function ‘remove_single_child_process’:
../../userprog/process.h:22:1: warning: empty declaration [enabled by default]
 struct process_control_block {
 ^
../../userprog/process.h:38:1: warning: empty declaration [enabled by default]
 struct file_desc {
 ^
In file included from ../../threads/init.c:30:0:
../../userprog/tss.h:6:1: warning: empty declaration [enabled by default]
 struct tss;
 ^
In file included from ../../threads/init.c:35:0:
../../devices/block.h:15:18: error: storage class specified for parameter ‘block_sector_t’
 typedef uint32_t block_sector_t;
                  ^
../../devices/block.h:23:1: warning: empty declaration [enabled by default]
 struct block;
 ^
../../devices/block.h:26:1: warning: empty declaration [enabled by default]
 enum block_type
 ^
../../devices/block.h:53:1: error: expected declaration specifiers before ‘block_sector_t’
 block_sector_t block_size (struct block *);
 ^
../../devices/block.h:54:34: error: expected declaration specifiers or ‘...’ before ‘block_sector_t’
 void block_read (struct block *, block_sector_t, void *);
                                  ^
../../devices/block.h:55:35: error: expected declaration specifiers or ‘...’ before ‘block_sector_t’
 void block_write (struct block *, block_sector_t, const void *);
                                   ^
../../devices/block.h:66:30: error: expected declaration specifiers or ‘...’ before ‘block_sector_t’
     void (*read) (void *aux, block_sector_t, void *buffer);
                              ^
../../devices/block.h:67:31: error: expected declaration specifiers or ‘...’ before ‘block_sector_t’
     void (*write) (void *aux, block_sector_t, const void *buffer);
                               ^
../../devices/block.h:68:3: warning: no semicolon at end of struct or union [enabled by default]
   };
   ^
../../devices/block.h:64:1: warning: empty declaration [enabled by default]
 struct block_operations
 ^
../../devices/block.h:71:55: error: expected declaration specifiers or ‘...’ before ‘block_sector_t’
                               const char *extra_info, block_sector_t size,
                                                       ^
In file included from ../../filesys/filesys.h:5:0,
                 from ../../threads/init.c:37:
../../filesys/off_t.h:9:17: error: storage class specified for parameter ‘off_t’
 typedef int32_t off_t;
                 ^
In file included from ../../threads/init.c:37:0:
../../filesys/filesys.h:16:40: error: expected declaration specifiers or ‘...’ before ‘off_t’
 bool filesys_create (const char *name, off_t initial_size);
                                        ^
../../threads/init.c:46:13: error: storage class specified for parameter ‘format_filesys’
 static bool format_filesys;
             ^
../../threads/init.c:50:20: error: storage class specified for parameter ‘filesys_bdev_name’
 static const char *filesys_bdev_name;
                    ^
../../threads/init.c:51:20: error: storage class specified for parameter ‘scratch_bdev_name’
 static const char *scratch_bdev_name;
                    ^
../../threads/init.c:58:15: error: storage class specified for parameter ‘user_page_limit’
 static size_t user_page_limit = SIZE_MAX;
               ^
../../threads/init.c:58:1: error: parameter ‘user_page_limit’ is initialized
 static size_t user_page_limit = SIZE_MAX;
 ^
../../threads/init.c:60:13: error: storage class specified for parameter ‘bss_init’
 static void bss_init (void);
             ^
../../threads/init.c:61:13: error: storage class specified for parameter ‘paging_init’
 static void paging_init (void);
             ^
../../threads/init.c:63:15: error: storage class specified for parameter ‘read_command_line’
 static char **read_command_line (void);
               ^
../../threads/init.c:64:15: error: storage class specified for parameter ‘parse_options’
 static char **parse_options (char **argv);
               ^
../../threads/init.c:65:13: error: storage class specified for parameter ‘run_actions’
 static void run_actions (char **argv);
             ^
../../threads/init.c:66:13: error: storage class specified for parameter ‘usage’
 static void usage (void);
             ^
../../threads/init.c:69:13: error: storage class specified for parameter ‘locate_block_devices’
 static void locate_block_devices (void);
             ^
../../threads/init.c:70:13: error: storage class specified for parameter ‘locate_block_device’
 static void locate_block_device (enum block_type, const char *name);
             ^
../../threads/init.c:73:5: warning: ‘main’ is usually a function [-Wmain]
 int main (void) NO_RETURN;
     ^
../../threads/init.c:78:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:148:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:159:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:195:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:230:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:283:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:299:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:350:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:393:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:407:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
 {
 ^
../../threads/init.c:73:5: error: declaration for parameter ‘main’ but no such parameter
 int main (void) NO_RETURN;
     ^
../../threads/init.c:70:13: error: declaration for parameter ‘locate_block_device’ but no such parameter
 static void locate_block_device (enum block_type, const char *name);
             ^
../../threads/init.c:69:13: error: declaration for parameter ‘locate_block_devices’ but no such parameter
 static void locate_block_devices (void);
             ^
../../threads/init.c:66:13: error: declaration for parameter ‘usage’ but no such parameter
 static void usage (void);
             ^
../../threads/init.c:65:13: error: declaration for parameter ‘run_actions’ but no such parameter
 static void run_actions (char **argv);
             ^
../../threads/init.c:64:15: error: declaration for parameter ‘parse_options’ but no such parameter
 static char **parse_options (char **argv);
               ^
../../threads/init.c:63:15: error: declaration for parameter ‘read_command_line’ but no such parameter
 static char **read_command_line (void);
               ^
../../threads/init.c:61:13: error: declaration for parameter ‘paging_init’ but no such parameter
 static void paging_init (void);
             ^
../../threads/init.c:60:13: error: declaration for parameter ‘bss_init’ but no such parameter
 static void bss_init (void);
             ^
../../threads/init.c:58:15: error: declaration for parameter ‘user_page_limit’ but no such parameter
 static size_t user_page_limit = SIZE_MAX;
               ^
../../threads/init.c:51:20: error: declaration for parameter ‘scratch_bdev_name’ but no such parameter
 static const char *scratch_bdev_name;
                    ^
../../threads/init.c:50:20: error: declaration for parameter ‘filesys_bdev_name’ but no such parameter
 static const char *filesys_bdev_name;
                    ^
../../threads/init.c:46:13: error: declaration for parameter ‘format_filesys’ but no such parameter
 static bool format_filesys;
             ^
../../threads/init.c:42:11: error: declaration for parameter ‘init_page_dir’ but no such parameter
 uint32_t *init_page_dir;
           ^
In file included from ../../threads/init.c:38:0:
../../filesys/fsutil.h:8:6: error: declaration for parameter ‘fsutil_append’ but no such parameter
 void fsutil_append (char **argv);
      ^
../../filesys/fsutil.h:7:6: error: declaration for parameter ‘fsutil_extract’ but no such parameter
 void fsutil_extract (char **argv);
      ^
../../filesys/fsutil.h:6:6: error: declaration for parameter ‘fsutil_rm’ but no such parameter
 void fsutil_rm (char **argv);
      ^
../../filesys/fsutil.h:5:6: error: declaration for parameter ‘fsutil_cat’ but no such parameter
 void fsutil_cat (char **argv);
      ^
../../filesys/fsutil.h:4:6: error: declaration for parameter ‘fsutil_ls’ but no such parameter
 void fsutil_ls (char **argv);
      ^
In file included from ../../threads/init.c:37:0:
../../filesys/filesys.h:18:6: error: declaration for parameter ‘filesys_remove’ but no such parameter
 bool filesys_remove (const char *name);
      ^
../../filesys/filesys.h:17:14: error: declaration for parameter ‘filesys_open’ but no such parameter
 struct file *filesys_open (const char *name);
              ^
../../filesys/filesys.h:15:6: error: declaration for parameter ‘filesys_done’ but no such parameter
 void filesys_done (void);
      ^
../../filesys/filesys.h:14:6: error: declaration for parameter ‘filesys_init’ but no such parameter
 void filesys_init (bool format);
      ^
../../filesys/filesys.h:12:15: error: declaration for parameter ‘fs_device’ but no such parameter
 struct block *fs_device;
               ^
In file included from ../../filesys/filesys.h:5:0,
                 from ../../threads/init.c:37:
../../filesys/off_t.h:9:17: error: declaration for parameter ‘off_t’ but no such parameter
 typedef int32_t off_t;
                 ^
In file included from ../../threads/init.c:36:0:
../../devices/ide.h:4:6: error: declaration for parameter ‘ide_init’ but no such parameter
 void ide_init (void);
      ^
In file included from ../../threads/init.c:35:0:
../../devices/block.h:60:6: error: declaration for parameter ‘block_print_stats’ but no such parameter
 void block_print_stats (void);
      ^
../../devices/block.h:57:17: error: declaration for parameter ‘block_type’ but no such parameter
 enum block_type block_type (struct block *);
                 ^
../../devices/block.h:56:13: error: declaration for parameter ‘block_name’ but no such parameter
 const char *block_name (struct block *);
             ^
../../devices/block.h:50:15: error: declaration for parameter ‘block_next’ but no such parameter
 struct block *block_next (struct block *);
               ^
../../devices/block.h:49:15: error: declaration for parameter ‘block_first’ but no such parameter
 struct block *block_first (void);
               ^
../../devices/block.h:47:15: error: declaration for parameter ‘block_get_by_name’ but no such parameter
 struct block *block_get_by_name (const char *name);
               ^
../../devices/block.h:46:6: error: declaration for parameter ‘block_set_role’ but no such parameter
 void block_set_role (enum block_type, struct block *);
      ^
../../devices/block.h:45:15: error: declaration for parameter ‘block_get_role’ but no such parameter
 struct block *block_get_role (enum block_type);
               ^
../../devices/block.h:42:13: error: declaration for parameter ‘block_type_name’ but no such parameter
 const char *block_type_name (enum block_type);
             ^
../../devices/block.h:15:18: error: declaration for parameter ‘block_sector_t’ but no such parameter
 typedef uint32_t block_sector_t;
                  ^
In file included from ../../threads/init.c:30:0:
../../userprog/tss.h:9:6: error: declaration for parameter ‘tss_update’ but no such parameter
 void tss_update (void);
      ^
../../userprog/tss.h:8:13: error: declaration for parameter ‘tss_get’ but no such parameter
 struct tss *tss_get (void);
             ^
../../userprog/tss.h:7:6: error: declaration for parameter ‘tss_init’ but no such parameter
 void tss_init (void);
      ^
In file included from ../../threads/init.c:29:0:
../../userprog/syscall.h:4:6: error: declaration for parameter ‘syscall_init’ but no such parameter
 void syscall_init (void);
      ^
In file included from ../../threads/init.c:28:0:
../../userprog/gdt.h:13:6: error: declaration for parameter ‘gdt_init’ but no such parameter
 void gdt_init (void);
      ^
In file included from ../../threads/init.c:27:0:
../../userprog/exception.h:10:6: error: declaration for parameter ‘exception_print_stats’ but no such parameter
 void exception_print_stats (void);
      ^
../../userprog/exception.h:9:6: error: declaration for parameter ‘exception_init’ but no such parameter
 void exception_init (void);
      ^
../../threads/init.c:428:1: error: expected ‘{’ at end of input
 }
 ^
../../threads/init.c:73:5: warning: unused parameter ‘main’ [-Wunused-parameter]
 int main (void) NO_RETURN;
     ^
../../threads/init.c:70:13: warning: unused parameter ‘locate_block_device’ [-Wunused-parameter]
 static void locate_block_device (enum block_type, const char *name);
             ^
../../threads/init.c:69:13: warning: unused parameter ‘locate_block_devices’ [-Wunused-parameter]
 static void locate_block_devices (void);
             ^
../../threads/init.c:66:13: warning: unused parameter ‘usage’ [-Wunused-parameter]
 static void usage (void);
             ^
../../threads/init.c:65:13: warning: unused parameter ‘run_actions’ [-Wunused-parameter]
 static void run_actions (char **argv);
             ^
../../threads/init.c:64:15: warning: unused parameter ‘parse_options’ [-Wunused-parameter]
 static char **parse_options (char **argv);
               ^
../../threads/init.c:63:15: warning: unused parameter ‘read_command_line’ [-Wunused-parameter]
 static char **read_command_line (void);
               ^
../../threads/init.c:61:13: warning: unused parameter ‘paging_init’ [-Wunused-parameter]
 static void paging_init (void);
             ^
../../threads/init.c:60:13: warning: unused parameter ‘bss_init’ [-Wunused-parameter]
 static void bss_init (void);
             ^
../../threads/init.c:58:15: warning: unused parameter ‘user_page_limit’ [-Wunused-parameter]
 static size_t user_page_limit = SIZE_MAX;
               ^
../../threads/init.c:51:20: warning: unused parameter ‘scratch_bdev_name’ [-Wunused-parameter]
 static const char *scratch_bdev_name;
                    ^
../../threads/init.c:50:20: warning: unused parameter ‘filesys_bdev_name’ [-Wunused-parameter]
 static const char *filesys_bdev_name;
                    ^
../../threads/init.c:46:13: warning: unused parameter ‘format_filesys’ [-Wunused-parameter]
 static bool format_filesys;
             ^
../../threads/init.c:42:11: warning: unused parameter ‘init_page_dir’ [-Wunused-parameter]
 uint32_t *init_page_dir;
           ^
In file included from ../../threads/init.c:38:0:
../../filesys/fsutil.h:8:6: warning: unused parameter ‘fsutil_append’ [-Wunused-parameter]
 void fsutil_append (char **argv);
      ^
../../filesys/fsutil.h:7:6: warning: unused parameter ‘fsutil_extract’ [-Wunused-parameter]
 void fsutil_extract (char **argv);
      ^
../../filesys/fsutil.h:6:6: warning: unused parameter ‘fsutil_rm’ [-Wunused-parameter]
 void fsutil_rm (char **argv);
      ^
../../filesys/fsutil.h:5:6: warning: unused parameter ‘fsutil_cat’ [-Wunused-parameter]
 void fsutil_cat (char **argv);
      ^
../../filesys/fsutil.h:4:6: warning: unused parameter ‘fsutil_ls’ [-Wunused-parameter]
 void fsutil_ls (char **argv);
      ^
In file included from ../../threads/init.c:37:0:
../../filesys/filesys.h:18:6: warning: unused parameter ‘filesys_remove’ [-Wunused-parameter]
 bool filesys_remove (const char *name);
      ^
../../filesys/filesys.h:17:14: warning: unused parameter ‘filesys_open’ [-Wunused-parameter]
 struct file *filesys_open (const char *name);
              ^
../../filesys/filesys.h:15:6: warning: unused parameter ‘filesys_done’ [-Wunused-parameter]
 void filesys_done (void);
      ^
../../filesys/filesys.h:14:6: warning: unused parameter ‘filesys_init’ [-Wunused-parameter]
 void filesys_init (bool format);
      ^
../../filesys/filesys.h:12:15: warning: unused parameter ‘fs_device’ [-Wunused-parameter]
 struct block *fs_device;
               ^
In file included from ../../filesys/filesys.h:5:0,
                 from ../../threads/init.c:37:
../../filesys/off_t.h:9:17: warning: unused parameter ‘off_t’ [-Wunused-parameter]
 typedef int32_t off_t;
                 ^
In file included from ../../threads/init.c:36:0:
../../devices/ide.h:4:6: warning: unused parameter ‘ide_init’ [-Wunused-parameter]
 void ide_init (void);
      ^
In file included from ../../threads/init.c:35:0:
../../devices/block.h:60:6: warning: unused parameter ‘block_print_stats’ [-Wunused-parameter]
 void block_print_stats (void);
      ^
../../devices/block.h:57:17: warning: unused parameter ‘block_type’ [-Wunused-parameter]
 enum block_type block_type (struct block *);
                 ^
../../devices/block.h:56:13: warning: unused parameter ‘block_name’ [-Wunused-parameter]
 const char *block_name (struct block *);
             ^
../../devices/block.h:50:15: warning: unused parameter ‘block_next’ [-Wunused-parameter]
 struct block *block_next (struct block *);
               ^
../../devices/block.h:49:15: warning: unused parameter ‘block_first’ [-Wunused-parameter]
 struct block *block_first (void);
               ^
../../devices/block.h:47:15: warning: unused parameter ‘block_get_by_name’ [-Wunused-parameter]
 struct block *block_get_by_name (const char *name);
               ^
../../devices/block.h:46:6: warning: unused parameter ‘block_set_role’ [-Wunused-parameter]
 void block_set_role (enum block_type, struct block *);
      ^
../../devices/block.h:45:15: warning: unused parameter ‘block_get_role’ [-Wunused-parameter]
 struct block *block_get_role (enum block_type);
               ^
../../devices/block.h:42:13: warning: unused parameter ‘block_type_name’ [-Wunused-parameter]
 const char *block_type_name (enum block_type);
             ^
../../devices/block.h:15:18: warning: unused parameter ‘block_sector_t’ [-Wunused-parameter]
 typedef uint32_t block_sector_t;
                  ^
In file included from ../../threads/init.c:30:0:
../../userprog/tss.h:9:6: warning: unused parameter ‘tss_update’ [-Wunused-parameter]
 void tss_update (void);
      ^
../../userprog/tss.h:8:13: warning: unused parameter ‘tss_get’ [-Wunused-parameter]
 struct tss *tss_get (void);
             ^
../../userprog/tss.h:7:6: warning: unused parameter ‘tss_init’ [-Wunused-parameter]
 void tss_init (void);
      ^
In file included from ../../threads/init.c:29:0:
../../userprog/syscall.h:4:6: warning: unused parameter ‘syscall_init’ [-Wunused-parameter]
 void syscall_init (void);
      ^
In file included from ../../threads/init.c:28:0:
../../userprog/gdt.h:13:6: warning: unused parameter ‘gdt_init’ [-Wunused-parameter]
 void gdt_init (void);
      ^
In file included from ../../threads/init.c:27:0:
../../userprog/exception.h:10:6: warning: unused parameter ‘exception_print_stats’ [-Wunused-parameter]
 void exception_print_stats (void);
      ^
../../userprog/exception.h:9:6: warning: unused parameter ‘exception_init’ [-Wunused-parameter]
 void exception_init (void);
      ^
make[1]: *** [threads/init.o] Error 1
make: *** [all] Error 2

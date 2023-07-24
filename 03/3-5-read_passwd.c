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
  char buf[0x1000];
  // file_handle構造体の初期化
  struct my_file_handle h = {
    .handle_bytes = 8,
    .handle_type = 1,
    // inode番号21435を16進数に変換すると53BBになる。それをリトルエンディアンで格納
    .f_handle = {0xbb, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  };

  // ホストのファイルシステム上にあるファイルのファイルディスクリプタを取得
  if ((fd1 = open("/etc/hosts", O_RDONLY)) < 0)
    die("failed to open");

  // open_by_handle_at()システムコールで、file_handleで指定されたinode番号のファイルを読み出す
  if ((fd2 = open_by_handle_at(fd1, (struct file_handle *)&h, O_RDONLY)) < 0)
    die("failed to open_by_handle_at");

  memset(buf, 0, sizeof(buf));

  if (read(fd2, buf, sizeof(buf) - 1) < 0)
    die("failed to read");

  fprintf(stderr, "%s", buf);
  close(fd2);
  close(fd1);
  return 0;
}

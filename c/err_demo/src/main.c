#include <errno.h>
#include <stdio.h>

int main(void) {
  errno = ENOENT; // No such file or directory
  perror("test ENOENT");
  errno = EACCES; // Permission denied
  perror("test EACCES");
  errno = EINTR; // Interrupted system call
  perror("test EINTR");
  return 0;
}

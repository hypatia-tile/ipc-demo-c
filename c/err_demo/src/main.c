
#include <errno.h>
#include <stdio.h>

int main(void) {
  errno = ENOENT;
  perror("test");
}

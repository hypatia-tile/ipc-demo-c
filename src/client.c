#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static void die(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  const char *path = "./ipc.sock";
  const char *msg = (argc >= 2) ? argv[1] : "hello\n";

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0)
    die("socket");

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

  if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    die("connect");

  if (write(fd, msg, strlen(msg)) < 0)
    die("write");

  char buf[1024];
  ssize_t n = read(fd, buf, sizeof(buf) - 1);
  if (n < 0)
    die("read");
  buf[n] = '\0';

  printf("%s", buf);
  close(fd);
  return 0;
}

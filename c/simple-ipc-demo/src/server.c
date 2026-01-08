#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/**
 * A simple error handling function
 * @param msg The error message to display
 * Exits the program with status 1 after printing the error message
 * Inplicitly depends on errno being set appropriately
 */
static void die(const char *msg) {
  perror(msg);
  exit(1);
}

/**
 * A simple IPC server using Unix domain sockets
 */
int main(void) {
  const char *path = "./ipc.sock";

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0)
    die("socket");

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

  // Remove stale socket file from previous run
  unlink(path);

  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    die("bind");

  if (listen(fd, 10) < 0)
    die("listen");

  printf("server PID=%ld listening on %s\n", (long)getpid(), path);

  for (;;) {
    int cfd = accept(fd, NULL, NULL);
    if (cfd < 0) {
      if (errno == EINTR)
        continue;
      die("accept");
    }

    char buf[1024];
    ssize_t n = read(cfd, buf, sizeof(buf) - 1);
    if (n < 0) {
      perror("read");
      close(cfd);
      continue;
    }
    if (n == 0) { // client closed without sending
      close(cfd);
      continue;
    }

    buf[n] = '\0';

    char out[1200];
    snprintf(out, sizeof(out), "ack: %s", buf);

    if (write(cfd, out, strlen(out)) < 0) {
      perror("write");
    }

    close(cfd);
  }

  // unreachable
  close(fd);
  unlink(path);
  return 0;
}

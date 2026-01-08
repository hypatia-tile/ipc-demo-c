#define _POSIX_C_SOURCE 200809L
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
 * A simple IPC client using Unix domain sockets
 */
int main(int argc, char **argv) {
  const char *path = "./ipc.sock";
  const char *msg = (argc >= 2) ? argv[1] : "hello\n";

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0)
    die("socket");

  /**
   * Set up the socket address structure
   * AF_UNIX indicates a Unix domain socket
   * sun_path is the file system path to the socket
   */
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

  /**
   * Associate the socket, which we can refer to via addr.sun_path,
   * wite h the socket file descriptor fd
   * > If the socket file does not exist, connect will fail
   * > with the message: "No such file or directory
   * > If the server is not listening on the socket,
   * > connect will fail with "Connection refused"
   */
  if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    die("connect");

  if (write(fd, msg, strlen(msg)) < 0)
    die("write");

  char buf[1024];
  ssize_t n = read(fd, buf, sizeof(buf) - 1);
  if (n < 0)
    die("read");
  buf[n] = '\0'; // Prevent over run when printing

  printf("%s", buf);
  close(fd);
  return 0;
}

# ipc-demo-c
## The simplest useful C IPC demo:

Unix domain socket  A real client/server IPC on one machine, no network, no 'parent/child only' limitation.

## Structure

* `server`: listens on `./ipc.sock`, accepts one client at a time one line, replies `ack: <line>`
* `client`: connects, sends a line, prints reply

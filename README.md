# ipc-demo-c
## The simplest useful C IPC demo:

Unix domain socket  A real client/server IPC on one machine, no network, no 'parent/child only' limitation.

## Structure

```
.
├── c
│   └── simple-ipc-demo
├── haskell
│   └── ipc-demo
│       ├── client
│       └── server
├── LICENSE
└── README.md
```

* `c/`: C version of the demo
* `haskell/`: Haskell version of the demo

Each demo has a `client` and `server` program.

I will add other demo related to IPC later.

## IPC DEMO

* `server`: listens on `./ipc.sock`, accepts one client at a time one line, replies `ack: <line>`
* `client`: connects, sends a line, prints reply

## Some inspections

* `ls -la ipc.sock` to see the socket file created
* `lsof -U | grep ipc.sock` to see the socket file in use


## Memo

### What did I observe?

* Two program, client and server which are compiled separately
  can communicate via socket file `./ipc.sock` (Unix domain socket?).
* I want to see real world IPC not just parent/child process communication.

### The deference between program and process

A program is a file on disk that contains instructions and static data.
A process is an operating-system-managed execution context created from a program,
consisting of its own virtual address space, execution state, and associated resources.

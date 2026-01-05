all:
	mkdir -p bin
	cc -Wall -Wextra -O2 -o bin/server src/server.c
	cc -Wall -Wextra -O2 -o bin/client src/client.c

clean:
	rm -rf bin

run: all
	./bin/server & \
	sleep 1; \
	./bin/client "hi from another process\n"

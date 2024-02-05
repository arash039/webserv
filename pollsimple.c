#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

int main() {
	char name[256];
	int counter = 0;

	struct pollfd mypoll;
	memset(&mypoll, 0 , sizeof(mypoll));
	mypoll.fd = 0;
	mypoll.events = POLLIN;

	printf("type your name:\n");

	while (1) {
		if (poll(&mypoll, 1, 1000) == 1) {
			read(0, name, sizeof(name));
			printf("hello %s", name);
			break;
		}
		else
			counter++;
	}
	printf("it took %d ms to type your name\n", 1000 * counter);
	return (0);	
}
#include "client.h"

void *worker1(void *);
void *worker2(void *);
int fd;

int main() {
	
	// preparations
	fd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51453);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	connect(fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	printf("connect success\n");

	return 0;
}

void *worker1(void *arg) {
	char str1[500];
	while (fgets(str1, 500, stdin)) {
		int byte_write = write(fd, str1, strlen(str1) - 1);
		if (byte_write == -1) exit(-1);
		str1[0] = '\0';
	}
	return NULL;
}

void *worker2(void *arg) {
	char str2[500];
	while (int n = (read(fd, str2, 500))) {
	 	str2[n] = '\0';
	 	fputs(str2, stdout);
	}
	return NULL;
}

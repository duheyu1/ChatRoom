#include "server.h"

void *worker1(void *);
void *worker2(void *);
int connfd;

int main() {
	
	printf("haha\n");
	// preparations
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket success\n");

	sockaddr_in servaddr, cliaddr;
	socklen_t clilen;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(51453);

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	printf("bind success\n");

	listen(listenfd, 5);
	printf("listen success\n");

	connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
	printf("connect success %d\n", connfd);

	

	return 0;
}

void *worker1(void *arg) {
	char str1[500];
	while (fgets(str1, 500, stdin)) {
		int byte_sent = write(connfd, str1, strlen(str1));
		if (byte_sent == -1) exit(-1);
		str1[0] = '\0';
	}
	return NULL;
}

void *worker2(void *arg) {
	char str2[500];
	while (true) {
		int n = read(connfd, str2, 500);
		if (n == 0) {
			printf("close normally\n");
			exit(0);
		}
		if (n == -1) {
			perror("failure\n");
			exit(-1);
		}
		str2[n] = '\0';
		printf("%s\n", str2);
	}
	return NULL;
}

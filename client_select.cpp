#include "client.h"

#define MAXLINE 500

int max(int i, int j) {
    if (i > j)
        return i;
    else
        return j;
}

int main() {
	
	// preparations
	int fd = socket(AF_INET, SOCK_STREAM, 0);

    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
	sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51453);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	connect(fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	printf("connect success\n");

    FD_ZERO(&rset);
	for ( ; ; ) {
        FD_SET(fd, &rset);
        FD_SET(fileno(stdin), &rset);
        select(max(fileno(stdin), fd) + 1, &rset, NULL, NULL, NULL);

        if ( FD_ISSET(fd, &rset) ) {
		    int n = read(fd, recvline, MAXLINE);
		    if (n == 0) {
			    printf("close normally\n");
			    exit(0);
		    }
		    if (n == -1) {
			    perror("failure\n");
			    exit(-1);
		    }
		    recvline[n] = '\0';
		    printf("%s", recvline);
        }

        if ( FD_ISSET(fileno(stdin), &rset) ) {
            fgets(sendline, 500, stdin);
		    int byte_sent = write(fd, sendline, strlen(sendline));
		    if (byte_sent == -1) exit(-1);
		    sendline[0] = '\0';
        }
    }

	return 0;
}
#include "server.h"

#define MAXLINE 500

int max(int i, int j) {
    if (i > j)
        return i;
    else
        return j;
}

int main() {
    	
	printf("haha\n");
	// preparations
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket success\n");

    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
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

	int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
	printf("connect success %d\n", connfd);

    FD_ZERO(&rset);
	for ( ; ; ) {
        FD_SET(connfd, &rset);
        FD_SET(fileno(stdin), &rset);
        select(max(fileno(stdin), connfd) + 1, &rset, NULL, NULL, NULL);

        if ( FD_ISSET(connfd, &rset) ) {
		    int n = read(connfd, recvline, MAXLINE);
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
		    int byte_sent = write(connfd, sendline, strlen(sendline));
		    if (byte_sent == -1) exit(-1);
		    sendline[0] = '\0';
        }
    }

	return 0;
}
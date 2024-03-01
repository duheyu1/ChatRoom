#include "server.h"

#define MAXLINE 500

int max(int i, int j) {
    if (i > j) return i;
    return j;
}

int main() {
    	
	// preparations
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in servaddr, cliaddr;
	socklen_t clilen;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(51453);

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 5);

	int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

	// main loop, check for input and socket
	fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
    FD_ZERO(&rset);
	for ( ; ; ) {
        FD_SET(connfd, &rset);
        FD_SET(fileno(stdin), &rset);
		fflush(stdout);
        select(max(fileno(stdin), connfd) + 1, &rset, NULL, NULL, NULL);

		// check for socket
        if ( FD_ISSET(connfd, &rset) ) {
			sprintf(recvline, "%s", "as: ");
		    int n = read(connfd, &recvline[4], MAXLINE - 4);
		    if (n == 0) {
			    printf("close normally\n");
			    exit(0);
		    }
		    if (n == -1) {
			    perror("failure\n");
			    exit(-1);
		    }
		    recvline[n + 4] = '\0';
		    fputs(recvline, stdout);
        }

		// check for input
        if ( FD_ISSET(fileno(stdin), &rset) ) {
            fgets(sendline, MAXLINE, stdin);
		    int byte_sent = write(connfd, sendline, strlen(sendline));
		    if (byte_sent == -1) exit(-1);
        }
    }

	return 0;
}
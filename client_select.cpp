#include "client.h"

#define MAXLINE 500

int max(int i, int j) {
    if (i > j) return i;
    return j;
}

int main(int argc, char **argv) {

	if (argc != 2) printf("two few argument\n");
	
	// preparations
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51453);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	// main loop, check for input and socket
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
    FD_ZERO(&rset);
	for ( ; ; ) {
        FD_SET(fd, &rset);
        FD_SET(fileno(stdin), &rset);
		fflush(stdout);
        select(max(fileno(stdin), fd) + 1, &rset, NULL, NULL, NULL);

		// check for socket
        if ( FD_ISSET(fd, &rset) ) {
			sprintf(recvline, "%s", "as: ");
		    int n = read(fd, &recvline[4], MAXLINE - 4);
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
		    int byte_sent = write(fd, sendline, strlen(sendline));
		    if (byte_sent == -1) exit(-1);
        }
    }

	return 0;
}
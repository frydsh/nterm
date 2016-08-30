#include "nterm.h"

void err_sys(char * msg) {
	printf("%s\n",msg);
	exit(1);
}
int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr) {
	int		n;
again:
	if ((n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
			err_sys("accept error");
	}
	return(n);
}
void
Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
	if (bind(fd, sa, salen) < 0)
		err_sys("bind error");
}
int
Connect(int fd, const struct sockaddr *sa, socklen_t salen) {
	return(connect(fd, sa, salen));
}
/* include Listen */
void
Listen(int fd, int backlog) {
	char	*ptr;
	/*4can override 2nd argument with environment variable */
	if ((ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);
	if (listen(fd, backlog) < 0)
		err_sys("listen error");
}
/* end Listen */
int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout) {
	int		n;
	if ((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		err_sys("select error");
	return(n);		/* can return 0 on timeout */
}
void
Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen) {
	if (setsockopt(fd, level, optname, optval, optlen) < 0)
		err_sys("setsockopt error");
}
void
Shutdown(int fd, int how) {
	if (shutdown(fd, how) < 0)
		err_sys("shutdown error");
}
/* include Socket */
int
Socket(int family, int type, int protocol) {
	int		n;
	if ((n = socket(family, type, protocol)) < 0)
		err_sys("socket error");
	return(n);
}
/* end Socket */
void
Socketpair(int family, int type, int protocol, int *fd) {
	int		n;
	if ((n = socketpair(family, type, protocol, fd)) < 0)
		err_sys("socketpair error");
}
void
Write(int fd, void *ptr, size_t nbytes) {
	if (write(fd, ptr, nbytes) != nbytes)
		err_sys("write error");
}
struct hostent *Gethostbyname(const char *host){
	struct hostent * hptr;
	if ((hptr=gethostbyname(host))==NULL) {
		err_sys("gethostbyname error");
	} else {
		return hptr;
	}
}

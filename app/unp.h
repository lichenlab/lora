#ifndef _UNP_H_
#define _UNP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>


#include <time.h>
#include <poll.h>
#include <pthread.h>
/*
#include <sys/time.h>
#include <sys/select.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <sys/filio.h>
#include <sys/sockio.h>
*/

#ifndef OPEN_MAX
#define OPEN_MAX	256
#endif

#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
#endif


typedef void Sigfunc(int);


#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default file access permissions for new files */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
					/* default permissions for new directories */


#define min(a,b)	((a) < (b) ? (a) : (b))
#define max(a,b)	((a) > (b) ? (a) : (b))

#define SA	struct sockaddr


#define MAXLINE		4096
#define LISTENQ		1024
#define BUFFSIZE	8192

#define SERV_PORT	12001


void err_msg(const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);
void err_dump(const char *, ...);
void err_quit(const char *, ...);

#if 0
/* lib/wrapsock.c */
int		 Socket(int, int, int);
void	 Listen(int, int);
int		 Accept(int, SA *, socklen_t *);
void	 Bind(int, const SA *, socklen_t);
void	 Connect(int, const SA *, socklen_t);
void	 Getpeername(int, SA *, socklen_t *);
void	 Getsockname(int, SA *, socklen_t *);

ssize_t	 Recv(int, void *, size_t, int);
ssize_t	 Recvfrom(int, void *, size_t, int, SA *, socklen_t *);
void	 Send(int, const void *, size_t, int);
void	 Sendto(int, const void *, size_t, int, const SA *, socklen_t);
void	 Shutdown(int, int);

int		 Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int		 Poll(struct pollfd *, unsigned long, int);



/* lib/writen.c */
void	 Writen(int, void *, size_t);

/* lib/readn.c */
ssize_t	 Readn(int, void *, size_t);

/* lib/readline.c */
ssize_t	 Readline(int, void *, size_t);

/* lib/wrapunix.c */
int		 Open(const char *, int, mode_t);
void	 Write(int, void *, size_t);
ssize_t	 Read(int, void *, size_t);
void	 Close(int);
int		 Fcntl(int, int, int);
pid_t	 Fork(void);
void	 Pipe(int *fds);

/* lib/wrapstdio.c */
void	 Fclose(FILE *);
FILE	*Fopen(const char *, const char *);
FILE	*Fdopen(int, const char *);
char	*Fgets(char *, int, FILE *);
void	 Fputs(const char *, FILE *);

/* lib/wraplib.c */
const char		*Inet_ntop(int, const void *, char *, size_t);
void			 Inet_pton(int, const char *, void *);


/* lib/sock_ntop.c */
char	*Sock_ntop(const SA *, socklen_t);
char	*Sock_ntop_host(const SA *, socklen_t);

#endif




/* stdlib.h
getenv();
atoi();
 */

/*
mode_t
socklen_t

time();
ctime();
*/


#endif

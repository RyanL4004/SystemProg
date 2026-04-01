// socktypes.h
// Kyle Mather
// Typedefs for common socket system types used by mom and kid modules.
// Comments below describe key socket data structures.
//
// struct sockaddr         { sa_family_t sa_family; char sa_data[14]; };
// struct sockaddr_in      { sa_family_t sin_family; in_port_t sin_port;
//                           struct in_addr sin_addr; };
// struct pollfd           { int fd; short events; short revents; };
// struct hostent          { char *h_name; char **h_aliases; int h_addrtype;
//                           int h_length; char **h_addr_list; };

#ifndef SOCKTYPES_H
#define SOCKTYPES_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

typedef struct sockaddr    SA;
typedef struct sockaddr_in SA_IN;
typedef struct hostent     HOSTENT;
typedef struct pollfd      POLLFD;

// Declared here; defined in each module's main .cpp file.
extern void fatalp(const char* msg);

#endif

// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <errno.h> 
#include <time.h>
#include <netdb.h> 
#include <string.h>
#include <netinet/ip_icmp.h>
#include <sys/select.h>
#include "icmp.h"

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#define PORT 27015 
#define PORT_UDP 13456
#define TCP_PORT 3457
#define HTTP_PORT 8880
#define DNS_PORT 5001
#define DEFAULT_BUFLEN 1024
#define HTTPS_PORT "9898"

const char *get_content_type(const char* path) {
	const char *last_dot = strrchr(path, '.');
	if (last_dot) {
		if (strcmp(last_dot, ".css") == 0) return "text/css";
		if (strcmp(last_dot, ".csv") == 0) return "text/csv";
		if (strcmp(last_dot, ".gif") == 0) return "image/gif";
		if (strcmp(last_dot, ".htm") == 0) return "text/html";
		if (strcmp(last_dot, ".html") == 0) return "text/html";
		if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
		if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
		if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
		if (strcmp(last_dot, ".js") == 0) return "application/javascript";
		if (strcmp(last_dot, ".json") == 0) return "application/json";
		if (strcmp(last_dot, ".png") == 0) return "image/png";
		if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
		if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
		if (strcmp(last_dot, ".txt") == 0) return "text/plain";
	}

	return "application/octet-stream";
}

SOCKET create_socket(const char* host, const char *port) {
	//printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *bind_address;
	getaddrinfo(host, port, &hints, &bind_address);

	//printf("Creating socket...\n");
	SOCKET socket_listen;
	socket_listen = socket(bind_address->ai_family,
		bind_address->ai_socktype, bind_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_listen)) {
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		exit(1);
	}

	//printf("Binding socket to local address...\n");
	if (bind(socket_listen,
		bind_address->ai_addr, bind_address->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
		exit(1);
	}
	freeaddrinfo(bind_address);

	//printf("Listening...\n");
	if (listen(socket_listen, 10) < 0) {
		fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
		exit(1);
	}

	return socket_listen;
}

#define MAX_REQUEST_SIZE 2047

struct client_info {
	socklen_t address_length;
	struct sockaddr_storage address;
	SOCKET socket;
	SSL *ssl;
	char request[MAX_REQUEST_SIZE + 1];
	int received;
	struct client_info *next;
};

static struct client_info *clients = 0;

struct client_info *get_client(SOCKET s) {
	struct client_info *ci = clients;

	while (ci) {
		if (ci->socket == s)
			break;
		ci = ci->next;
	}

	if (ci) return ci;
	struct client_info *n =
		(struct client_info*) calloc(1, sizeof(struct client_info));

	if (!n) {
		fprintf(stderr, "Out of memory.\n");
		exit(1);
	}

	n->address_length = sizeof(n->address);
	n->next = clients;
	clients = n;
	return n;
}


void drop_client(struct client_info *client) {
	SSL_shutdown(client->ssl);
	CLOSESOCKET(client->socket);
	SSL_free(client->ssl);

	struct client_info **p = &clients;

	while (*p) {
		if (*p == client) {
			*p = client->next;
			free(client);
			return;
		}
		p = &(*p)->next;
	}

	exit(1);
}


const char *get_client_address(struct client_info *ci) {
	static char address_buffer[100];
	getnameinfo((struct sockaddr*)&ci->address,
		ci->address_length,
		address_buffer, sizeof(address_buffer), 0, 0,
		NI_NUMERICHOST);
	return address_buffer;
}




fd_set wait_on_clients(SOCKET server) {
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(server, &reads);
	SOCKET max_socket = server;

	struct client_info *ci = clients;

	while (ci) {
		FD_SET(ci->socket, &reads);
		if (ci->socket > max_socket)
			max_socket = ci->socket;
		ci = ci->next;
	}

	if (select(max_socket + 1, &reads, 0, 0, 0) < 0) {
		fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
		exit(1);
	}

	return reads;
}


void send_400(struct client_info *client) {
	const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
		"Connection: close\r\n"
		"Content-Length: 11\r\n\r\nBad Request";
	SSL_write(client->ssl, c400, strlen(c400));
	drop_client(client);
}

void send_404(struct client_info *client) {
	const char *c404 = "HTTP/1.1 404 Not Found\r\n"
		"Connection: close\r\n"
		"Content-Length: 9\r\n\r\nNot Found";
	SSL_write(client->ssl, c404, strlen(c404));
	drop_client(client);
}



void serve_resource(struct client_info *client, char *path, char *message) {
	char full_path[128] = "index.html";
	FILE *fp = fopen(full_path, "rb");
	if (!fp) {
		send_404(client);
		return;
	}
	fseek(fp, 0L, SEEK_END);
	size_t cl = ftell(fp);
	rewind(fp);

	const char *ct = get_content_type(full_path);

	#define BSIZE 1024
	char buffer[BSIZE];
	sprintf(buffer, "HTTP/1.1 200 OK\r\n");
	SSL_write(client->ssl, buffer, strlen(buffer));

	sprintf(buffer, "%s\r\n", message);
	SSL_write(client->ssl, buffer, strlen(buffer));
	sprintf(buffer, "\r\n");
	SSL_write(client->ssl, buffer, strlen(buffer));

	int r = fread(buffer, 1, BSIZE, fp);
	while (r) {
		SSL_write(client->ssl, buffer, r);
		r = fread(buffer, 1, BSIZE, fp);
	}

	fclose(fp);
	drop_client(client);
}

